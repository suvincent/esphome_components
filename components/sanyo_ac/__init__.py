from esphome.components import climate
from esphome.const import CONF_ID
from esphome import automation

AUTO_LOAD = ["climate"]

CONF_SANYO_AC_ID = "sanyo_ac_id"

SanyoAc = climate.climate_ns.class_("SanyoAC", climate.Climate, automation.Trigger.template())

def validate_sanyo_ac(config):
    if CONF_SANYO_AC_ID in config:
        var = yield automation.build_automation(SanyoAc, [(SanyoAc, CONF_SANYO_AC_ID)])
        yield var
    yield config

def to_code(config):
    var = yield climate.register_climate(config)
    yield automation.setup_automation(var)