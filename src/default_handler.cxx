module;
#include <cstdint>
#include <vector>
export module modbus:default_handler;
import :packet;
import :error;

// TODO: Create a simpler default handler and write tests for both
namespace modbus {
export struct default_handler {
  default_handler() : registers(0x20000), coils(0x20000), input_registers(0x20000), desc_input(0x20000) {}

  modbus::response::read_coils handle(uint8_t, const modbus::request::read_coils& req, modbus::errc_t&) const {
    modbus::response::read_coils resp{};
    resp.values.insert(resp.values.end(), coils.cbegin() + req.address, coils.cbegin() + req.address + req.count);
    return resp;
  }

  modbus::response::read_discrete_inputs handle(uint8_t,
                                                const modbus::request::read_discrete_inputs& req,
                                                modbus::errc_t&) const {
    modbus::response::read_discrete_inputs resp{};
    resp.values.insert(resp.values.end(), desc_input.cbegin() + req.address, desc_input.cbegin() + req.address + req.count);
    return resp;
  }

  modbus::response::read_holding_registers handle(uint8_t,
                                                  const modbus::request::read_holding_registers& req,
                                                  modbus::errc_t&) const {
    modbus::response::read_holding_registers resp{};
    resp.values.insert(resp.values.end(), registers.cbegin() + req.address, registers.cbegin() + req.address + req.count);
    return resp;
  }

  modbus::response::read_input_registers handle(uint8_t,
                                                const modbus::request::read_input_registers& req,
                                                modbus::errc_t&) const {
    modbus::response::read_input_registers resp;
    resp.values.insert(resp.values.end(), input_registers.cbegin() + req.address,
                       input_registers.cbegin() + req.address + req.count);
    return resp;
  }

  modbus::response::write_single_coil handle(uint8_t, const modbus::request::write_single_coil& req, modbus::errc_t&) {
    modbus::response::write_single_coil resp{};
    coils[req.address] = req.value;
    resp.address = req.address;
    resp.value = req.value;
    return resp;
  }

  modbus::response::write_single_register handle(uint8_t,
                                                 const modbus::request::write_single_register& req,
                                                 modbus::errc_t&) {
    modbus::response::write_single_register resp{};
    registers[req.address] = req.value;
    resp.address = req.address;
    resp.value = req.value;
    return resp;
  }

  modbus::response::write_multiple_coils handle(uint8_t, const modbus::request::write_multiple_coils& req, modbus::errc_t&) {
    modbus::response::write_multiple_coils resp{};
    resp.address = req.address;
    resp.count = 0;
    auto iit = req.values.begin();
    auto oit = coils.begin() + req.address;
    while (iit < req.values.end() && oit < coils.end()) {
      *oit++ = *iit++;
      ++resp.count;
    }
    return resp;
  }

  modbus::response::write_multiple_registers handle(uint8_t,
                                                    const modbus::request::write_multiple_registers& req,
                                                    modbus::errc_t&) {
    modbus::response::write_multiple_registers resp{};
    resp.address = req.address;
    resp.count = 0;
    auto iit = req.values.begin();
    auto oit = registers.begin() + req.address;
    while (iit < req.values.end() && oit < registers.end()) {
      *oit++ = *iit++;
      ++resp.count;
    }
    return resp;
  }

  // TODO: Verify this method
  modbus::response::mask_write_register handle(uint8_t, const modbus::request::mask_write_register& req, modbus::errc_t&) {
    modbus::response::mask_write_register resp{};
    resp.address = req.address;
    resp.and_mask = req.and_mask;
    resp.or_mask = req.or_mask;
    registers[req.address] = (registers[req.address] & req.and_mask) | req.or_mask;
    return resp;
  }

  std::vector<std::uint16_t> registers;
  std::vector<bool> coils;
  std::vector<std::uint16_t> input_registers;
  std::vector<bool> desc_input;
};
}  // namespace modbus