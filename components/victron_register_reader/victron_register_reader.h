 #pragma once

#include "esphome.h"
#include "BLEDevice.h"

class VictronRegisterReader : public esphome::PollingComponent, public esphome::sensor::Sensor {
 public:
  std::string mac_address;
  uint16_t reg;

  VictronRegisterReader(std::string mac, uint16_t reg, uint32_t interval)
      : PollingComponent(interval), mac_address(mac), reg(reg) {}

  void setup() override {
    BLEDevice::init("");
  }

  void update() override {
    ESP_LOGI("victron_reg", "Reading register 0x%04X from %s", reg, mac_address.c_str());

    BLEAddress addr(mac_address);
    auto client = BLEDevice::createClient();

    if (!client->connect(addr)) {
      ESP_LOGE("victron_reg", "Failed to connect");
      return;
    }

    auto service = client->getService(BLEUUID("000018f0-0000-1000-8000-00805f9b34fb"));
    if (!service) {
      ESP_LOGE("victron_reg", "Service not found");
      client->disconnect();
      return;
    }

    auto ch = service->getCharacteristic(BLEUUID("00002af1-0000-1000-8000-00805f9b34fb"));
    if (!ch) {
      ESP_LOGE("victron_reg", "Characteristic not found");
      client->disconnect();
      return;
    }

    uint8_t reg_bytes[2];
    reg_bytes[0] = reg & 0xFF;
    reg_bytes[1] = (reg >> 8) & 0xFF;

    ch->writeValue(reg_bytes, 2, false);

    std::string value = ch->readValue();
    client->disconnect();

    if (value.length() < 2) {
      ESP_LOGE("victron_reg", "Invalid response");
      return;
    }

    uint16_t raw = (uint8_t)value[0] | ((uint8_t)value[1] << 8);

    ESP_LOGI("victron_reg", "Register 0x%04X = %u", reg, raw);

    publish_state(raw);
  }
};


