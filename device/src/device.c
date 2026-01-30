#include "device.h"
#include "driver.h"

device_state_t g_device_state = {
    .ble_initialized = false,
    .driver_initialized = false,

    .last_command_authorization = 0,
};

device_lock_state_t device_get_lock_state()
{
    float angle = driver_get_angle_deg();
    if (angle < 45.f)
    {
        return LOCKED;
    }
    else
    {
        return UNLOCKED;
    }
}

error_t device_set_lock_state(device_lock_state_t lock_state)
{
    if (lock_state == LOCKED)
    {
        driver_set_angle_deg(0.f);
    }
    else
    {
        driver_set_angle_deg(90.f);
    }

    return error_ok;
}