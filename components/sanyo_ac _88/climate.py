import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID

sanyo_ac_ns = cg.esphome_ns.namespace('sanyo_ac_88')
SanyoAC = sanyo_ac_ns.class_('SanyoAC88', climate.Climate, cg.Component)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(SanyoAC),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)

    cg.add_library("crankyoldgit/IRremoteESP8266", "2.8.4")