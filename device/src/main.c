#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/async_context.h"
#include "pico/cyw43_arch.h"
#include "btstack.h"
#include "driver.h"
#include "device.h"
#include "ble.h"


/*
 * Components
 * - Driver
 * - Debug management
 * - Crypto
 * - State management
 */

/**
 * Manages the
 */
[[noreturn]] void error_state_handler()
{
    while (true)
    {}
}

[[noreturn]] int main(void)
{
    error_t e = error_ok;

    // Initialize error IRQs
    //irq_set_exclusive_handler(IRQ_ERROR_FATAL, error_state_handler);
    //irq_set_priority(IRQ_ERROR_FATAL, 0);
    //irq_set_enabled(IRQ_ERROR_FATAL, true);

    // Enable components
    e = driver_init();
    if (error_thrown(&e))
    {
        //error_fatal(&e);
    }

    e = ble_init();
    if (error_thrown(&e))
    {
        //error_fatal(&e);
    }

    btstack_run_loop_execute();
}