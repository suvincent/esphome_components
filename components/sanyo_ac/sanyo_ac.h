#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/core/helpers.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Sanyo.h>



namespace esphome {
namespace sanyo_ac {
const uint16_t kIrLed = 4; // GPIO 4 = D2
IRSanyoAc ac(kIrLed, false, false);
class SanyoAC : public climate::Climate, public Component {
 public:
  void setup() override;
  void control(const climate::ClimateCall &call) override;
  climate::ClimateTraits traits() override;
 protected:
  void transmit_state_();
};

}  // namespace sanyo_ac
}  // namespace esphome