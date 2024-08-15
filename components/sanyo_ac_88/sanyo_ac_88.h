#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/core/helpers.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Sanyo.h>



namespace esphome {
namespace sanyo_ac_88 {
class SanyoAC88 : public climate::Climate, public Component {
 public:
  void setup() override;
  void control(const climate::ClimateCall &call) override;
  climate::ClimateTraits traits() override;
  IRSanyoAc88 ac;
};

}  // namespace sanyo_ac_88
}  // namespace esphome