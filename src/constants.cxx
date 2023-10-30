module;
#include <cstddef>
export module modbus:constants;

namespace modbus {
// Because the modbus protocol was first
// implemented for RS485 the max pdu size is 253
// See Modbus Application protocol specification V1.1b3 page 5
static constexpr size_t modbus_max_pdu = 253;
};  // namespace modbus
