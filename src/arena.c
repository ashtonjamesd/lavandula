#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "include/arena.h"

// Align to 8-byte boundary for better performance
#define ARENA_ALIGNMENT 8
#define ALIGN_UP(size) (((size) + (ARENA_ALIGNMENT - 1)) & ~(ARENA_ALIGNMENT - 1))

static ArenaBlock *createBlock(size_t size) {
    ArenaBlock *block = malloc(sizeof(ArenaBlock) + size);
    if (!block) {
        return NULL;
    }
    
    block->next = NULL;
    block->size = size;
    block->used = 0;
    
    return block;
}

Arena *arenaCreate(void) {
    return arenaCreateWithSize(ARENA_DEFAULT_BLOCK_SIZE);
}

Arena *arenaCreateWithSize(size_t blockSize) {
    Arena *arena = malloc(sizeof(Arena));
    if (!arena) {
        return NULL;
    }
    
    ArenaBlock *block = createBlock(blockSize);
    if (!block) {
        free(arena);
        return NULL;
    }
    
    arena->head = block;
    arena->current = block;
    arena->blockSize = blockSize;
    arena->totalAllocated = blockSize;
    
    return arena;
}

void *arenaAlloc(Arena *arena, size_t size) {
    if (!arena || size == 0) {
        return NULL;
    }
    
    size_t alignedSize = ALIGN_UP(size);
    
    // Check if current block has enough space
    if (arena->current->used + alignedSize <= arena->current->size) {
        void *ptr = arena->current->data + arena->current->used;
        arena->current->used += alignedSize;
        return ptr;
    }
    
    // Need a new block - use larger of requested size or default block size
    size_t newBlockSize = alignedSize > arena->blockSize ? alignedSize : arena->blockSize;
    ArenaBlock *newBlock = createBlock(newBlockSize);
    if (!newBlock) {
        return NULL;
    }
    
    // Link new block
    arena->current->next = newBlock;
    arena->current = newBlock;
    arena->totalAllocated += newBlockSize;
    
    void *ptr = newBlock->data;
    newBlock->used = alignedSize;
    
    return ptr;
}

void *arenaCalloc(Arena *arena, size_t count, size_t size) {
    size_t total = count * size;
    void *ptr = arenaAlloc(arena, total);
    if (ptr) {
        memset(ptr, 0, total);
    }
    return ptr;
}

char *arenaStrdup(Arena *arena, const char *str) {
    if (!str) {
        return NULL;
    }
    
    size_t len = strlen(str);
    char *copy = arenaAlloc(arena, len + 1);
    if (copy) {
        memcpy(copy, str, len + 1);
    }
    return copy;
}

char *arenaStrndup(Arena *arena, const char *str, size_t maxLen) {
    if (!str) {
        return NULL;
    }
    
    size_t len = strlen(str);
    if (len > maxLen) {
        len = maxLen;
    }
    
    char *copy = arenaAlloc(arena, len + 1);
    if (copy) {
        memcpy(copy, str, len);
        copy[len] = '\0';
    }
    return copy;
}

void arenaReset(Arena *arena) {
    if (!arena) {
        return;
    }
    
    // Reset all blocks to unused state
    ArenaBlock *block = arena->head;
    while (block) {
        block->used = 0;
        block = block->next;
    }
    
    arena->current = arena->head;
}

void arenaDestroy(Arena *arena) {
    if (!arena) {
        return;
    }
    
    ArenaBlock *block = arena->head;
    while (block) {
        ArenaBlock *next = block->next;
        free(block);
        block = next;
    }
    
    free(arena);
}

size_t arenaGetTotalAllocated(Arena *arena) {
    return arena ? arena->totalAllocated : 0;
}

size_t arenaGetUsed(Arena *arena) {
    if (!arena) {
        return 0;
    }
    
    size_t used = 0;
    ArenaBlock *block = arena->head;
    while (block) {
        used += block->used;
        block = block->next;
    }
    return used;
}
