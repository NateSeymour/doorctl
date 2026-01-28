#ifndef DOORCTL_DRIVER_H
#define DOORCTL_DRIVER_H

#include <stdint.h>
#include "error.h"

typedef struct
{
    uint8_t step_index;
    float angle;
} driver_state_t;

extern driver_state_t g_driver_state;

typedef enum : int8_t
{
    LEFT = -1,
    RIGHT = 1,
} driver_direction_t;

typedef enum
{
    OFF,
    ON,
} driver_power_state_t;

constexpr uint8_t k_driver_pins[] = { 2, 3, 4, 5 };
constexpr uint8_t k_driver_pin_count = 4;

/**
 * Driver follows an 8 step motor driving pattern.
 * The pattern controls four outputs (BLUE - 1, PINK - 2, YELLOW - 3, ORANGE - 4).
 * Outputs are encoded into a bitfield on the lower nibble of a byte.
 */
constexpr uint8_t k_driver_steps[] = {
    0b0001,
    0b0011,
    0b0010,
    0b0110,
    0b0100,
    0b1100,
    0b1000,
    0b1001,
};
constexpr uint8_t k_driver_step_count = 8;

constexpr uint32_t k_driver_steps_per_rotation = 4096;

error_t driver_set_power(driver_power_state_t power_state);

/**
 * Step the motor in the desired direction. Updates g_driver_state global struct.
 * @param direction LEFT or RIGHT.
 * @return Error.
 */
error_t driver_step(driver_direction_t direction);

/**
 * Discovers the current angle of the motor.
 * @param update_global Update the global driver_state.
 * @param out_angle Current angle in degrees of the motor.
 * @return Error.
 */
error_t driver_autosense_angle(bool update_global, float *out_angle);

/**
 * Initializes the motor driver by completing one forward-reverse step run to ensure motor is aligned with step.
 * @return Error.
 */
error_t driver_init();

#endif //DOORCTL_DRIVER_H