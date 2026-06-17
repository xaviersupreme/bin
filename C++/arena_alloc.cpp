#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef uintptr_t uptr;

typedef struct mem_arena {
    u64 capacity;
    u64 offset; 
    
    u8* memory_block;
} mem_arena;

#define KiB(n) ((u64)n << (10))
#define MiB(n) ((u64)n << (20))
#define GiB(n) ((u64)n << (30))

mem_arena init_arena(u64 capacity);
void* alloc_arena(mem_arena* arena, u64 size);
void reset_arena(mem_arena* arena);
void f_reset_arena(mem_arena* arena);

template<typename T>
void* push_var_arena(mem_arena* arena, T &var, u64 size);

int main(void) {
    mem_arena perm_arena = init_arena(KiB(1));

    const char* f = "Hello, world!";

    char** ptr = (char**)push_var_arena(&perm_arena, f, sizeof(char) * 13);

    printf("%s, %p\n", *ptr, ptr);

    f_reset_arena(&perm_arena);
}

mem_arena init_arena(u64 capacity) {
    if (capacity == 0) throw std::runtime_error("Attempt to set Arena capacity to or less than 0");

    mem_arena arena;
    arena.memory_block = (u8*)malloc(capacity);

    if (!arena.memory_block || arena.memory_block == nullptr) throw std::runtime_error("Failed to allocate memory");

    arena.capacity = capacity;
    arena.offset = 0;

    return arena;
}

void* alloc_arena(mem_arena* arena, u64 size) {
    if (!arena->memory_block || arena->memory_block == nullptr) throw std::runtime_error("Arena not initialized\n");

    if (arena->offset + size > arena->capacity) throw std::runtime_error("Arena out of memory\n");

    u8* result = arena->memory_block + arena->offset;
    arena->offset += size;

    return result;
}

void reset_arena(mem_arena* arena) {
    if (arena->capacity == 0) throw std::runtime_error("Attempt to reset unused Arena.\n");

    arena->offset = 0;
}

void f_reset_arena(mem_arena* arena) {
    if (!arena->memory_block || arena->memory_block == nullptr) {
        throw std::runtime_error("Attempt to free uninitialized Arena");
    } else {
        free(arena->memory_block);
    }

    arena->memory_block = nullptr;
    arena->capacity = 0;
    arena->offset = 0;
}

template<typename T>
void* push_var_arena(mem_arena* arena, T &var, u64 size) {
    if (size == 0) throw std::runtime_error("Attempt to push to arena with 0 bytes");

    T* allocated = (T*)alloc_arena(arena, size);
    *allocated = var;;

    return allocated;
}
