// clang-format off
#include <asio.hpp>
#include <array>
#include <boost/ut.hpp>
// clang-format on

import modbus;

int main() {
  using boost::ut::operator""_test;
  using boost::ut::operator|;
  using boost::ut::expect;

  // Setup a server to use for tests
  int port = 15502;
  asio::io_context ctx;

  auto handler = std::make_shared<modbus::default_handler>();
  modbus::server<modbus::default_handler> server{ ctx, handler, port };
  server.start();

  modbus::client client{ ctx };

  bool finished = false;

  "Client integration tests"_test = [&]() {
    co_spawn(
        ctx,
        [&]() mutable -> asio::awaitable<void> {
          auto [connect_error] =
              co_await client.connect("localhost", std::to_string(port), asio::as_tuple(asio::use_awaitable));
          expect(!connect_error);
          handler->registers[5] = 55;
          auto res = co_await client.read_holding_registers(0, 5, 1, asio::use_awaitable);
          expect(res.has_value());
          expect(res.value().values.size() == 1);
          expect(res.value().values[0] == 55);

          auto write = co_await client.write_single_register(0, 5, 54, asio::use_awaitable);
          expect(write.has_value());
          expect(handler->registers[5] == 54);
          finished = true;
          co_return;
        },
        asio::detached);
  };

  ctx.run_for(std::chrono::milliseconds(1500));
  "Finished"_test = [&]() { expect(finished); };
}