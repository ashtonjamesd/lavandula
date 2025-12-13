# Arena Allocator

Lavandula includes a high-performance arena (pool) allocator designed for efficient per-request memory management. This significantly reduces the overhead of many small allocations during request processing.

## Overview

An arena allocator works by pre-allocating large blocks of memory and then efficiently distributing portions of these blocks for individual allocations. When the request is complete, the entire arena can be freed with a single operation, eliminating the need to track and free individual allocations.

### Benefits

- **Performance**: Reduces overhead from many small `malloc`/`free` calls
- **Simplicity**: No need to track individual allocations for cleanup
- **Cache-friendly**: Allocations are contiguous in memory
- **No fragmentation**: Memory is released as a whole

## API Reference

### Creating an Arena

```c
#include "include/arena.h"

// Create with default block size (64KB)
Arena *arena = arenaCreate();

// Create with custom block size
Arena *arena = arenaCreateWithSize(1024 * 128);  // 128KB blocks
```

### Allocating Memory

```c
// Basic allocation (8-byte aligned)
void *ptr = arenaAlloc(arena, 256);

// Zero-initialized allocation
int *numbers = arenaCalloc(arena, 100, sizeof(int));

// String duplication
char *copy = arenaStrdup(arena, "Hello, World!");

// String duplication with max length
char *prefix = arenaStrndup(arena, "Hello, World!", 5);  // "Hello"
```

### Memory Management

```c
// Reset arena (reuse blocks, mark as unused)
arenaReset(arena);

// Destroy arena (free all memory)
arenaDestroy(arena);

// Query memory usage
size_t total = arenaGetTotalAllocated(arena);
size_t used = arenaGetUsed(arena);
```

## Usage in Request Handling

Each HTTP request automatically gets an arena attached to its `RequestContext`:

```c
HttpResponse myHandler(RequestContext ctx) {
    // Use the per-request arena for temporary allocations
    char *temp = arenaStrdup(ctx.arena, "temporary string");
    
    // No need to free - arena is destroyed when request completes
    
    return ok("{\"status\": \"success\"}", APPLICATION_JSON);
}
```

### Best Practices

1. **Use arena for request-scoped data**: Any data that only needs to live for the duration of a request should use the arena.

2. **Don't store arena pointers long-term**: Arena memory is freed when the request ends.

3. **Fall back gracefully**: The arena may be NULL in edge cases. Use traditional allocation as a fallback:

```c
char *str = ctx.arena 
    ? arenaStrdup(ctx.arena, source)
    : strdup(source);
```

4. **Large allocations are fine**: The arena automatically creates larger blocks for allocations exceeding the default block size.

## Implementation Details

- Default block size: 64KB
- Alignment: 8 bytes
- Thread safety: Not thread-safe (one arena per request)
- Growth strategy: New blocks are allocated as needed

### Memory Layout

```
Arena
  ├── head -> Block1 (64KB)
  │             ├── used: 1024 bytes
  │             └── data[...]
  │
  └── current -> Block2 (64KB)
                  ├── used: 256 bytes
                  └── data[...]
```

## Error Handling

Arena functions return `NULL` on allocation failure rather than crashing:

```c
void *ptr = arenaAlloc(arena, size);
if (!ptr) {
    // Handle allocation failure
    return internalServerError("Out of memory", TEXT_PLAIN);
}
```

See also: [Error Handling](./error_handling.md)
