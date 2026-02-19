#include "driver.h"
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <string.h>
#include "device.h"

void driver_swap_buffers()
{
    driver_buffer_index = (driver_buffer_index + 1) % DRIVER_SCREEN_BUFFER_COUNT;
}

uint8_t *driver_get_front_buffer()
{
    return driver_screen_buffer[driver_buffer_index];
}

uint8_t *driver_get_back_buffer()
{
    return driver_screen_buffer[(driver_buffer_index + 1) % DRIVER_SCREEN_BUFFER_COUNT];
}

void driver_set_address(uint8_t address)
{
    gpio_put(DRIVER_GPIO_A, address & 1);
    gpio_put(DRIVER_GPIO_B, address & 2);
    gpio_put(DRIVER_GPIO_C, address & 4);
}

[[noreturn]] void driver_gpu_entry()
{
    while(true)
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            driver_set_address(i);

            gpio_put(DRIVER_GPIO_OE, true);

            for(int y = 0; y < 32; y++)
            {
                gpio_put(DRIVER_GPIO_CLK, false);

                unsigned int top_index = (i * 32) + y;
                unsigned int bottom_index = ((i + 8) * 32) + y;

                uint8_t *output_buffer = driver_get_front_buffer();

                // Set top
                gpio_put(DRIVER_GPIO_R1, output_buffer[top_index] & 0xFF0000);
                gpio_put(DRIVER_GPIO_G1, output_buffer[top_index] & 0x00FF00);
                gpio_put(DRIVER_GPIO_B1, output_buffer[top_index] & 0x0000FF);

                // Set bottom
                gpio_put(DRIVER_GPIO_R2, output_buffer[bottom_index] & 0xFF0000);
                gpio_put(DRIVER_GPIO_G2, output_buffer[bottom_index] & 0x00FF00);
                gpio_put(DRIVER_GPIO_B2, output_buffer[bottom_index] & 0x0000FF);

                gpio_put(DRIVER_GPIO_CLK, true);
            }

            gpio_put(DRIVER_GPIO_LAT, false);
            gpio_put(DRIVER_GPIO_LAT, true);
            gpio_put(DRIVER_GPIO_OE, false);
            sleep_us(200);
        }
    }
}

void driver_init_io_bank(uint8_t const *bank, size_t count, bool direction)
{
    for (size_t i = 0; i < count; i++)
    {
        gpio_init(bank[i]);
        gpio_set_dir(bank[i], direction);
    }
}

error_t driver_init()
{
    // Initialize IO output
    driver_init_io_bank(k_driver_color_pins, sizeof(k_driver_color_pins), true);
    driver_init_io_bank(k_driver_address_pins, sizeof(k_driver_address_pins), true);
    driver_init_io_bank(k_driver_control_pins, sizeof(k_driver_control_pins), true);

    // Blank screen buffers
    memset(driver_screen_main_buffer, 0, sizeof(driver_screen_main_buffer));

    // Launch GPU routine
    multicore_launch_core1(driver_gpu_entry);

    g_device_state.driver_initialized = true;

    return error_ok;
}
