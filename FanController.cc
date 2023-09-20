
#include <chrono>
#include <iostream>
#include <thread>
#include "FanController.h"

FanController::FanController(const VoltageSensorInterface &temp_sensor_raw, GpioOutputInterface &sensor_power_enable,
                             GpioOutputInterface &fan_relay_enable, PwmOutputInterface &fan_output_raw)
    : m_temp_sensor_raw(temp_sensor_raw),
      m_sensor_power_enable(sensor_power_enable),
      m_fan_relay_enable(fan_relay_enable),
      m_fan_output_raw(fan_output_raw) {}

void FanController::init() {
  m_current_state = ENABLE_SENSOR_POWER;
  m_start_time = std::chrono::time_point<std::chrono::high_resolution_clock>{};
}

void FanController::loop() {
  std::cout << "Current state " << m_current_state << std::endl;
  switch (m_current_state) {
    case ENABLE_SENSOR_POWER: {
      m_sensor_power_enable.setOutput(true);
      if (m_start_time == std::chrono::time_point<std::chrono::high_resolution_clock>{}) {
        m_start_time = std::chrono::high_resolution_clock::now();
      }
      std::this_thread::sleep_for(
          std::chrono::milliseconds(100));  // sleeping for 100 to avoid any system watchdog trigger
      auto currentTime = std::chrono::high_resolution_clock::now();
      double time_elapsed = std::chrono::duration<double, std::milli>(currentTime - m_start_time).count();
      std::cout << "Time elapsed since the high edge" << time_elapsed << std::endl;
      if (time_elapsed >= 5000) {
        std::cout << "Time elapsed" << std::endl;
        m_current_state = FAN_CONTROL_ENABLE;
        m_start_time = std::chrono::time_point<std::chrono::high_resolution_clock>{};
      }
    } break;

    case FAN_CONTROL_ENABLE:
      if (get_current_temperature() < 50.0) {
        m_fan_relay_enable.setOutput(false);
      } else {
        m_fan_relay_enable.setOutput(true);
        m_current_state = FAN_CONTROL;
      }
      break;

    case FAN_CONTROL:
      double pwm_duty = private_get_fan_duty_cycle();
      m_fan_output_raw.setOutputDuty(pwm_duty);
      m_current_state = FAN_CONTROL_ENABLE;
      break;
  }
}

double FanController::private_get_fan_duty_cycle() {
  double frequency{0.0};
  double current_temperature{get_current_temperature()};
  if ((current_temperature >= 50) && (current_temperature < 99)) {
    frequency = (current_temperature - 50U) / 2U;

  } else if ((current_temperature >= 100) && (current_temperature < 129)) {
    frequency = (current_temperature - 98U) / 2U;

  } else if ((current_temperature >= 130) && (current_temperature < 140)) {
    frequency = 90;

  } else if (current_temperature > 140) {
    frequency = 100;
  } else {
    // misra
  }

  return frequency;
}

double FanController::get_current_temperature() {
  auto vin = m_temp_sensor_raw.getVoltage();
  auto temperature = ((vin - 1.375) / (22.5 / 1000));
  std::cout << "current_temperature = " << temperature << std::endl;
  return temperature;
}
