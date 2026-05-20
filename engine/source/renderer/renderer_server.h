#pragma once

#include "renderer_types.h"

b8 renderer_server_create(renderer_server_types type, platform_state* plat_state, RendererServer* out_renderer_server);
void renderer_server_destroy(RendererServer* renderer_server);
