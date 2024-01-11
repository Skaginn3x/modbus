
// Sniff raw tcp traffic on port 502 and print in hex array form
// Used to generate test data from known working modbus clients
// fe mbpoll

// clang-format off
#include <exception>
#include <asio.hpp>
#include <iostream>
// clang-format on

namespace ip = asio::ip;
using tcp = ip::tcp;

int main() {
  asio::io_context ctx;
  while (true) {
    tcp::acceptor acceptor(ctx, tcp::endpoint(tcp::v4(), 502));
    tcp::socket socket(ctx);
    acceptor.accept(socket);
    size_t size;
    while (true) {
      std::array<uint8_t, 1024> buffer{};
      try {
        size = socket.read_some(asio::buffer(buffer));
      } catch (std::exception& e) {
        break;
      }
      std::cout << "auto data = std::array<uint8_t, " << std::dec << size << "> {";
      for (size_t i = 0; i < size; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(buffer[i]);
        if (i != size - 1)
          std::cout << ", ";
      }
      std::cout << "};" << std::endl;
    }
  }
}