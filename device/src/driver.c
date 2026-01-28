#include "driver.h"
#include <stdlib.h>
#include "device.h"

driver_state_t g_driver_state = {
    .step_index = 0,
    .angle = 0.f,
};

error_t driver_set_power(driver_power_state_t power_state)
{
    return error_ok;
}

error_t driver_step(driver_direction_t direction)
{
    if (abs(direction) != 1)
    {
        error_t error = {
            .code = ERROR_DRIVER_BAD_STEP,
            .message = "Step direction must be equal to -1 or +1",
        };
        return error;
    }

    for (uint8_t i = 0; i < k_driver_pin_count; i++)
    {
        gpio_put(k_driver_pins[i], 0x1 & (k_driver_steps[g_driver_state.step_index] >> i));
    }

    sleep_ms(1);

    gpio_put_all(OFF);

    g_driver_state.step_index = (g_driver_state.step_index + 1 * direction) % k_driver_step_count;

    return error_ok;
}

error_t driver_autosense_angle(bool update_global, float *out_angle)
{
    float angle = 0.f;

    if (out_angle != nullptr)
    {
        *out_angle = angle;
    }

    if (update_global)
    {
        g_driver_state.angle = angle;
    }

    error_t error = {
        .code = ERROR_ROUTINE_NOT_IMPLEMENTED,
        .message = "Driver position autosense is not implemented.",
    };

    return error;
}

error_t driver_init()
{
    for (uint8_t i = 0; i < k_driver_pin_count; i++)
    {
        gpio_init(k_driver_pins[i]);
        gpio_set_dir(k_driver_pins[i], true);
        gpio_set_irq_enabled(k_driver_pins[i], 0b1111, false);
    }

    g_device_state.driver_initialized = true;

    return error_ok;
}
