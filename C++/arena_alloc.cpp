#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdexcept>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef uintptr_t uptr;

typedef struct mem_arena {
    u8* memory_block = 0x0;

    u64 capacity = 0;
    u64 offset = 0;
} mem_arena;

#define KiB(n) ((u64)n << (10))
#define MiB(n) ((u64)n << (20))
#define GiB(n) ((u64)n << (30))

inline u64 align_up(u64 operand, u64 alignment) {
    return ((operand + (alignment - 1)) & ~(alignment - 1));
}

mem_arena init_arena(u64 capacity);
void* alloc_arena(mem_arena* arena, u64 size);
void reset_arena(mem_arena* arena);
void f_reset_arena(mem_arena* arena);

template<typename T>
T* push_var_arena(mem_arena* arena, T &var);

template<typename T>
T* push_arr_arena(mem_arena* arena, T arr[], u64 elems);

int main(void) {
    mem_arena perm_arena = init_arena(KiB(1));

    const char* f = "Hello, world!";

    char** ptr = (char**)push_var_arena(&perm_arena, f);

    printf("%s, %p\n", *ptr, (void*)ptr);

    u16 arr[] = {1,2,3,4, 5, 1,3,3};

    u16* array_allocated = (u16*)push_arr_arena(&perm_arena, arr, 8);

    for (u64 i{}; i < 8; ++i) {
        printf("i = %hu\n", array_allocated[i]);
    }

    f_reset_arena(&perm_arena);
}

mem_arena init_arena(u64 capacity) {
    if (capacity == 0) throw std::runtime_error("[F] Attempt to set Arena capacity to or less than 0");

    mem_arena arena;
    arena.memory_block = (u8*)malloc(capacity);

    if (!arena.memory_block || arena.memory_block == nullptr) throw std::runtime_error("[F] Failed to allocate memory");

    arena.capacity = capacity;
    arena.offset = 0;

    return arena;
}

void* alloc_arena(mem_arena* arena, u64 size) {
    if (!arena->memory_block || arena->memory_block == nullptr) throw std::runtime_error("[F] Arena not initialized\n");

    if (arena->offset + size > arena->capacity) throw std::runtime_error("[F] Arena out of memory\n");

    u8* result = arena->memory_block + (arena->offset);
    arena->offset += size;

    return result;
}

void reset_arena(mem_arena* arena) {
    if (arena->offset == 0 || arena->capacity) throw std::runtime_error("[F] Attempt to reset unused Arena.\n");

    arena->offset = 0;
}

void f_reset_arena(mem_arena* arena) {
    if (!arena->memory_block || arena->memory_block == nullptr) {
        throw std::runtime_error("[F] Attempt to free uninitialized Arena");
    } else {
        free(arena->memory_block);
    }

    arena->memory_block = nullptr;
    arena->capacity = 0;
    arena->offset = 0;
}

template<typename T>
T* push_var_arena(mem_arena* arena, T &var) {
    if (sizeof(var) == 0) throw std::runtime_error("[F] Attempt to call push_var_arena(...) with a 0 byte variable");

    arena->offset = align_up(arena->offset, sizeof(T));
    T* allocated = (T*)alloc_arena(arena, sizeof(T));

    if (!(arena->memory_block)) throw std::runtime_error("[F] Failed to allocate memory in push_var_arena(...)");

    *allocated = var;

    return allocated;
}

template<typename T>
T* push_arr_arena(mem_arena* arena, T arr[], u64 elems) {
    arena->offset = align_up(arena->offset, sizeof(T));
    T* allocated = (T*)alloc_arena(arena, sizeof(T) * elems);
    
    if (!(allocated) || allocated == nullptr) throw std::runtime_error("[F] Failed to allocate memory for array in push_arr_arena(...)");

    for (u64 i{}; i < elems; ++i) {
        allocated[i] = arr[i];
        if (!(allocated[i] == arr[i])) {
            std::cerr << "[W] Failed to assign current array index to current allocated index. Returning early.\n";
            break;
        }
    }

    return allocated;
}
