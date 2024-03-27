// Copyright (c) 2017, Fizyr (https://fizyr.com)
// Copyright (c) 2023, Skaginn3x (https://skaginn3x.com)
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holder(s) nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <cstdint>
#include <variant>
#include <vector>

#include <modbus/functions.hpp>
#include <modbus/impl/deserialize_base.hpp>
#include <modbus/impl/serialize_base.hpp>

namespace modbus {

namespace response {
struct read_coils;
struct read_discrete_inputs;
struct read_holding_registers;
struct read_input_registers;
struct write_single_coil;
struct write_single_register;
struct write_multiple_coils;
struct write_multiple_registers;
struct mask_write_register;
struct read_write_multiple_registers;
}  // namespace response

namespace request {

/// Message representing a read_coils request.
struct read_coils {
  /// Response type.
  using response = response::read_coils;

  /// The function code.
  static constexpr function_e function = function_e::read_coils;

  /// The address of the first coil/register to read from.
  std::uint16_t address;

  /// The number of registers/coils to read.
  std::uint16_t count;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 5; }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;

    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());

    auto arr_count = impl::serialize_16_array(impl::serialize_be16(count));
    ret_value.insert(ret_value.end(), arr_count.begin(), arr_count.end());

    return ret_value;
  }

  /// Deserialize request.
  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    if (auto error = impl::check_length(data.size(), length())) {
      return error;
    }
    address = impl::deserialize_be16(std::span(data).subspan(1, 2));
    count = impl::deserialize_be16(std::span(data).subspan(3, 2));
    return {};
  }
};

/// Message representing a read_discrete_inputs request.
struct read_discrete_inputs {
  /// Response type.
  using response = response::read_discrete_inputs;

  /// The function code.
  static constexpr function_e function = function_e::read_discrete_inputs;

  /// The address of the first coil/register to read from.
  std::uint16_t address;

  /// The number of registers/coils to read.
  std::uint16_t count;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 5; }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;

    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());

    auto arr_count = impl::serialize_16_array(impl::serialize_be16(count));
    ret_value.insert(ret_value.end(), arr_count.begin(), arr_count.end());

    return ret_value;
  }

  /// Deserialize request.
  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    if (auto error = impl::check_length(data.size(), length())) {
      return error;
    }
    address = impl::deserialize_be16(std::span(data).subspan(1, 2));
    count = impl::deserialize_be16(std::span(data).subspan(3, 2));
    return {};
  }
};

/// Message representing a read_holding_registers request.
struct read_holding_registers {
  /// Response type.
  using response = response::read_holding_registers;

  /// The function code.
  static constexpr function_e function = function_e::read_holding_registers;

  /// The address of the first coil/register to read from.
  std::uint16_t address;

  /// The number of registers/coils to read.
  std::uint16_t count;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 5; }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;

    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());

    auto arr_count = impl::serialize_16_array(impl::serialize_be16(count));
    ret_value.insert(ret_value.end(), arr_count.begin(), arr_count.end());

    return ret_value;
  }

  /// Deserialize request.
  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    if (auto error = impl::check_length(data.size(), length())) {
      return error;
    }
    address = impl::deserialize_be16(std::span(data).subspan(1, 2));
    count = impl::deserialize_be16(std::span(data).subspan(3, 2));
    return {};
  }
};

/// Message representing a read_input_registers request.
struct read_input_registers {
  /// Response type.
  using response = response::read_input_registers;

  /// The function code.
  static constexpr function_e function = function_e::read_input_registers;

  /// The address of the first coil/register to read from.
  std::uint16_t address;

  /// The number of registers/coils to read.
  std::uint16_t count;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 5; }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;

    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());

    auto arr_count = impl::serialize_16_array(impl::serialize_be16(count));
    ret_value.insert(ret_value.end(), arr_count.begin(), arr_count.end());

    return ret_value;
  }

  /// Deserialize request.
  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    if (auto error = impl::check_length(data.size(), length())) {
      return error;
    }
    address = impl::deserialize_be16(std::span(data).subspan(1, 2));
    count = impl::deserialize_be16(std::span(data).subspan(3, 2));
    return {};
  }
};

/// Message representing a write_single_coil request.
struct write_single_coil {
  /// Response type.
  using response = response::write_single_coil;

  /// The function code.
  static constexpr function_e function = function_e::write_single_coil;

  /// The address of the coil to write to.
  std::uint16_t address;

  /// The value to write.
  bool value;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 5; }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;

    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());

    auto arr_value = impl::serialize_16_array(impl::serialize_be16(impl::bool_to_uint16(value)));
    ret_value.insert(ret_value.end(), arr_value.begin(), arr_value.end());

    return ret_value;
  }

  /// Deserialize request.
  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    if (auto error = impl::check_length(data.size(), length())) {
      return error;
    }
    address = impl::deserialize_be16(std::span(data).subspan(1, 2));
    auto value_expected = impl::deserialize_bool(std::span(data).subspan(3, 2));
    if (!value_expected) {
      return value_expected.error();
    }
    value = value_expected.value();
    return {};
  }
};

/// Message representing a write_single_register request.
struct write_single_register {
  /// Response type.
  using response = response::write_single_register;

  /// The function code.
  static constexpr function_e function = function_e::write_single_register;

  /// The address of the register to write to.
  std::uint16_t address;

  /// The value to write.
  std::uint16_t value;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 5; }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;

    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());

    auto arr_value = impl::serialize_16_array(impl::serialize_be16(value));
    ret_value.insert(ret_value.end(), arr_value.begin(), arr_value.end());

    return ret_value;
  }

  /// Deserialize request.
  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    if (auto error = impl::check_length(data.size(), length())) {
      return error;
    }
    address = impl::deserialize_be16(std::span(data).subspan(1, 2));
    value = impl::deserialize_be16(std::span(data).subspan(3, 2));
    return {};
  }
};

