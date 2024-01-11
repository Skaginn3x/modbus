#include <array>

#include <boost/ut.hpp>
#include <span>

import modbus;

void print_bytes(std::span<uint8_t> data) {
  for (auto& k : data) {
    std::cout << static_cast<int>(k) << " ";
  }
  std::cout << std::endl;
}

int main() {
  using boost::ut::operator""_test;
  using boost::ut::operator|;
  using boost::ut::expect;

  "serialize_bit_list"_test = []() {
    std::vector<bool> bits = { true, false, true, false, true, false, true, false };

    auto data = modbus::impl::serialize_bit_list(bits);
    auto back = modbus::impl::deserialize_bit_list(data, bits.size());
    expect(back.has_value());
    expect(back.value().size() == bits.size());
    for (size_t i = 0; i < bits.size(); i++)
      expect(back.value()[i] == bits[i]);
  };

  "serialize_bit_request"_test =
      [](const auto& bits) {
        auto data = modbus::impl::serialize_bits_request(bits);
        auto back = modbus::impl::deserialize_bits_request(data);
        expect(back.has_value());
        expect(back.value().size() == bits.size());
        for (size_t i = 0; i < bits.size(); i++)
          expect(back.value()[i] == bits[i]);
      } |
      std::vector{ std::vector<bool>{ true },
                   std::vector<bool>{ true, false },
                   std::vector<bool>{ true, false, true },
                   std::vector<bool>{ true, false, true, false },
                   std::vector<bool>{ true, false, true, false, true },
                   std::vector<bool>{ true, false, true, false, true, false },
                   std::vector<bool>{ true, false, true, false, true, false, true },
                   std::vector<bool>{ true, false, true, false, true, false, true, false },
                   std::vector<bool>{ true, false, true, false, true, false, true, false, true } };

  "serialize_bit_response"_test =
      [](const auto& bits) {
        auto data = modbus::impl::serialize_bits_response(bits);
        auto back = modbus::impl::deserialize_bits_response(data);
        expect(back.has_value());
        expect((back.value().size() + 7) / 8 ==
               (bits.size() + 7) / 8);  // the byte count is encoded in the response so we can't compare the sizes directly
        for (size_t i = 0; i < bits.size(); i++)
          expect(back.value()[i] == bits[i]);
      } |
      std::vector{ std::vector<bool>{ true },
                   std::vector<bool>{ true, false },
                   std::vector<bool>{ true, false, true },
                   std::vector<bool>{ true, false, true, false },
                   std::vector<bool>{ true, false, true, false, true },
                   std::vector<bool>{ true, false, true, false, true, false },
                   std::vector<bool>{ true, false, true, false, true, false, true },
                   std::vector<bool>{ true, false, true, false, true, false, true, false },
                   std::vector<bool>{ true, false, true, false, true, false, true, false, true } };

  "serialize_word_request"_test = []() {
    std::vector<uint16_t> words = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    auto data = modbus::impl::serialize_words_request(words);
    auto back = modbus::impl::deserialize_words_request(data);
    expect(back.has_value());
    expect(back.value().size() == words.size());
    for (size_t i = 0; i < words.size(); i++)
      expect(back.value()[i] == words[i]);
  };

  "serialize_words_response"_test = []() {
    std::vector<uint16_t> words = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    auto data = modbus::impl::serialize_words_response(words);
    auto back = modbus::impl::deserialize_words_response(data);
    expect(back.has_value());
    expect(back.value().size() == words.size());
    for (size_t i = 0; i < words.size(); i++)
      expect(back.value()[i] == words[i]);
  };

  "Modbus tcp header serialize/deserialize"_test = []() {
    using modbus::tcp_mbap;
    tcp_mbap header;
    auto bytes = std::array<uint8_t, tcp_mbap::size>{ 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01 };
    auto hd = tcp_mbap::from_bytes(bytes);

    expect(hd.transaction == 1);
    expect(hd.protocol == 0);
    expect(hd.length == 6);
    expect(hd.unit == 1);

    auto buffer = hd.to_bytes();
    print_bytes(buffer);
    print_bytes(bytes);
    for (size_t i = 0; i < buffer.size(); i++)
      expect(buffer[i] == bytes[i]);
  };

  "serialize request read_coils"_test = []() {
    using req_t = modbus::request::read_coils;
    req_t request{};
    request.address = 14;
    request.count = 55;

    auto data = request.serialize();
    req_t ex_request{};
    auto error = ex_request.deserialize(data);
    expect(!error);
    expect(request.address == ex_request.address) << request.address << " " << ex_request.address;
    expect(request.count == ex_request.count) << request.count << " " << ex_request.count;
  };

  "serialize request read_discrete_inputs"_test = []() {
    using req_t = modbus::request::read_discrete_inputs;
    req_t request{};
    request.address = 14;
    request.count = 55;

    auto data = request.serialize();
    req_t ex_request{};
    auto error = ex_request.deserialize(data);
    expect(!error);
    expect(request.address == ex_request.address) << request.address << " " << ex_request.address;
    expect(request.count == ex_request.count) << request.count << " " << ex_request.count;
  };

  "serialize request read_holding_registers"_test = []() {
    using req_t = modbus::request::read_holding_registers;
    req_t request{};
    request.address = 14;
    request.count = 55;

    auto data = request.serialize();
    req_t ex_request{};
    auto error = ex_request.deserialize(data);
    expect(!error);
    expect(request.address == ex_request.address) << request.address << " " << ex_request.address;
    expect(request.count == ex_request.count) << request.count << " " << ex_request.count;
  };

  "serialize request read_input_registers"_test = []() {
    using req_t = modbus::request::read_input_registers;
    req_t request{};
    request.address = 14;
    request.count = 55;

    auto data = request.serialize();
    req_t ex_request{};
    auto error = ex_request.deserialize(data);
    expect(!error);
    expect(request.address == ex_request.address) << request.address << " " << ex_request.address;
    expect(request.count == ex_request.count) << request.count << " " << ex_request.count;
  };
  "serialize request write_signle_coil"_test = []() {
    using req_t = modbus::request::write_single_coil;
    req_t request{};
    request.address = 14;
    request.value = 55;

    auto data = request.serialize();
    req_t ex_request{};
    auto error = ex_request.deserialize(data);
    expect(!error);
    expect(request.address == ex_request.address) << request.address << " " << ex_request.address;
    expect(request.value == ex_request.value) << request.value << " " << ex_request.value;
  };
  "serialize request write_single_register"_test = []() {
    using req_t = modbus::request::write_single_register;
    req_t request{};
    request.address = 14;
    request.value = 55;

    auto data = request.serialize();
    req_t ex_request{};
    auto error = ex_request.deserialize(data);
    expect(!error);
    expect(request.address == ex_request.address) << request.address << " " << ex_request.address;
    expect(request.value == ex_request.value) << request.value << " " << ex_request.value;
  };
  "serialize request write_multiple_coils"_test = []() {
    using req_t = modbus::request::write_multiple_coils;
    req_t request{};
    request.address = 14;
    request.values = { false, true, false, true, true, false, false };

    auto data = request.serialize();
    req_t ex_request{};
    auto error = ex_request.deserialize(data);
    expect(!error);
    expect(request.address == ex_request.address) << request.address << " " << ex_request.address;
    expect(request.values.size() == ex_request.values.size()) << request.values.size() << " " << ex_request.values.size();
    for (size_t i = 0; i < request.values.size(); i++)
      expect(request.values[i] == ex_request.values[i]) << request.values[i] << " " << ex_request.values[i];
  };
  "serialize request write_multiple_registers"_test = []() {
    using req_t = modbus::request::write_multiple_registers;
    req_t request{};
    request.address = 14;
    request.values = { 1, 2, 3, 4, 5, 6, 7, 9, 9, 10000 };

    auto data = request.serialize();
    req_t ex_request{};
    auto error = ex_request.deserialize(data);
    expect(!error);
    expect(request.address == ex_request.address) << request.address << " " << ex_request.address;
    expect(request.values.size() == ex_request.values.size()) << request.values.size() << " " << ex_request.values.size();
    for (size_t i = 0; i < request.values.size(); i++)
      expect(request.values[i] == ex_request.values[i]) << request.values[i] << " " << ex_request.values[i];
  };
  "serialize request mask_write_register"_test = []() {
    using req_t = modbus::request::mask_write_register;
    req_t request{};
    request.address = 14;
    request.and_mask = 55;
    request.and_mask = 85;

    auto data = request.serialize();
    req_t ex_request{};
    auto error = ex_request.deserialize(data);
    expect(!error);
    expect(request.address == ex_request.address) << request.address << " " << ex_request.address;
    expect(request.and_mask == ex_request.and_mask) << request.and_mask << " " << ex_request.and_mask;
    expect(request.or_mask == ex_request.or_mask) << request.or_mask << " " << ex_request.or_mask;
  };
  "serialize response read_coils"_test = []() {
    using res_t = modbus::response::read_coils;
    res_t response{};
    response.values = { false, true, false, true, true, false, false, true, false };

    auto data = response.serialize();
    res_t ex_response{};
    auto error = ex_response.deserialize(data);
    expect(!error) << error.message();
    auto s1 = (response.values.size() + 7) / 8;
    auto s2 = (ex_response.values.size() + 7) / 8;
    expect(s1 == s2) << s1 << " " << s2;
    for (size_t i = 0; i < response.values.size(); i++) {
      expect(response.values[i] == ex_response.values[i]) << response.values[i] << " " << ex_response.values[i];
    }
  };
  "serialize response read_discrete_inputs"_test = []() {
    using res_t = modbus::response::read_discrete_inputs;
    res_t response{};
    response.values = { false, true, false, true, true, false, false, true, false };

    auto data = response.serialize();
    res_t ex_response{};
    auto error = ex_response.deserialize(data);
    expect(!error) << error.message();
    auto s1 = (response.values.size() + 7) / 8;
    auto s2 = (ex_response.values.size() + 7) / 8;
    expect(s1 == s2) << s1 << " " << s2;
    for (size_t i = 0; i < response.values.size(); i++) {
      expect(response.values[i] == ex_response.values[i]) << response.values[i] << " " << ex_response.values[i];
    }
  };
  "serialize response read_holding_registers"_test = []() {
    using res_t = modbus::response::read_holding_registers;
    res_t response{};
    response.values = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    auto data = response.serialize();
    res_t ex_response{};
    auto error = ex_response.deserialize(data);
    expect(!error) << error.message();
    auto s1 = (response.values.size() + 7) / 8;
    auto s2 = (ex_response.values.size() + 7) / 8;
    expect(s1 == s2) << s1 << " " << s2;
    for (size_t i = 0; i < response.values.size(); i++) {
      expect(response.values[i] == ex_response.values[i]) << response.values[i] << " " << ex_response.values[i];
    }
  };
  "serialize response read_input_registers"_test = []() {
    using res_t = modbus::response::read_input_registers;
    res_t response{};
    response.values = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    auto data = response.serialize();
    res_t ex_response{};
    auto error = ex_response.deserialize(data);
    expect(!error) << error.message();
    auto s1 = (response.values.size() + 7) / 8;
    auto s2 = (ex_response.values.size() + 7) / 8;
    expect(s1 == s2) << s1 << " " << s2;
    for (size_t i = 0; i < response.values.size(); i++) {
      expect(response.values[i] == ex_response.values[i]) << response.values[i] << " " << ex_response.values[i];
    }
  };
  "serialize response write_single_coil"_test = []() {
    using res_t = modbus::response::write_single_coil;
    res_t response{};
    response.address = 15;
    response.value = false;

    auto data = response.serialize();
    res_t ex_response{};
    auto error = ex_response.deserialize(data);
    expect(!error) << error.message();
    expect(response.address == ex_response.address) << response.address << " " << ex_response.address;
    expect(response.value == ex_response.value) << response.value << " " << ex_response.value;
  };
  "serialize response write_single_register"_test = []() {
    using res_t = modbus::response::write_single_register;
    res_t response{};
    response.address = 15;
    response.value = 555;

    auto data = response.serialize();
    res_t ex_response{};
    auto error = ex_response.deserialize(data);
    expect(!error) << error.message();
    expect(response.address == ex_response.address) << response.address << " " << ex_response.address;
    expect(response.value == ex_response.value) << response.value << " " << ex_response.value;
  };
  "serialize response write_multiple_coils"_test = []() {
    using res_t = modbus::response::write_multiple_coils;
    res_t response{};
    response.address = 15;
    response.count = 55;

    auto data = response.serialize();
    res_t ex_response{};
    auto error = ex_response.deserialize(data);
    expect(!error) << error.message();
    expect(response.address == ex_response.address) << response.address << " " << ex_response.address;
    expect(response.count == ex_response.count) << response.count << " " << ex_response.count;
  };
  "serialize response write_multiple_registers"_test = []() {
    using res_t = modbus::response::write_multiple_registers;
    res_t response{};
    response.address = 15;
    response.count = 55;

    auto data = response.serialize();
    res_t ex_response{};
    auto error = ex_response.deserialize(data);
    expect(!error) << error.message();
    expect(response.address == ex_response.address) << response.address << " " << ex_response.address;
    expect(response.count == ex_response.count) << response.count << " " << ex_response.count;
  };
  "serialize response mask_write_register"_test = []() {
    using res_t = modbus::response::mask_write_register;
    res_t response{};
    response.address = 15;
    response.and_mask = 55;
    response.or_mask = 55;

    auto data = response.serialize();
    res_t ex_response{};
    auto error = ex_response.deserialize(data);
    expect(!error) << error.message();
    expect(response.address == ex_response.address) << response.address << " " << ex_response.address;
    expect(response.and_mask == ex_response.and_mask) << response.and_mask << " " << ex_response.and_mask;
    expect(response.or_mask == ex_response.or_mask) << response.or_mask << " " << ex_response.or_mask;
  };
  return 0;
}