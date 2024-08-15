#include "sanyo_ac_88.h"
#include "esphome/core/log.h"

const uint16_t kIrLed = 4; // GPIO 4 = D2

namespace esphome {
namespace sanyo_ac_88 {

static const char *TAG = "sanyo.climate";

void SanyoAC88::setup() {
  this->ac = IRSanyoAc88(kIrLed, false, false);
  this->ac.begin();
  this->ac.on();
  this->ac.setMode(kSanyoAcAuto);
  this->ac.setTemp(25);  // Set a default temperature
  this->ac.setFan(kSanyoAcFanAuto);
  this->mode = mode;
  this->target_temperature = 25;
  this->publish_state();
}

climate::ClimateTraits SanyoAC88::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(false);
  traits.set_supports_two_point_target_temperature(false);
  traits.set_visual_min_temperature(16);
  traits.set_visual_max_temperature(30);
  traits.set_visual_temperature_step(1);

  traits.set_supported_modes({
    climate::CLIMATE_MODE_OFF,
    climate::CLIMATE_MODE_HEAT_COOL,
    climate::CLIMATE_MODE_COOL,
    climate::CLIMATE_MODE_HEAT,
    climate::CLIMATE_MODE_DRY,
  });

  traits.set_supported_fan_modes({
    climate::CLIMATE_FAN_AUTO,
    climate::CLIMATE_FAN_LOW,
    climate::CLIMATE_FAN_MEDIUM,
    climate::CLIMATE_FAN_HIGH,
  });

  return traits;
}

void SanyoAC88::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value()) {
    esphome::climate::ClimateMode mode = *call.get_mode();
    switch (mode) {
      case climate::CLIMATE_MODE_OFF:
        this->ac.off();
        break;
      case climate::CLIMATE_MODE_HEAT_COOL:
        this->ac.on();
        this->ac.setMode(kSanyoAcAuto);
        break;
      case climate::CLIMATE_MODE_COOL:
        this->ac.on();
        this->ac.setMode(kSanyoAcCool);
        break;
      case climate::CLIMATE_MODE_HEAT:
        this->ac.on();
        this->ac.setMode(kSanyoAcHeat);
        break;
      case climate::CLIMATE_MODE_DRY:
        this->ac.on();
        this->ac.setMode(kSanyoAcDry);
        break;
      default:
        ESP_LOGE(TAG, "Unsupported climate mode");
    }
    this->mode = mode;
  }

  if (call.get_target_temperature().has_value()) {
    float temp = *call.get_target_temperature();
    this->ac.setTemp(temp);
    this->target_temperature = temp;
  }

  if (call.get_fan_mode().has_value()) {
    esphome::climate::ClimateFanMode fan_mode = *call.get_fan_mode();
    switch (fan_mode) {
      case climate::CLIMATE_FAN_AUTO:
        this->ac.setFan(kSanyoAcFanAuto);
        break;
      case climate::CLIMATE_FAN_LOW:
        this->ac.setFan(kSanyoAcFanLow);
        break;
      case climate::CLIMATE_FAN_MEDIUM:
        this->ac.setFan(kSanyoAcFanMedium);
        break;
      case climate::CLIMATE_FAN_HIGH:
        this->ac.setFan(kSanyoAcFanHigh);
        break;
      default:
        ESP_LOGE(TAG, "Unsupported fan mode");
    }
    this->fan_mode = fan_mode;
  }

  this->ac.send(1);
  this->publish_state();
}

}  // namespace sanyo_ac
}  // namespace esphome