#ifndef arena_h
#define arena_h

#include <stddef.h>
#include <stdbool.h>

// Default arena block size (64KB)
#define ARENA_DEFAULT_BLOCK_SIZE (64 * 1024)

// Arena block for linked list of memory blocks
typedef struct ArenaBlock {
    struct ArenaBlock *next;
    size_t             size;
    size_t             used;
    char               data[];  // Flexible array member
} ArenaBlock;

// Arena allocator - per-request memory pool
typedef struct {
    ArenaBlock *head;          // First block
    ArenaBlock *current;       // Current block for allocations
    size_t      blockSize;     // Default block size
    size_t      totalAllocated; // Total bytes allocated
} Arena;

// Error codes for arena operations
typedef enum {
    ARENA_OK = 0,
    ARENA_ERR_NOMEM = -1,
    ARENA_ERR_NULL = -2,
} ArenaError;

// Initialize a new arena with default block size
Arena *arenaCreate(void);

// Initialize a new arena with custom block size
Arena *arenaCreateWithSize(size_t blockSize);

// Allocate memory from the arena (never returns NULL on success)
// Returns NULL only on allocation failure
void *arenaAlloc(Arena *arena, size_t size);

// Allocate zeroed memory from the arena
void *arenaCalloc(Arena *arena, size_t count, size_t size);

// Duplicate a string using arena memory
char *arenaStrdup(Arena *arena, const char *str);

// Duplicate a string with max length using arena memory
char *arenaStrndup(Arena *arena, const char *str, size_t maxLen);

// Reset the arena (keeps allocated blocks but marks them as unused)
// This is faster than destroy+create for reusing arenas
void arenaReset(Arena *arena);

// Destroy the arena and free all memory
void arenaDestroy(Arena *arena);

// Get total bytes allocated by this arena
size_t arenaGetTotalAllocated(Arena *arena);

// Get total bytes currently used
size_t arenaGetUsed(Arena *arena);

#endif
