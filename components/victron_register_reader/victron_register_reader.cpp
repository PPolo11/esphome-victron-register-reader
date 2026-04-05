#include "victron_register_reader.h"
#include "esphome/core/log.h"

namespace esphome {
namespace victron_register_reader {

static const char *const TAG = "victron_register_reader";

VictronRegisterReader::VictronRegisterReader(const std::string &mac, uint16_t reg, uint32_t update_interval)
    : PollingComponent(update_interval), mac_address_(mac), register_(reg) {}

void VictronRegisterReader::update() {
  ESP_LOGD(TAG, "Polling Victron device %s register 0x%04X", this->mac_address_.c_str(), this->register_);

  // TODO: BLE read logic here
  // For now, publish NAN to avoid crashes
  this->publish_state(NAN);
}

}  // namespace victron_register_reader
}  // namespace esphome


