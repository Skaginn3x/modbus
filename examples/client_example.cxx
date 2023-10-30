// Copyright (c) 2023, Skaginn3x (https://skaginn3x.com)

#include <iostream>
#include <thread>
#include <vector>
// clang-format off
#include <exception> // Missing include inside asio
#include <asio.hpp>
// clang-format on

import modbus;

void on_io_error(std::error_code const& error) {
  std::cout << "Read error: " << error.message() << "\n";
}

int main(int argc, const char** argv) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " <hostname> <port>\n";
    return -1;
  }

  std::vector<std::thread> pool;
  asio::io_context ctx;

  std::string_view hostname = argv[1];
  std::string_view port = argv[2];

  modbus::client client{ ctx };

  co_spawn(
      ctx,
      [&]() -> asio::awaitable<void> {
        auto [error] =
            co_await client.connect(std::string(hostname), std::string(port), asio::as_tuple(asio::use_awaitable));
        if (error) {
          std::cerr << "Error connecting: " << error.message() << '\n';
          exit(-1);
        }
        std::cout << "Connected!" << '\n';

        for (size_t i = 0; i < 25; i++) {
          auto response = co_await client.read_holding_registers(0, 0, 15, asio::use_awaitable);
          if (!response) {
            std::cerr << "Error reading: " << response.error().message() << '\n';
            exit(-1);
          }
          std::cout << "Read registers: ";
          for (unsigned short i : response->values) {
            std::cout << "\t\t" << i << " ";
          }
          std::cout << "\n";
          auto coils_response = co_await client.read_coils(0, 0, 15, asio::use_awaitable);
          if (!coils_response) {
            std::cerr << "Error reading: " << coils_response.error().message() << '\n';
            exit(-1);
          }
          std::cout << "Read coils: ";
          for (unsigned short i : coils_response->values) {
            std::cout << "\t" << (i ? "true" : "false") << " ";
          }
          std::cout << '\n';
          std::this_thread::sleep_for(std::chrono::milliseconds{ 150 });
        }
        client.close();
      },
      asio::detached);

  ctx.run();
}
