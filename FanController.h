#pragma once

#include "Drivers.h"
#include <chrono>
/**
 * @brief Example FanController base
 */
class FanController{
public:
  enum state {
    ENABLE_SENSOR_POWER = 0,
    FAN_CONTROL_ENABLE, // enables the temperature sensor
    FAN_CONTROL,        // all pwm happens here
  };
  /*
   * @brief One time executed initialization method
   */
  void init();
  /**
   * @brief Periodic loop method that will be called every 500ms
   */
  void loop();
  FanController(const VoltageSensorInterface &temp_sensor_raw,
                             GpioOutputInterface &sensor_power_enable,
                             GpioOutputInterface &fan_relay_enable,
                             PwmOutputInterface &fan_output_raw);

private:
  double private_get_fan_duty_cycle();
  double get_current_temperature();
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
  state m_current_state{ENABLE_SENSOR_POWER};
  VoltageSensorInterface m_temp_sensor_raw;
  GpioOutputInterface m_sensor_power_enable;
  GpioOutputInterface m_fan_relay_enable;
  PwmOutputInterface m_fan_output_raw;
};
