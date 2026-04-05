import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_MAC_ADDRESS, CONF_UPDATE_INTERVAL

victron_ns = cg.esphome_ns.namespace("victron_register_reader")
VictronRegisterReader = victron_ns.class_("VictronRegisterReader", cg.PollingComponent, sensor.Sensor)

CONF_REGISTER = "register"

CONFIG_SCHEMA = sensor.sensor_schema().extend({
    cv.GenerateID(): cv.declare_id(VictronRegisterReader),
    cv.Required(CONF_MAC_ADDRESS): cv.mac_address,
    cv.Required(CONF_REGISTER): cv.hex_uint16_t,
    cv.Optional(CONF_UPDATE_INTERVAL, default="60s"): cv.update_interval,
})

async def to_code(config):
    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_MAC_ADDRESS],
        config[CONF_REGISTER],
        config[CONF_UPDATE_INTERVAL]
    )
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

