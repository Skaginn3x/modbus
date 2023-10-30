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
module;
#include <cstdint>
#include <expected>
#include <variant>
#include <vector>
export module modbus:packet;

import :deserialize;
import :serialize;
import :function;
import :tcp;

export namespace modbus {
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

using responses = std::variant<mask_write_register,
                               read_holding_registers,
                               read_coils,
                               read_discrete_inputs,
                               read_input_registers,
                               write_multiple_coils,
                               write_multiple_registers,
                               write_single_coil,
                               write_single_register>;
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

using requests = std::variant<read_coils,
                              read_discrete_inputs,
                              read_holding_registers,
                              read_input_registers,
                              write_single_coil,
                              write_single_register,
                              write_multiple_coils,
                              write_multiple_registers,
                              mask_write_register>;
}  // namespace request
namespace impl {
[[nodiscard]] auto serialize_response(response::responses const& response_variant) -> std::vector<uint8_t> {
  return std::visit([](auto& response) { return response.serialize(); }, response_variant);
}

[[nodiscard]] auto serialize_request(request::requests const& request_variant) -> std::vector<uint8_t> {
  return std::visit([](auto& request) { return request.serialize(); }, request_variant);
}
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
}  // namespace impl
}  // namespace modbus
