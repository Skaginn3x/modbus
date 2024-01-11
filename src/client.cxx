// Copyright (c) 2023, Skaginn3x (https://Skaginn3x.com)
module;

// clang-format off
#include <expected>
#include <asio.hpp>
// clang-format on

export module modbus:client;
import :packet;
import :tcp;
import :constants;
import :error;

namespace modbus {
namespace ip = asio::ip;

using asio::async_compose;
using tcp = ip::tcp;

/// A connection to a Modbus server.
export class client {
protected:
  /// Execution context
  asio::io_context& ctx_;

  /// The socket to use.
  tcp::socket socket_;

  /// Next transaction ID.
  std::uint16_t next_id_ = 0;

  /// Track connected state of client.
  bool connected_{ false };

  /// Socket options
  asio::ip::tcp::no_delay no_delay_option{ true };
  asio::socket_base::keep_alive keep_alive_option{ true };

public:
  /// Construct a client.
  explicit client(asio::io_context& io_context) : ctx_{ io_context }, socket_{ io_context } {}

  /// Get the IO executor used by the client.
  auto io_executor() -> tcp::socket::executor_type { return socket_.get_executor(); };

  /// Connect to a server.
  template <typename completion_token>
  auto connect(const std::string& hostname, const std::string& port, completion_token&& token) ->
      typename asio::async_result<std::decay_t<completion_token>, void(std::error_code)>::return_type {
    return async_compose<completion_token, void(std::error_code)>(
        [&](auto& self) {
          co_spawn(
              ctx_,
              [&, self = std::move(self)]() mutable -> asio::awaitable<void> {
                tcp::resolver resolver{ co_await asio::this_coro::executor };
                const tcp::resolver::query query{ hostname, port };
                auto [error, endpoint] = co_await resolver.async_resolve(query, asio::as_tuple(asio::use_awaitable));
                if (error) {
                  self.complete(error);
                  co_return;
                }

                auto [connect_error, _] =
                    co_await asio::async_connect(socket_, endpoint, asio::as_tuple(asio::use_awaitable));
                if (connect_error) {
                  self.complete(connect_error);
                  co_return;
                }

                connected_ = true;

                // Set socket options as recommended by the modbus spec.
                socket_.set_option(no_delay_option);
                socket_.set_option(keep_alive_option);

                self.complete({});

                co_return;
              },
              asio::detached);
        },
        token, ctx_);
  }

  /// Disconnect from the server.
  /**
   * Any remaining transaction callbacks will be invoked with an EOF error.
   */
  void close() {
    if (socket_.is_open()) {
      // Shutdown and close socket.
      socket_.shutdown(asio::socket_base::shutdown_type::shutdown_both);
      socket_.close();
    }
    connected_ = false;
  }

  /// Check if the connection to the server is open.
  /**
   * \return True if the connection to the server is open.
   */
  auto is_open() -> bool { return socket_.is_open(); }

  /// Check if the client is connected.
  auto is_connected() -> bool { return is_open() && connected_; }

  /// Read a number of coils from the connected server.
  template <typename completion_token>
  auto read_coils(std::uint8_t unit, std::uint16_t address, std::uint16_t count, completion_token&& token) {
    return send_message<completion_token>(unit, request::read_coils{ address, count }, std::forward<decltype(token)>(token));
  }

  /// Read a number of discrete inputs from the connected server.
  template <typename completion_token>
  auto read_discrete_inputs(std::uint8_t unit, std::uint16_t address, std::uint16_t count, completion_token&& token) {
    return send_message<completion_token>(unit, request::read_discrete_inputs{ address, count },
                                          std::forward<decltype(token)>(token));
  }

  /// Read a number of holding registers from the connected server.
  template <typename completion_token>
  auto read_holding_registers(std::uint8_t unit, std::uint16_t address, std::uint16_t count, completion_token&& token) {
    return send_message<completion_token>(unit, request::read_holding_registers{ address, count },
                                          std::forward<decltype(token)>(token));
  }

  /// Read a number of input registers from the connected server.
  template <typename completion_token>
  auto read_input_registers(std::uint8_t unit, std::uint16_t address, std::uint16_t count, completion_token&& token) {
    return send_message<completion_token>(unit, request::read_input_registers{ address, count },
                                          std::forward<decltype(token)>(token));
  }

  /// Write to a single coil on the connected server.
  template <typename completion_token>
  auto write_single_coil(std::uint8_t unit, std::uint16_t address, bool value, completion_token&& token) {
    return send_message<completion_token>(unit, request::write_single_coil{ address, value },
                                          std::forward<decltype(token)>(token));
  }

  /// Write to a single register on the connected server.
  template <typename completion_token>
  auto write_single_register(std::uint8_t unit, std::uint16_t address, std::uint16_t value, completion_token&& token) {
    return send_message<completion_token>(unit, request::write_single_register{ address, value },
                                          std::forward<decltype(token)>(token));
  }

