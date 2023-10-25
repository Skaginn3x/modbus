// Copyright (c) 2023, Skaginn3x (https://skaginn3x.com)

#pragma once

#include <expected>
#include <ranges>
#include <span>
#include <system_error>

#include <modbus/impl/deserialize_base.hpp>
#include <modbus/request.hpp>

namespace modbus::impl {
auto request_from_function(function_e func) -> std::expected<request::requests, std::error_code> {
  switch (func) {
    case function_e::read_discrete_inputs:
      return request::read_discrete_inputs{};
    case function_e::read_coils:
      return request::read_coils{};
    case function_e::read_holding_registers:
      return request::read_holding_registers{};
    case function_e::read_input_registers:
      return request::read_input_registers{};
    case function_e::write_single_coil:
      return request::write_single_coil{};
    case function_e::write_single_register:
      return request::write_single_register{};
    case function_e::write_multiple_coils:
      return request::write_multiple_coils{};
    case function_e::write_multiple_registers:
      return request::write_multiple_registers{};
    case function_e::mask_write_register:
      return request::mask_write_register{};
    case function_e::read_exception_status:
    case function_e::diagnostic:
    case function_e::get_com_event_log:
    case function_e::get_com_event_counter:
    case function_e::report_server_id:
    case function_e::read_file_record:
    case function_e::write_file_record:
    case function_e::read_write_multiple_registers:
    case function_e::read_fifo_record:
    default:
      return std::unexpected(modbus_error(errc_t::illegal_function));
  }
}

/// Deserialize request. Expect a function code.
[[nodiscard]] auto deserialize_request(std::ranges::range auto data, function_e const expected_function)
    -> std::expected<request::requests, std::error_code> {
  // Deserialize the function
  auto expect_function = deserialize_function(std::span(data).subspan(0), expected_function);
  if (!expect_function) {
    return std::unexpected(expect_function.error());
  }
  auto function = expect_function.value();

  // Fetch a request instance from the function code.
  auto expect_request = request_from_function(function);
  if (!expect_request) {
    return std::unexpected(expect_request.error());
  }
  auto deserialize_error = std::visit([&](auto& request) { return request.deserialize(data); }, expect_request.value());
  if (deserialize_error) {
    return std::unexpected(deserialize_error);
  }
  return expect_request.value();
}

}  // namespace modbus::impl
