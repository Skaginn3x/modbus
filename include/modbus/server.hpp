#pragma once

#include <string>
#include <array>
#include <cstdint>
#include <ostream>
#include <set>
#include <expected>

#include <boost/asio.hpp>
#include <boost/asio/as_tuple.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <iostream>
#include <algorithm>
#include <functional>

#include "functions.hpp"
#include "tcp.hpp"
#include "request.hpp"
#include "response.hpp"
#include "error.hpp"
#include "impl/serialize.hpp"
#include "impl/deserialize.hpp"

namespace asio = boost::asio;
namespace ip = asio::ip;
using ip::tcp;
using asio::awaitable;
using asio::use_awaitable;
using asio::detached;

//using awaitable_tuple = asio::as_tuple(use_awaitable);

using std::chrono::steady_clock;
using asio::steady_timer;

using namespace std::chrono_literals;
using namespace asio::experimental::awaitable_operators;

namespace modbus {

    template<typename Request>
    std::expected<std::vector<uint8_t>, modbus::errc_t>
    handle_request(tcp_mbap header, const uint8_t *data, size_t data_size, auto &&handler) {
        Request req;
        std::error_code err;
        impl::deserialize(data, data_size, req, err);

        if (err) {
            return std::unexpected(modbus::errc_t::illegal_data_value);
        }
        modbus::errc_t modbus_error = modbus::errc_t::no_error;
        typename Request::response resp = handler->handle(header.unit, req, modbus_error);
        if (modbus_error) {
            return std::unexpected(modbus_error);
        }
        asio::streambuf buffer;
        auto out = std::ostreambuf_iterator<char>(&buffer);
        impl::serialize(out, resp);
        std::vector<uint8_t> resp_buffer(buffer.size(), 0);
        std::copy_n(asio::buffers_begin(buffer.data()), buffer.size(), resp_buffer.begin());
        return resp_buffer;
    }

    struct connection_state {
        connection_state(tcp::socket client) : client_(std::move(client)) {
        }

        tcp::socket client_;
    };

    awaitable<void> timeout(steady_clock::duration tp) {
        steady_timer timer(co_await asio::this_coro::executor);
        timer.expires_after(tp);
        co_await timer.async_wait(use_awaitable);
    }

    std::array<uint8_t, 9> build_error_buffer(tcp_mbap req_header, uint8_t function, errc::errc_t error) {
        std::array<uint8_t, 9> error_buffer{};
        tcp_mbap *header = std::launder(reinterpret_cast<tcp_mbap *>(error_buffer.data()));
        header->length = htons(3);
        header->transaction = htons(req_header.transaction);
        header->unit = req_header.unit;
        error_buffer[7] = function | 0x80;
        error_buffer[8] = static_cast<uint8_t>(error);
        return error_buffer;
    }

    std::array<asio::const_buffer, 2> encode_resp(std::vector<uint8_t> resp, tcp_mbap *header) {
        header->length = resp.size() + 1;
        header->length = htons(header->length);
        header->protocol = htons(header->protocol);
        header->transaction = htons(header->transaction);
        std::array<asio::const_buffer, 2> buffs{
                asio::buffer(std::launder(reinterpret_cast<uint8_t *>(header)), tcp_mbap::size()), asio::buffer(resp)};
        return buffs;
    }

