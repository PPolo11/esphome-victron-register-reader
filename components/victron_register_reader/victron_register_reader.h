#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include <string>

namespace esphome {
namespace victron_register_reader {

class VictronRegisterReader : public PollingComponent, public sensor::Sensor {
 public:
  VictronRegisterReader(const std::string &mac, uint16_t reg, uint32_t update_interval);

  void update() override;

 protected:
  std::string mac_address_;
  uint16_t register_;
};

}  // namespace victron_register_reader
}  // namespace esphome

