#include <core/logger.h>

int main(void){

    CAL_LOG_FATAL("A test message: %f", 3.14f);
    CAL_LOG_ERROR("A test message: %f", 3.14f);
    CAL_LOG_WARN("A test message: %f", 3.14f);
    CAL_LOG_INFO("A test message: %f", 3.14f);
    CAL_LOG_DEBUG("A test message: %f", 3.14f);    
    CAL_LOG_TRACE("A test message: %f", 3.14f);    

    return 0;
}