    awaitable<void> handle_connection(tcp::socket client, auto &&handler) {
        auto state = std::make_shared<connection_state>(std::move(client));
        std::array<uint8_t, 7> header_buffer{};
        std::array<uint8_t, 1024> request_buffer{};
        for (;;) {
            auto result = co_await (
                    state->client_.async_read_some(asio::buffer(header_buffer, tcp_mbap::size()),
                                                   asio::as_tuple(asio::use_awaitable)) ||
                    timeout(60s));
            if (result.index() == 1) {
                // Timeout
                std::cerr << "timeout client: " << state->client_.remote_endpoint() << " Disconnecting!" << std::endl;
                break;
            }
            auto [ec, count] = std::get<0>(result);
            if (ec) {
                std::cerr << "error client: " << state->client_.remote_endpoint() << " Disconnecting!" << std::endl;
                break;
            }
            if (count < tcp_mbap::size()) {
                std::cerr << "packet size to small for header " << count << " " << state->client_.remote_endpoint()
                          << " Disconnecting!" << std::endl;
                break;
            }
            // Deserialize the request
            tcp_mbap *header = std::launder(reinterpret_cast<tcp_mbap *>(header_buffer.data()));
            header->transaction = ntohs(header->transaction);
            header->protocol = ntohs(header->protocol);
            header->length = ntohs(header->length);
            std::cout << *header << std::endl;

            if (header->length < 2) {
                co_await async_write(state->client_,
                                     asio::buffer(build_error_buffer(*header, 0, errc::illegal_function), count),
                                     use_awaitable);
                continue;
            }

            // Read the request body
            auto [request_ec, request_count] = co_await state->client_.async_read_some(
                    asio::buffer(request_buffer, header->length - 1), asio::as_tuple(asio::use_awaitable));
            if (request_ec) {
                std::cerr << "error client: " << state->client_.remote_endpoint() << " Disconnecting!" << std::endl;
                break;
            }

            if (request_count < header->length - 1) {
                std::cerr << "packet size to small for body " << request_count << " "
                          << state->client_.remote_endpoint() << " Disconnecting!" << std::endl;
                co_await async_write(state->client_,
                                     asio::buffer(build_error_buffer(*header, 0, errc::illegal_data_value), count),
                                     use_awaitable);
                continue;
            }

            uint8_t function_code = request_buffer[0];
            //co_await async_write(state->client_, encode_resp(resp, header), use_awaitable);
            // Handle the request
            auto resp = [&]() -> std::expected<std::vector<uint8_t>, modbus::errc_t> {
                switch (function_code) {
                    case functions::read_coils:
                        return handle_request<request::read_coils>(*header, request_buffer.data(), request_count,
                                                                   handler);
                    case functions::read_discrete_inputs:;
                        return handle_request<request::read_discrete_inputs>(*header, request_buffer.data(),
                                                                             request_count, handler);
                    case functions::read_holding_registers:;
                        return handle_request<request::read_holding_registers>(*header, request_buffer.data(),
                                                                               request_count, handler);
                    case functions::read_input_registers:;
                        return handle_request<request::read_input_registers>(*header, request_buffer.data(),
                                                                             request_count, handler);
                    case functions::write_single_coil:;
                        return handle_request<request::write_single_coil>(*header, request_buffer.data(), request_count,
                                                                          handler);
                    case functions::write_single_register:;
                        return handle_request<request::write_single_register>(*header, request_buffer.data(),
                                                                              request_count, handler);
                    case functions::write_multiple_coils:;
                        return handle_request<request::write_multiple_coils>(*header, request_buffer.data(),
                                                                             request_count, handler);
                    case functions::write_multiple_registers:;
                        return handle_request<request::write_multiple_registers>(*header, request_buffer.data(),
                                                                                 request_count, handler);
                    default:
                        return std::unexpected(errc::illegal_function);
                }
            }();
            if (resp) {
                size_t n = co_await async_write(state->client_, encode_resp(resp.value(), header), use_awaitable);
                std::cout << "Wrote " << n << "\n";
            } else {
                co_await async_write(state->client_,
                                     asio::buffer(build_error_buffer(*header, 0, resp.error()), count),
                                     use_awaitable);
            }
        }
        state->client_.close();
    }


    template<typename server_handler_t>
    struct server {
        server(asio::io_context &io_context, std::shared_ptr<server_handler_t> &handler, int port) :
                acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
                handler_(handler) {
        }

        ~server() {
            std::cerr << "server dtor" << std::endl;
        }

        void start() {
            co_spawn(acceptor_.get_executor(), listen(), detached);
        }

    private:
        awaitable<void> listen() {
            for (;;) {
                auto client = co_await acceptor_.async_accept(use_awaitable);
                client.set_option(asio::ip::tcp::no_delay(true));
                client.set_option(asio::socket_base::keep_alive(true));

                std::cout << "Connection opened from " << client.remote_endpoint() << "\n";
                // std::make_shared<Connection<server_handler_t>>(std::move(client), handler_)->start();

                co_spawn(acceptor_.get_executor(), handle_connection(std::move(client), handler_), detached);

                co_await listen();
            }
        }

        asio::ip::tcp::acceptor acceptor_;
        std::shared_ptr<server_handler_t> handler_;
    };

} // namespace modbus

