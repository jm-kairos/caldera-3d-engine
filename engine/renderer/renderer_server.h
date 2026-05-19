#pragma once

#include "renderer_types.h"

struct RendererServer
{
    b8 initialize(const char* app_name, platform_state* plat_stat); 
    void release();
    void resize(u16 width, u16 height);
    b8 begin_frame(real dt);
    b8 end_frame(real dt);
private: 
    platform_state* plat_stat = (platform_state*)0;
};