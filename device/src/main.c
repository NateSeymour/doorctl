#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/async_context.h"
#include "pico/cyw43_arch.h"
#include "pico/time.h"
#include "btstack.h"
#include "driver.h"
#include "device.h"
#include "ble.h"
#include "crypto.h"

[[noreturn]] void spin()
{
    while (true)
    {
        busy_wait_until(at_the_end_of_time);
    }
}

/**
 * Manages the fatal error state of the MCU.
 * In five second intervals, flashes the last registered fatal error code on WL LED.
 */
[[noreturn]] void error_fatal_handler()
{
    while(true)
    {
        // If BLE is not initialized, then we do not have access to the cyw43 chip. The LED is attached to it.
        // Thus can only sleep.
        if (!g_device_state.ble_initialized)
        {
            spin();
        }

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
        busy_wait_ms(2000);

        for(error_code_t i = error_last_fatal.code; i > 0; i--)
        {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
            busy_wait_ms(100);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
            busy_wait_ms(200);
        }
    }
}

void initialize_component(error_t (*init_fn)(void))
{
    error_t error = init_fn();
    if (error_thrown(&error))
    {
        error_fatal(&error);
    }
}

[[noreturn]] int main(void)
{
    // Initialize error IRQs
    irq_set_exclusive_handler(IRQ_ERROR_FATAL, error_fatal_handler);
    irq_set_priority(IRQ_ERROR_FATAL, 0);
    irq_set_enabled(IRQ_ERROR_FATAL, true);

    // Initialize components
    initialize_component(ble_init);
    initialize_component(driver_init);
    initialize_component(crypto_init);

    btstack_run_loop_execute();

    // Dead code because btstack does not return, but is not marked [[noreturn]]
    spin();
}