// Copyright (c) 2023, Skaginn3x (https://skaginn3x.com)

#pragma once

#include <expected>
#include <ranges>
#include <span>

#include <modbus/impl/deserialize_base.hpp>
#include <modbus/response.hpp>

namespace modbus::impl {
auto response_from_function(function_e func) -> std::expected<response::responses, std::error_code> {
  switch (func) {
    case function_e::read_discrete_inputs:
      return response::read_discrete_inputs{};
    case function_e::read_coils:
      return response::read_coils{};
    case function_e::read_holding_registers:
      return response::read_holding_registers{};
    case function_e::read_input_registers:
      return response::read_input_registers{};
    case function_e::write_single_coil:
      return response::write_single_coil{};
    case function_e::write_single_register:
      return response::write_single_register{};
    case function_e::write_multiple_coils:
      return response::write_multiple_coils{};
    case function_e::write_multiple_registers:
      return response::write_multiple_registers{};
    case function_e::mask_write_register:
      return response::mask_write_register{};
    case function_e::read_write_multiple_registers:
      return response::read_write_multiple_registers{};
    case function_e::read_exception_status:
    case function_e::diagnostic:
    case function_e::get_com_event_log:
    case function_e::get_com_event_counter:
    case function_e::report_server_id:
    case function_e::read_file_record:
    case function_e::write_file_record:
    case function_e::read_fifo_record:
    default:
      return std::unexpected(modbus_error(errc_t::illegal_function));
  }
}

/// Deserialize response. Expect a function code.
[[nodiscard]] auto deserialize_response(std::ranges::range auto data, function_e const expected_function)
    -> std::expected<response::responses, std::error_code> {
  // Deserialize the function
  auto expect_function = deserialize_function(std::span(data).subspan(0), expected_function);
  if (!expect_function) {
    return std::unexpected(expect_function.error());
  }
  auto function = expect_function.value();

  // Fetch a response instance from the function code.
  auto expect_response = response_from_function(function);
  if (!expect_response) {
    return std::unexpected(expect_response.error());
  }
  auto deserialize_error = std::visit([&](auto& response) { return response.deserialize(data); }, expect_response.value());
  if (deserialize_error) {
    return std::unexpected(deserialize_error);
  }
  return expect_response.value();
}
}  // namespace modbus::impl
