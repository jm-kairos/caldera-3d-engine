#include "application.h"
#include "game_types.h"

#include "logger.h"
#include "platform/platform.h"

// Singleton design for the state of the application struct.

typedef struct application_state{
    game* game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    real last_time;
} application_state;

// Safety check if there is more than one instance of application state.
static b8 initialized = FALSE;

// Private to this CPP file.
static application_state app_state;

b8 application_init(game* game_inst){
    if (initialized)
    {
        CAL_LOG_ERROR("application_init called more than once !");
        return FALSE;
    }

    app_state.game_inst = game_inst;

    // Initialize subsystems
    logger_init();

    // TODO: remove this
    CAL_LOG_FATAL("A test message: %f", 3.14f);
    CAL_LOG_ERROR("A test message: %f", 3.14f);
    CAL_LOG_WARN("A test message: %f", 3.14f);
    CAL_LOG_INFO("A test message: %f", 3.14f);
    CAL_LOG_DEBUG("A test message: %f", 3.14f);    
    CAL_LOG_TRACE("A test message: %f", 3.14f); 

    app_state.is_running = TRUE;
    // State the application enters in when the window is, for example, minimized.
    app_state.is_suspended = FALSE;

    if (!platform_init(&app_state.platform, 
        game_inst->app_config.name, 
        game_inst->app_config.start_pos_x, 
        game_inst->app_config.start_pos_y, 
        game_inst->app_config.start_width, 
        game_inst->app_config.start_height)){

        CAL_LOG_ERROR("platform_init failed !");
        return FALSE;
    }

    if (!app_state.game_inst->init(app_state.game_inst))
    {
        CAL_LOG_FATAL("Game failed to initialized.");
        return FALSE;
    }

    app_state.game_inst->on_resize(
        app_state.game_inst,
        app_state.width,
        app_state.height
    );

    initialized = TRUE;

    return TRUE;
}

b8 application_run(){

    while (app_state.is_running){   

        if(!platform_pump_messages(&app_state.platform)){
            app_state.is_running = FALSE;
        }

        if (!app_state.is_suspended)
        {
            if (!app_state.game_inst->update(app_state.game_inst, (f32)0))
            {
                CAL_LOG_FATAL("Game update failed, shutting down.");
                app_state.is_running = FALSE;
                break;
            }
            
        }
        
    }
    
    app_state.is_running = FALSE; 

    platform_terminate(&app_state.platform);

    return TRUE;
}