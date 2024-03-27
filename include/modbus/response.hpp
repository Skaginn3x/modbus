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
#include <modbus/impl/serialize_base.hpp>

namespace modbus {
namespace request {
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
}  // namespace request

namespace response {

/// Message representing a read_coils response.
struct read_coils {
  /// Request type.
  using request = request::read_coils;

  /// The function code.
  static constexpr function_e function = function_e::read_coils;

  /// The read values.
  std::vector<bool> values;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] auto length() const -> std::size_t { return 2 + (values.size() + 7) / 8; }

  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    auto ex_values = impl::deserialize_bits_response(std::span(data).subspan(1));
    if (!ex_values) {
      return ex_values.error();
    }
    values = ex_values.value();
    return {};
  }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;
    ret_value.emplace_back(impl::serialize_function(function));
    auto bit_response = impl::serialize_bits_response(values);
    ret_value.insert(ret_value.end(), bit_response.begin(), bit_response.end());
    return ret_value;
  }
};

/// Message representing a read_discrete_inputs response.
struct read_discrete_inputs {
  /// Request type.
  using request = request::read_discrete_inputs;

  /// The function code.
  static constexpr function_e function = function_e::read_discrete_inputs;

  /// The read values.
  std::vector<bool> values;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] auto length() const -> std::size_t { return 2 + (values.size() + 7) / 8; }

  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    auto ex_values = impl::deserialize_bits_response(std::span(data).subspan(1));
    if (!ex_values) {
      return ex_values.error();
    }
    values = ex_values.value();
    return {};
  }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;
    ret_value.emplace_back(impl::serialize_function(function));
    auto bit_response = impl::serialize_bits_response(values);
    ret_value.insert(ret_value.end(), bit_response.begin(), bit_response.end());
    return ret_value;
  }
};

/// Message representing a read_holding_registers response.
struct read_holding_registers {
  /// Request type.
  using request = request::read_holding_registers;

  /// The function code.
  static constexpr function_e function = function_e::read_holding_registers;

  /// The read values.
  std::vector<std::uint16_t> values;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] auto length() const -> std::size_t { return 2 + values.size() * 2; }

  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    auto ex_values = impl::deserialize_words_response(std::span(data).subspan(1));
    if (!ex_values) {
      return ex_values.error();
    }
    values = ex_values.value();
    return {};
  }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;
    ret_value.emplace_back(impl::serialize_function(function));
    auto word_response = impl::serialize_words_response(values);
    ret_value.insert(ret_value.end(), word_response.begin(), word_response.end());
    return ret_value;
  }
};

/// Message representing a read_input_registers response.
struct read_input_registers {
  /// Request type.
  using request = request::read_input_registers;

  /// The function code.
  static constexpr function_e function = function_e::read_input_registers;

  /// The read values.
  std::vector<std::uint16_t> values;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] auto length() const -> std::size_t { return 2 + values.size() * 2; }

  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    auto ex_values = impl::deserialize_words_response(std::span(data).subspan(1));
    if (!ex_values) {
      return ex_values.error();
    }
    values = ex_values.value();
    return {};
  }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;
    ret_value.emplace_back(impl::serialize_function(function));
    auto word_response = impl::serialize_words_response(values);
    ret_value.insert(ret_value.end(), word_response.begin(), word_response.end());
    return ret_value;
  }
};

/// Message representing a write_single_coil response.
struct write_single_coil {
  /// Request type.
  using request = request::write_single_coil;

  /// The function code.
  static constexpr function_e function = function_e::write_single_coil;

  /// The address of the coil written to.
  std::uint16_t address;

  /// The value written to the coil.
  bool value;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 5; }

  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    address = impl::deserialize_be16(std::span(data).subspan(1));

    auto ex_value = impl::deserialize_bool(std::span(data).subspan(3));
    if (!ex_value) {
      return ex_value.error();
    }
    value = ex_value.value();
    return {};
  }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;
    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());
    auto arr_value = impl::serialize_16_array(impl::serialize_be16(impl::bool_to_uint16(value)));
    ret_value.insert(ret_value.end(), arr_value.begin(), arr_value.end());
    return ret_value;
  }
};

/// Message representing a write_single_register response.
struct write_single_register {
  /// Request type.
  using request = request::write_single_register;

  /// The function code.
  static constexpr function_e function = function_e::write_single_register;