/// Message representing a write_multiple_coils request.
struct write_multiple_coils {
  /// Response type.
  using response = response::write_multiple_coils;

  /// The function code.
  static constexpr function_e function = function_e::write_multiple_coils;

  /// The address of the first coil to write to.
  std::uint16_t address;

  /// The values to write.
  std::vector<bool> values;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] auto length() const -> std::size_t { return 6 + (values.size() + 7) / 8; }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;

    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());

    auto arr_values = impl::serialize_bits_request(values);
    ret_value.insert(ret_value.end(), arr_values.begin(), arr_values.end());

    return ret_value;
  }

  /// Deserialize request.
  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    if (auto error = impl::check_length(data.size(), 3)) {
      return error;
    }
    address = impl::deserialize_be16(std::span(data).subspan(1));
    auto expected = impl::deserialize_bits_request(std::span(data).subspan(3));
    if (!expected) {
      return expected.error();
    }
    values = expected.value();
    return {};
  }
};

/// Message representing a write_multiple_registers request.
struct write_multiple_registers {
  /// Response type.
  using response = response::write_multiple_registers;

  /// The function code.
  static constexpr function_e function = function_e::write_multiple_registers;

  /// The address of the first register to write to.
  std::uint16_t address;

  /// The values to write.
  std::vector<std::uint16_t> values;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] auto length() const -> std::size_t { return 6 + values.size() * 2; }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;

    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());

    auto arr_values = impl::serialize_words_request(values);
    ret_value.insert(ret_value.end(), arr_values.begin(), arr_values.end());

    return ret_value;
  }

  /// Deserialize request.
  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    if (auto error = impl::check_length(data.size(), 3)) {
      return error;
    }
    address = impl::deserialize_be16(std::span(data).subspan(1));
    auto expected = impl::deserialize_words_request(std::span(data).subspan(3));
    if (!expected) {
      return expected.error();
    }
    values = expected.value();
    return {};
  }
};

/// Message representing a mask_write_register request.
struct mask_write_register {
  /// Response type.
  using response = response::mask_write_register;

  /// The function code.
  static constexpr function_e function = function_e::mask_write_register;

  /// The address of the register to write to.
  std::uint16_t address;

  /// The mask to AND the register value with.
  std::uint16_t and_mask;

  /// The mask to OR the register value with.
  std::uint16_t or_mask;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 7; }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;

    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());

    auto arr_and_mask = impl::serialize_16_array(impl::serialize_be16(and_mask));
    ret_value.insert(ret_value.end(), arr_and_mask.begin(), arr_and_mask.end());

    auto arr_or_mask = impl::serialize_16_array(impl::serialize_be16(or_mask));
    ret_value.insert(ret_value.end(), arr_or_mask.begin(), arr_or_mask.end());

    return ret_value;
  }

  /// Deserialize request.
  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    if (auto error = impl::check_length(data.size(), length())) {
      return error;
    }
    address = impl::deserialize_be16(std::span(data).subspan(1, 2));
    and_mask = impl::deserialize_be16(std::span(data).subspan(3, 2));
    or_mask = impl::deserialize_be16(std::span(data).subspan(5, 2));
    return {};
  }
};

/// Message representing a read_write_multiple_registers request.
struct read_write_multiple_registers {
  /// Response type.
  using response = response::read_write_multiple_registers;

  /// The function code.
  static constexpr function_e function = function_e::read_write_multiple_registers;

  /// The address of the register to read from
  std::uint16_t read_address;

  /// The amount of words to read
  std::uint16_t read_count;

  /// The address of the register to read from
  std::uint16_t write_address;

  /// The values to write.
  std::vector<std::uint16_t> values;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] auto length() const -> std::size_t { return 10 + values.size() * 2; }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;
    ret_value.emplace_back(impl::serialize_function(function));

    auto read_address_arr = impl::serialize_16_array(impl::serialize_be16(read_address));
    ret_value.insert(ret_value.end(), read_address_arr.begin(), read_address_arr.end());

    auto read_count_arr = impl::serialize_16_array(impl::serialize_be16(read_count));
    ret_value.insert(ret_value.end(), read_count_arr.begin(), read_count_arr.end());

    auto write_address_arr = impl::serialize_16_array(impl::serialize_be16(write_address));
    ret_value.insert(ret_value.end(), write_address_arr.begin(), write_address_arr.end());

    auto arr_values = impl::serialize_words_request(values);
    ret_value.insert(ret_value.end(), arr_values.begin(), arr_values.end());

    return ret_value;
  }

  /// Deserialize request.
  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    if (auto error = impl::check_length(data.size(), length())) {
      return error;
    }
    read_address = impl::deserialize_be16(std::span(data).subspan(1, 2));
    read_count = impl::deserialize_be16(std::span(data).subspan(3, 2));
    write_address = impl::deserialize_be16(std::span(data).subspan(5, 2));
    auto expected = impl::deserialize_words_request(std::span(data).subspan(7));
    if (!expected) {
      return expected.error();
    }
    values = expected.value();
    return {};
  }
};

using requests = std::variant<read_coils,
                              read_discrete_inputs,
                              read_holding_registers,
                              read_input_registers,
                              write_single_coil,
                              write_single_register,
                              write_multiple_coils,
                              write_multiple_registers,
                              mask_write_register,
                              read_write_multiple_registers>;
}  // namespace request
}  // namespace modbus
