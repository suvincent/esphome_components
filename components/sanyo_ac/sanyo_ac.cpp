#include "sanyo_ac.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sanyo_ac {

static const char *TAG = "sanyo.climate";

void SanyoAC::setup() {
  this->ac = IRSanyoAc(2);
  this->ac_.begin();
  this->ac_.on();
  this->ac_.setMode(kSanyoAcAuto);
  this->ac_.setTemp(25);  // Set a default temperature
  this->ac_.setFan(kSanyoAcFanAuto);
}

climate::ClimateTraits SanyoAC::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supports_current_temperature(true);
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

void SanyoAC::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value()) {
    ClimateMode mode = *call.get_mode();
    switch (mode) {
      case climate::CLIMATE_MODE_OFF:
        this->ac_.off();
        break;
      case climate::CLIMATE_MODE_HEAT_COOL:
        this->ac_.on();
        this->ac_.setMode(kSanyoAcAuto);
        break;
      case climate::CLIMATE_MODE_COOL:
        this->ac_.on();
        this->ac_.setMode(kSanyoAcCool);
        break;
      case climate::CLIMATE_MODE_HEAT:
        this->ac_.on();
        this->ac_.setMode(kSanyoAcHeat);
        break;
      case climate::CLIMATE_MODE_DRY:
        this->ac_.on();
        this->ac_.setMode(kSanyoAcDry);
        break;
      default:
        ESP_LOGE(TAG, "Unsupported climate mode");
    }
    this->mode = mode;
  }

  if (call.get_target_temperature().has_value()) {
    float temp = *call.get_target_temperature();
    this->ac_.setTemp(temp);
    this->target_temperature = temp;
  }

  if (call.get_fan_mode().has_value()) {
    ClimateFanMode fan_mode = *call.get_fan_mode();
    switch (fan_mode) {
      case climate::CLIMATE_FAN_AUTO:
        this->ac_.setFan(kSanyoAcFanAuto);
        break;
      case climate::CLIMATE_FAN_LOW:
        this->ac_.setFan(kSanyoAcFanLow);
        break;
      case climate::CLIMATE_FAN_MEDIUM:
        this->ac_.setFan(kSanyoAcFanMedium);
        break;
      case climate::CLIMATE_FAN_HIGH:
        this->ac_.setFan(kSanyoAcFanHigh);
        break;
      default:
        ESP_LOGE(TAG, "Unsupported fan mode");
    }
    this->fan_mode = fan_mode;
  }

  this->transmit_state_();
  this->publish_state();
}

void SanyoAC::transmit_state_() {
  this->ac_.send();
}

}  // namespace sanyo_ac
}  // namespace esphome