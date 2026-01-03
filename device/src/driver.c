#include "driver.h"
#include "device.h"

driver_state_t g_driver_state = {
    .step_index = 0,
    .angle = 0.f,
};

error_t driver_init()
{
    error_t error = {
        .code = ERROR_ROUTINE_NOT_IMPLEMENTED,
        .message = "Driver initialization is not implemented.",
    };
    return error;

    // TODO: implement
    g_device_state.driver_initialized = true;

    return error_ok;
}

error_t driver_autosense_angle(bool update_global, float *out_angle)
{
    float angle = 0.f;
    *out_angle = angle;

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
