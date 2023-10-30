// Copyright (c) 2023, Skaginn3x (https://skaginn3x.com)

// clang-format off
#include <exception>
#include <asio.hpp>
#include <iostream>
// clang-format on

import modbus;

int main(int argc, char* argv[]) {
  uint16_t port = 1502;
  if (argc != 1) {
    std::cout << "Using provided port: " << argv[1] << std::endl;
    port = std::atoi(argv[1]);
  }
  asio::io_context ctx;

  auto handler = std::make_shared<modbus::default_handler>();
  handler->registers[0] = 0;
  handler->registers[1] = 1;
  handler->registers[2] = 2;
  handler->registers[3] = 3;
  handler->registers[4] = 4;
  handler->registers[5] = 5;
  handler->registers[6] = 6;
  handler->registers[7] = 7;
  handler->registers[8] = 8;
  handler->registers[9] = 9;
  handler->registers[10] = 10;
  handler->registers[11] = 11;
  handler->registers[12] = 12;
  handler->registers[13] = 13;
  handler->registers[14] = 14;

  handler->coils[0] = false;
  handler->coils[1] = true;
  handler->coils[2] = false;
  handler->coils[3] = true;
  handler->coils[4] = false;
  handler->coils[5] = true;
  handler->coils[6] = false;
  handler->coils[7] = true;
  handler->coils[8] = false;
  handler->coils[9] = true;
  handler->coils[10] = false;
  handler->coils[11] = true;
  handler->coils[12] = false;
  handler->coils[13] = true;
  handler->coils[14] = false;

  handler->input_registers[0] = 0;
  handler->input_registers[1] = 1;
  handler->input_registers[2] = 2;
  handler->input_registers[3] = 3;
  handler->input_registers[4] = 4;
  handler->input_registers[5] = 5;
  handler->input_registers[6] = 6;
  handler->input_registers[7] = 7;
  handler->input_registers[8] = 8;
  handler->input_registers[9] = 9;
  handler->input_registers[10] = 10;
  handler->input_registers[11] = 11;
  handler->input_registers[12] = 12;
  handler->input_registers[13] = 13;
  handler->input_registers[14] = 14;

  handler->desc_input[0] = false;
  handler->desc_input[1] = true;
  handler->desc_input[2] = false;
  handler->desc_input[3] = true;
  handler->desc_input[4] = false;
  handler->desc_input[5] = true;
  handler->desc_input[6] = false;
  handler->desc_input[7] = true;
  handler->desc_input[8] = false;
  handler->desc_input[9] = true;
  handler->desc_input[10] = false;
  handler->desc_input[11] = true;
  handler->desc_input[12] = false;
  handler->desc_input[13] = true;
  handler->desc_input[14] = false;

  modbus::server<modbus::default_handler> server{ ctx, handler, port };
  server.start();

  std::cout << "Starting example server!" << std::endl;
  ctx.run();
}