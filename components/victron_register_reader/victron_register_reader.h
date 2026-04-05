#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/victron_ble/victron_ble.h"
#include <string>

namespace esphome {
namespace victron_register_reader {

class VictronRegisterReader : public PollingComponent,
                              public sensor::Sensor,
                              public victron_ble::VictronBLEListener {
 public:
  VictronRegisterReader(const std::string &mac, uint16_t reg, uint32_t update_interval);

  void update() override;

  // BLE Callback
  void on_solar_charger_data(const victron_ble::SolarChargerData &data) override;

 protected:
  std::string mac_address_;
  uint16_t register_;
  float last_value_ = NAN;
};

}  // namespace victron_register_reader
}  // namespace esphome
