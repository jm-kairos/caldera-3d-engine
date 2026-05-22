#pragma once

#include "defines.h"

// Based on the tag we will be able to aligned or not aligned memory.
typedef enum memory_tag{
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_VECTOR,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,
    MEMORY_TAG_ARENA,

    MEMORY_TAG_MAX_TAGS // Should always be the laste element.
} memory_tag;

// TODO: these shall not be exported !
erg__api__ void cal_memory_initialize();
erg__api__ void cal_memory_terminate();

erg__api__ VOID_PTR cal_memory_allocator_virtual_memory_commit(VOID_PTR block, u64 commit_size);
erg__api__ VOID_PTR cal_memory_allocator_virtual_memory_reserve(u64 reserve_size);
erg__api__ void cal_memory_virtual_free(VOID_PTR block, u64 size);

erg__api__ VOID_PTR cal_memory_allocator(u64 size, memory_tag tag);
erg__api__ void cal_memory_free(VOID_PTR block, u64 size, memory_tag tag);

erg__api__ VOID_PTR cal_memory_zero(VOID_PTR block, u64 size);
erg__api__ VOID_PTR cal_memory_copy(VOID_PTR dest, const VOID_PTR source, u64 size);
erg__api__ VOID_PTR cal_memory_set(VOID_PTR dest, i32 value, u64 size);

// Debug function. Prints usage statistics to the console.
erg__api__ char* cal_memory_get_memory_usage_string();

