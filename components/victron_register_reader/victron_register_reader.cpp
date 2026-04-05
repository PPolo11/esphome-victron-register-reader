#include "victron_register_reader.h"
#include "esphome/core/log.h"

namespace esphome {
namespace victron_register_reader {

static const char *const TAG = "victron_register_reader";

VictronRegisterReader::VictronRegisterReader(const std::string &mac, uint16_t reg, uint32_t update_interval)
    : PollingComponent(update_interval), mac_address_(mac), register_(reg) {}

void VictronRegisterReader::on_solar_charger_data(const victron_ble::SolarChargerData &data) {
  if (data.mac_address != this->mac_address_) {
    return;  // falsches Gerät
  }

  switch (this->register_) {
    case 0xEDF7:  // Absorption Voltage
      this->last_value_ = data.absorption_voltage;
      break;

    case 0xEDF6:  // Float Voltage
      this->last_value_ = data.float_voltage;
      break;

    case 0xEDF0:  // Max Charge Current
      this->last_value_ = data.max_charge_current;
      break;

    case 0xEDF1:  // Battery Type
      this->last_value_ = data.battery_type;
      break;

    case 0x2015:  // Charge Current Limit
      this->last_value_ = data.charge_current_limit;
      break;

    default:
      ESP_LOGW(TAG, "Unknown register 0x%04X", this->register_);
      break;
  }
}

void VictronRegisterReader::update() {
  if (std::isnan(this->last_value_)) {
    ESP_LOGW(TAG, "No BLE data received yet for %s", this->mac_address_.c_str());
    return;
  }

  ESP_LOGD(TAG, "Publishing register 0x%04X = %.2f", this->register_, this->last_value_);
  this->publish_state(this->last_value_);
}
}  // namespace victron_register_reader
}  // namespace esphome


