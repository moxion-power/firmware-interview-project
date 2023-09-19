#pragma once

#include "Drivers.h"
/**
 * @brief Example FanController base
 */

namespace FanControllerTypes {

enum state {
  ENABLE_SENSOR_POWER = 0,
  TEMPERATURE_SENSOR, // we get the temperature reading
  FAN_CONTROL_ENABLE, // enables the temperature sensor
  FAN_CONTROL,        // all pwm happens here
};
}; // namespace FanController
class FanController_i {

  /*
   * @brief One time executed initialization method
   */
  void init(){};
  /**
   * @brief Periodic loop method that will be called every 500ms
   */
  void loop();

private:
  double private_get_fan_duty_cycle();
  double get_current_temperature();
  state m_current_state{ENABLE_SENSOR_POWER};
  VoltageSensorInterface m_temp_sensor_raw;
  GpioOutputInterface m_sensor_power_enable;
  GpioOutputInterface m_fan_relay_enable;
  PwmOutputInterface m_fan_output_raw;
};