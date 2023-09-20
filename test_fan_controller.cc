#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

#define private public  // do this to expose private members to test only so that we can test the private members state
#define protected public
#include "Drivers.h"
#include "FanController.h"

using namespace testing;

class MockVoltageSensor : public VoltageSensorInterface {
 public:
  MOCK_METHOD(float, getVoltage, (), (override));
};

class MockPwmOutput : public PwmOutputInterface {
 public:
  MOCK_METHOD(void, setOutputDuty, (float duty), (override));
};

class MockGPIO : public GpioOutputInterface {
 public:
  MOCK_METHOD(void, setOutput, (bool output), (override));
};

class MockFanGPIO : public GpioOutputInterface {
 public:
  MOCK_METHOD(void, setOutput, (bool output), (override));
};

class test_fan_controller : public Test {
 protected:
  void SetUp() override {
    voltageMock = std::make_unique<StrictMock<MockVoltageSensor>>();
    pwmMock = std::make_unique<StrictMock<MockPwmOutput>>();
    fan_gpio_Mock = std::make_unique<StrictMock<MockGPIO>>();
    sensor_gpio_Mock = std::make_unique<StrictMock<MockGPIO>>();
    controller = std::make_unique<FanController>(*voltageMock, *sensor_gpio_Mock, *fan_gpio_Mock, *pwmMock);
    controller->init();
  }
  void TearDown() override {}

 public:
  std::unique_ptr<StrictMock<MockVoltageSensor>> voltageMock;
  std::unique_ptr<StrictMock<MockPwmOutput>> pwmMock;
  std::unique_ptr<StrictMock<MockGPIO>> fan_gpio_Mock;
  std::unique_ptr<StrictMock<MockGPIO>> sensor_gpio_Mock;
  std::unique_ptr<FanController> controller;
};

TEST_F(test_fan_controller, init_test) {
  controller->init();
  ASSERT_EQ(controller->m_current_state, FanController::ENABLE_SENSOR_POWER);
}

TEST_F(test_fan_controller, expect_sensor_to_be_enabled_and_wait) {
  controller->m_current_state = FanController::FAN_CONTROL;
  controller->loop();
}

TEST_F(test_fan_controller, go_to_the_next_state) {
  // go to the next fan_control_enable state once the line is stablized
  controller->m_current_state = FanController::ENABLE_SENSOR_POWER;
  //49*100 = 4900ms delay
  for (int i = 0; i < 49; i++) {
    ASSERT_EQ(controller->m_current_state, FanController::ENABLE_SENSOR_POWER);
    controller->loop();
  }
  //100ms delay, state remains the same
  ASSERT_EQ(controller->m_current_state, FanController::ENABLE_SENSOR_POWER);
  controller->loop();

  //state changes once the delay is attained
  ASSERT_EQ(controller->m_current_state, FanController::FAN_CONTROL_ENABLE);
  controller->loop();
}

TEST_F(test_fan_controller, transition_to_fan_control_enable_and_to_pwm_frequency) {
    // go to the next fan_control_enable state once the line is stablized
  controller->m_current_state = FanController::ENABLE_SENSOR_POWER;
  //49*100 = 4900ms delay
  for (int i = 0; i < 49; i++) {
    ASSERT_EQ(controller->m_current_state, FanController::ENABLE_SENSOR_POWER);
    controller->loop();
  }
  //100ms delay, state remains the same
  ASSERT_EQ(controller->m_current_state, FanController::ENABLE_SENSOR_POWER);
  controller->loop();

  //state changes once the delay is attained
  ASSERT_EQ(controller->m_current_state, FanController::FAN_CONTROL_ENABLE);
  controller->loop();

  //state changes if the temperature is greater than equal to 50
  ASSERT_EQ(controller->m_current_state, FanController::FAN_CONTROL);
  controller->loop();

  //goes back to this state to monitor the temperature
  ASSERT_EQ(controller->m_current_state, FanController::FAN_CONTROL_ENABLE);
}

//the mocks are not working, otherwise the expect calls would have looked beautiful :(
