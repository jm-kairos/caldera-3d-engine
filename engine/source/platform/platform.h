#pragma once

#include "defines.h"

typedef struct platform_state{
    void* internal_state; // type of this will be determined on the particular implementation on the cpp file
} platform_state;

b8 platform_init(
    platform_state* plat_state,
    const char* app_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height);

void platform_terminate(platform_state* plat_state);

b8 platform_pump_messages(platform_state* plat_state);

CALDERA_API void* platform_allocate(u64 size, b8 aligned);
CALDERA_API void platform_free(void* block, b8 aligned);
void* platform_zero_memory(void* block, u64 size);
void* platfor_copy_memory(void* dest, const void* source, u64 size);
void* platform_set_memory(void* dest, i32 value, u64 size);

void platform_console_write(const char* message, u8 colour);
void platform_console_write_error(const char* message, u8 colour);

real platform_get_absolute_time();

// Sleep on the thread for the provided ms. This cloks the mais thread.
// Should only be used for giving time back to the OS for unused update power.
// Therefore it is not exported.
void platform_sleep(u64 ms);