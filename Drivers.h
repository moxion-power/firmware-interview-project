#pragma once

/**
 * @brief Interface for getting a raw voltage sensor measurement
 */
class VoltageSensorInterface {

public:
  /**
   * @brief Get the most recent voltage measurement
   * @return Latest voltage measurement [0 12]
   */
  virtual float getVoltage() { return 4.0f; };
};

/**
 * @brief Interface for commanding a PWM output
 */
class PwmOutputInterface {

public:
  /**
   * @brief Set the Output Duty object
   * @param duty Duty cycle in percent [0 100] 
   */
  virtual void setOutputDuty(float duty) {};
};

/**
 * @brief Interface for commanding a GPIO output
 */
class GpioOutputInterface {

public:
  /**
   * @brief Set the Output object
   * @param output True to enable output, False to disable output
   */
  virtual void setOutput(bool output) {};
};
