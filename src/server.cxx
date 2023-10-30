module;
#include <array>
#include <expected>
#include <iostream>
#include <ranges>
#include <string>

// clang-format off
#include <asio.hpp>
// clang-format on
#include <asio/experimental/awaitable_operators.hpp>
export module modbus:server;
import :tcp;
import :error;
import :packet;
import :deserialize;
import :function;

namespace modbus {

namespace ip = asio::ip;
using asio::awaitable;
using asio::detached;
using asio::steady_timer;
using asio::use_awaitable;
using ip::tcp;
using std::chrono::steady_clock;
using std::chrono_literals::operator""s;
using std::chrono_literals::operator""min;
using asio::experimental::awaitable_operators::operator||;

auto handle_request(tcp_mbap const& header, std::ranges::range auto data, auto&& handler)
    -> std::expected<std::vector<uint8_t>, modbus::errc_t> {
  auto req_variant = impl::deserialize_request(std::span(data), static_cast<function_e>(data[0]));
  if (!req_variant) {
    return std::unexpected(modbus::errc_t::illegal_data_value);
  }
  auto req = req_variant.value();

  auto resp = std::visit(
      [&](auto& request) -> std::expected<std::vector<uint8_t>, modbus::errc_t> {
        modbus::errc_t error = modbus::errc_t::no_error;
        response::responses resp = handler->handle(header.unit, request, error);
        if (error) {
          return std::unexpected(error);
        }
        return impl::serialize_response(resp);
      },
      req);

  if (!resp) {
    return std::unexpected(resp.error());
  }
  return resp.value();
}

struct connection_state {
  explicit connection_state(tcp::socket&& client) : client_(std::move(client)) {}

  tcp::socket client_;
};

auto timeout(steady_clock::duration dur) -> awaitable<void> {
  steady_timer timer(co_await asio::this_coro::executor);
  timer.expires_after(dur);
  co_await timer.async_wait(use_awaitable);
}

auto build_error_buffer(tcp_mbap req_header, uint8_t function, errc::errc_t error) -> std::array<uint8_t, 9> {
  std::array<uint8_t, 9> error_buffer{};
  tcp_mbap* header = std::launder(reinterpret_cast<tcp_mbap*>(error_buffer.data()));
  header->length = htons(3);
  header->transaction = htons(req_header.transaction);
  header->unit = req_header.unit;
  error_buffer[7] = function | 0x80;
  error_buffer[8] = static_cast<uint8_t>(error);
  return error_buffer;
}

auto handle_connection(tcp::socket client, auto&& handler) -> awaitable<void> {
  auto state = std::make_shared<connection_state>(std::move(client));
  std::array<uint8_t, 7> header_buffer{};
  std::array<uint8_t, 1024> request_buffer{};
  for (;;) {
    auto result = co_await (
        state->client_.async_read_some(asio::buffer(header_buffer, tcp_mbap::size), asio::as_tuple(asio::use_awaitable)) ||
        timeout(60s));
    if (result.index() == 1) {
      // Timeout
      std::cerr << "timeout client: " << state->client_.remote_endpoint() << " Disconnecting!" << '\n';
      break;
    }
    auto [ec, count] = std::get<0>(result);
    if (ec) {
      std::cerr << "error client: " << state->client_.remote_endpoint() << " Disconnecting!" << '\n';
      break;
    }
    if (count < tcp_mbap::size) {
      std::cerr << "packet size to small for header " << count << " " << state->client_.remote_endpoint()
                << " Disconnecting!" << std::endl;
      break;
    }
    // Deserialize the request
    auto header = tcp_mbap::from_bytes(header_buffer);

    if (header.length < 2) {
      co_await async_write(state->client_, asio::buffer(build_error_buffer(header, 0, errc::illegal_function), count),
                           use_awaitable);
      continue;
    }

    // Read the request body
    auto [request_ec, request_count] = co_await state->client_.async_read_some(
        asio::buffer(request_buffer, header.length - 1), asio::as_tuple(asio::use_awaitable));
    if (request_ec) {
      std::cerr << "error client: " << state->client_.remote_endpoint() << " Disconnecting!" << '\n';
      break;
    }

    if (request_count + 1 < static_cast<size_t>(header.length)) {
      std::cerr << "packet size to small for body " << request_count << " " << state->client_.remote_endpoint()
                << " Disconnecting!" << std::endl;
      co_await async_write(state->client_, asio::buffer(build_error_buffer(header, 0, errc::illegal_data_value), count),
                           use_awaitable);
      continue;
    }

    // Handle the request
    auto resp = handle_request(header, request_buffer, handler);
    if (resp) {
      header.length = resp.value().size() + 1;
      auto header_bytes = header.to_bytes();
      std::array<asio::const_buffer, 2> buffs{ asio::buffer(header_bytes), asio::buffer(resp.value()) };
      co_await async_write(state->client_, buffs, use_awaitable);
    } else {
      co_await async_write(state->client_, asio::buffer(build_error_buffer(header, 0, resp.error()), count), use_awaitable);
    }
  }
  // state->client_.close();
  steady_timer wait_a_minute(co_await asio::this_coro::executor);
  wait_a_minute.expires_after(5min);
  co_await wait_a_minute.async_wait(asio::use_awaitable);
}

export template <typename server_handler_t>
struct server {
  explicit server(asio::io_context& io_context, std::shared_ptr<server_handler_t>& handler, int port)
      : acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), handler_(handler) {}

  void start() { co_spawn(acceptor_.get_executor(), listen(), detached); }

private:
  auto listen() -> awaitable<void> {
    for (;;) {
      auto client = co_await acceptor_.async_accept(use_awaitable);
      client.set_option(asio::ip::tcp::no_delay(true));
      client.set_option(asio::socket_base::keep_alive(true));

      co_spawn(acceptor_.get_executor(), handle_connection(std::move(client), handler_), detached);

      co_await listen();
    }
  }

  asio::ip::tcp::acceptor acceptor_;
  std::shared_ptr<server_handler_t> handler_;
};

}  // namespace modbus