  /// Write to a number of coils on the connected server.
  template <typename completion_token>
  auto write_multiple_coils(std::uint8_t unit, std::uint16_t address, std::vector<bool> values, completion_token&& token) {
    return send_message<completion_token>(unit, request::write_multiple_coils{ address, std::move(values) },
                                          std::forward<decltype(token)>(token));
  }

  /// Write to a number of registers on the connected server.
  template <typename completion_token>
  auto write_multiple_registers(std::uint8_t unit,
                                std::uint16_t address,
                                std::vector<std::uint16_t> values,
                                completion_token&& token) {
    return send_message<completion_token>(unit, request::write_multiple_registers{ address, std::move(values) },
                                          std::forward<decltype(token)>(token));
  }

  /// Perform a masked write to a register on the connected server.
  /**
   * Compliant servers will set the value of the register to:
   * ((old_value AND and_mask) OR (or_mask AND NOT and_MASK))
   */
  template <typename completion_token>
  auto mask_write_register(std::uint8_t unit,
                           std::uint16_t address,
                           std::uint16_t and_mask,
                           std::uint16_t or_mask,
                           completion_token&& token) {
    return send_message<completion_token>(unit, request::mask_write_register{ address, and_mask, or_mask },
                                          std::forward<decltype(token)>(token));
  }

protected:
  /// Send a Modbus request to the server.
  template <typename completion_token>
  auto send_message(std::uint8_t unit, auto const send_request, completion_token&& token) {
    using response_type = typename decltype(send_request)::response;
    return async_compose<completion_token, void(std::expected<response_type, std::error_code>)>(
        [&, send_request](auto& self_outer) {
          co_spawn(
              ctx_,
              [&, self = std::move(self_outer), request = std::move(send_request)]() mutable -> asio::awaitable<void> {
                assert(request.length() <= std::numeric_limits<uint16_t>::max() - 1 && "Request length too large for type");
                tcp_mbap request_header{ .transaction = ++next_id_,
                                         .protocol = static_cast<uint16_t>(0),
                                         .length = static_cast<uint16_t>(request.length() + 1U),
                                         .unit = unit };

                auto header_encoded = request_header.to_bytes();
                auto request_serialized = impl::serialize_request(request);

                std::vector<asio::const_buffer> buffers{ { asio::buffer(header_encoded),
                                                           asio::buffer(request_serialized) } };

                co_await asio::async_write(socket_, buffers, asio::use_awaitable);

                /// Buffer for read operations.
                std::array<uint8_t, tcp_mbap::size> header_buffer{};
                // Read the response
                auto [header_error, bytes_transferred] = co_await socket_.async_read_some(
                    asio::buffer(header_buffer, tcp_mbap::size), asio::as_tuple(asio::use_awaitable));
                if (header_error) {
                  self.complete(std::unexpected(header_error));
                  co_return;
                }
                auto header = tcp_mbap::from_bytes(header_buffer);

                // Make sure the message contains at least a function code. and a unit
                if (header.length < 2) {
                  self.complete(std::unexpected(modbus_error(errc::message_size_mismatch)));
                  co_return;
                }

                std::array<uint8_t, modbus_max_pdu> read_buffer{};
                auto [body_error, size_of_body] =
                    co_await socket_.async_read_some(asio::buffer(read_buffer,
                                                                  header.length - 1),  // -1 header.unit is inside the count
                                                     asio::as_tuple(asio::use_awaitable));
                if (body_error) {
                  self.complete(std::unexpected(body_error));
                  co_return;
                }
                if (size_of_body + 1 != static_cast<size_t>(header.length)) {
                  self.complete(std::unexpected(modbus_error(errc::message_size_mismatch)));
                  co_return;
                }
                // Parse and process all complete messages in the buffer.
                auto function = response_type::function;
                auto response = impl::deserialize_response(read_buffer, function);

                // Handle deserialization errors in TCP MBAP.
                // Cant send an error to a specific transaction and can't continue to read from the connection.
                if (!response) {
                  self.complete(std::unexpected(response.error()));
                  co_return;
                }
                if (std::visit(
                        [&](auto res) {
                          // Function codes 128 and above are exception responses.
                          if (static_cast<uint8_t>(res.function) >= 128) {
                            self.complete(std::unexpected{
                                modbus_error(header.length >= 3 ? errc_t(res.function) : errc::message_size_mismatch) });
                            return true;
                          }
                          return false;
                        },
                        response.value())) {
                  co_return;
                }

                auto* value = std::get_if<response_type>(&response.value());
                if (value) {
                  self.complete(std::move(*value));
                } else {
                  self.complete(std::unexpected(modbus_error(errc::unexpected_function_code)));
                }
              },
              asio::detached);
        },
        token, ctx_);
  }
};

}  // namespace modbus