  /// The address of the register written to.
  std::uint16_t address;

  /// The value written to the register.
  std::uint16_t value;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 5; }

  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    address = impl::deserialize_be16(std::span(data).subspan(1));
    value = impl::deserialize_be16(std::span(data).subspan(3));
    return {};
  }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;
    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());
    auto arr_value = impl::serialize_16_array(impl::serialize_be16(value));
    ret_value.insert(ret_value.end(), arr_value.begin(), arr_value.end());
    return ret_value;
  }
};

/// Message representing a write_multiple_coil response.
struct write_multiple_coils {
  /// Request type.
  using request = request::write_multiple_coils;

  /// The function code.
  static constexpr function_e function = function_e::write_multiple_coils;

  /// The address of the first coil written to.
  std::uint16_t address;

  /// The number of coils written to.
  std::uint16_t count;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 5; }

  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    address = impl::deserialize_be16(std::span(data).subspan(1));
    count = impl::deserialize_be16(std::span(data).subspan(3));
    return {};
  }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;
    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());
    auto arr_count = impl::serialize_16_array(impl::serialize_be16(count));
    ret_value.insert(ret_value.end(), arr_count.begin(), arr_count.end());
    return ret_value;
  }
};

/// Message representing a write_multiple_registers response.
struct write_multiple_registers {
  /// Request type.
  using request = request::write_multiple_registers;

  /// The function code.
  static constexpr function_e function = function_e::write_multiple_registers;

  /// The address of the first register written to.
  std::uint16_t address;

  /// The number of registers written to.
  std::uint16_t count;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 5; }

  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    address = impl::deserialize_be16(std::span(data).subspan(1));
    count = impl::deserialize_be16(std::span(data).subspan(3));
    return {};
  }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;
    ret_value.emplace_back(impl::serialize_function(function));
    auto arr_address = impl::serialize_16_array(impl::serialize_be16(address));
    ret_value.insert(ret_value.end(), arr_address.begin(), arr_address.end());
    auto arr_count = impl::serialize_16_array(impl::serialize_be16(count));
    ret_value.insert(ret_value.end(), arr_count.begin(), arr_count.end());
    return ret_value;
  }
};

/// Message representing a mask_write_register response.
struct mask_write_register {
  /// Request type.
  using request = request::mask_write_register;

  /// The function code.
  static constexpr function_e function = function_e::mask_write_register;

  /// The address of the register written to.
  std::uint16_t address;

  /// The AND mask used.
  std::uint16_t and_mask;

  /// The OR mask used.
  std::uint16_t or_mask;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] static auto length() -> std::size_t { return 7; }

  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    address = impl::deserialize_be16(std::span(data).subspan(1));
    and_mask = impl::deserialize_be16(std::span(data).subspan(3));
    or_mask = impl::deserialize_be16(std::span(data).subspan(5));
    return {};
  }

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
};

/// Message representing a read_write_multiple_registers response.
struct read_write_multiple_registers {
  /// Request type.
  using request = request::read_write_multiple_registers;

  /// The function code.
  static constexpr function_e function = function_e::read_write_multiple_registers;

  /// The read values.
  std::vector<std::uint16_t> values;

  /// The length of the serialized ADU in bytes.
  [[nodiscard]] auto length() const -> std::size_t { return 2 + values.size() * 2; }

  [[nodiscard]] auto deserialize(std::ranges::range auto data) -> std::error_code {
    auto ex_values = impl::deserialize_words_response(std::span(data).subspan(1));
    if (!ex_values) {
      return ex_values.error();
    }
    values = ex_values.value();
    return {};
  }

  [[nodiscard]] auto serialize() const -> std::vector<uint8_t> {
    std::vector<uint8_t> ret_value;
    ret_value.emplace_back(impl::serialize_function(function));
    auto word_response = impl::serialize_words_response(values);
    ret_value.insert(ret_value.end(), word_response.begin(), word_response.end());
    return ret_value;
  }
};

using responses = std::variant<mask_write_register,
                               read_holding_registers,
                               read_coils,
                               read_discrete_inputs,
                               read_input_registers,
                               write_multiple_coils,
                               write_multiple_registers,
                               write_single_coil,
                               write_single_register,
                               read_write_multiple_registers>;
}  // namespace response
}  // namespace modbus
