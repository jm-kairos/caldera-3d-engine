#pragma once

#include "defines.h"
#include "core/cal_memory.h"

#define VECTOR_INITIAL_CAPACITY 32

struct allocator_t{
    VOID_PTR (*alloc)(u64 size, memory_tag tag, VOID_PTR context);
    VOID_PTR (*free)(u64 size, VOID_PTR ptr, memory_tag tag, VOID_PTR context);
    VOID_PTR context;
};

VOID_PTR my_alloc(u64 size, memory_tag tag, VOID_PTR context) {
    (void)context;
    return cal_memory_allocator(size, tag);
}

VOID_PTR my_free(u64 size, VOID_PTR ptr, memory_tag tag, VOID_PTR context) {
    (void)ptr; 
    (void)context;
    cal_memory_free(ptr, size, tag);
}

allocator_t alloc = {my_alloc, my_free, 0};

struct vector_header_t{
    u64 length;
    u64 capacity;
    u64 padding;
    allocator_t* allocator;
};

#define vector(_T, _alloc) vector_init(sizeof(_T), VECTOR_INITIAL_CAPACITY, _alloc)

VOID_PTR vector_init(u64 data_type_size_in_bytes, u64 vector_capacity, allocator* allocator);


