#include <core/logger.h>
#include <core/asserts.h>

// TODO: test
#include <platform/platform.h>

int main(void){

    CAL_LOG_FATAL("A test message: %f", 3.14f);
    CAL_LOG_ERROR("A test message: %f", 3.14f);
    CAL_LOG_WARN("A test message: %f", 3.14f);
    CAL_LOG_INFO("A test message: %f", 3.14f);
    CAL_LOG_DEBUG("A test message: %f", 3.14f);    
    CAL_LOG_TRACE("A test message: %f", 3.14f); 

    platform_state state;
    if (platform_init(&state, "Caldera Engine Sandbox", 100, 100, 1280, 720))
    {
        while (TRUE)
        {
            platform_pump_messages(&state);
        }
        
    }

    platform_terminate(&state);
    

    return 0;
}