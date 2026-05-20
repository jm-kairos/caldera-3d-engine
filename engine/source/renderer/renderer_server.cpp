#include "renderer_server.h"

b8 renderer_server_create(renderer_server_types type, platform_state *plat_state, RendererServer *out_renderer_server)
{
    out_renderer_server->plat_stat = plat_state;
    if (type == RENDERER_SERVER_TYPE_VULKAN)
    {
        

        return TRUE;
    }
    
    return FALSE;
}

void renderer_server_destroy(RendererServer *renderer_server)
{
    renderer_server->initialize = 0;
    renderer_server->terminate = 0;
    renderer_server->begin_frame = 0;
    renderer_server->end_frame = 0;
    renderer_server->resized = 0;
}
