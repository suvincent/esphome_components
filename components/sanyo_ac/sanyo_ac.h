#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/core/helpers.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Sanyo.h>

const uint16_t kIrLed = 4; // GPIO 4 = D2
IRSanyoAc ac(kIrLed, false, false);

namespace esphome {
namespace sanyo_ac {

class SanyoAC : public climate::Climate, public Component {
 public:
  SanyoAC(){
    this->ac = ac;
  }
  void setup() override;
  void control(const climate::ClimateCall &call) override;
  climate::ClimateTraits traits() override;
 protected:
  IRSanyoAc ac;  // The pin will be set afterward
  void transmit_state_();
};

}  // namespace sanyo_ac
}  // namespace esphome