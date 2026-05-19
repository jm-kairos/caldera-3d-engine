#pragma once

#include "defines.h"

#include "platform/platform.h"

// Supported renderer servers.
typedef enum renderer_server_types{
    RENDERER_SERVER_TYPE_VULKAN,
    RENDERER_SERVER_TYPE_OPENGL,
    RENDERER_SERVER_TYPE_DIRECTX
} renderer_server_types;

struct RendererServer
{
    b8 initialize(const char* app_name, platform_state* plat_stat); 
    void release();
    void resize(u16 width, u16 height);
    b8 begin_frame(real dt);
    b8 end_frame(real dt);
private:
    platform_state* plat_stat;
};
