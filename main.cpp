
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>

#include "FanController.h"
#include "Drivers.h"

/**
 * @brief Example of how parameters will be passed to your Controller and how it will be executed
 */
void run_test_demo_code(VoltageSensorInterface& temp_sensor_raw,
                        GpioOutputInterface& sensor_power_enable,
                        GpioOutputInterface& fan_relay_enable,
                        PwmOutputInterface& fan_output_raw) 
{
    /** TODO: instantiate and initialize FanController **/
    
    auto fan_controller = FanController(temp_sensor_raw, sensor_power_enable, fan_relay_enable, fan_output_raw);
    fan_controller.init();

    std::chrono::minutes loopDuration(1);
    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::minutes>(currentTime - startTime);
        if (elapsedTime >= loopDuration) {
            break;
        }

    
       fan_controller.loop();
       std::this_thread::sleep_for(std::chrono::milliseconds(500));  
    }

}

int main() {
    VoltageSensorInterface temp_sensor_raw;
    GpioOutputInterface sensor_power_enable;
    GpioOutputInterface fan_relay_enable;
    PwmOutputInterface fan_output_raw;

    run_test_demo_code(temp_sensor_raw, sensor_power_enable, fan_relay_enable, fan_output_raw);
    return 0;
}
