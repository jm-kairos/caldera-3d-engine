#include "vector.h"

#define vector_header(_v) ((vector_header_t *)(_v) - 1)
#define vector_length(_v) (vector_header(_v)->length)
#define vector_capacity(_v) (vector_header(_v)->capacity)

#define vector_append(_v, _i) do {\
    (_v) = vector_ensure_capacity(_v, 1, sizeof(_i));\
    (_v)[vector_header(vector)]\
    &(a)[];
} while(0)\

VOID_PTR vector_init(
    u64 data_type_size_in_bytes, 
    u64 vector_capacity, 
    allocator* allocator)
{
    VOID_PTR out_ptr = 0;

    u64 size = data_type_size_in_bytes * vector_capacity + sizeof(vector_header_t);

    vector_header_t* vh = (vector_header_t*)allocator->alloc(size, MEMORY_TAG_VECTOR, 0);

    if (vh)
    {
        vh->capacity = vector_capacity;
        vh->length = 0;
        vh->allocator = allocator;
        out_ptr = vh + 1;
    }
    
    return out_ptr;
}
