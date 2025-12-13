#include <stdio.h>
#include <string.h>
#include "../src/include/lavandula_test.h"
#include "../src/include/arena.h"

void test_arena_create() {
    Arena *arena = arenaCreate();
    expectNotNull(arena);
    
    if (arena) {
        expect(arenaGetTotalAllocated(arena), toBe(ARENA_DEFAULT_BLOCK_SIZE));
        expect(arenaGetUsed(arena), toBe(0));
        arenaDestroy(arena);
    }
}

void test_arena_create_with_size() {
    size_t customSize = 1024;
    Arena *arena = arenaCreateWithSize(customSize);
    expectNotNull(arena);
    
    if (arena) {
        expect(arenaGetTotalAllocated(arena), toBe(customSize));
        arenaDestroy(arena);
    }
}

void test_arena_alloc_basic() {
    Arena *arena = arenaCreate();
    expectNotNull(arena);
    
    if (arena) {
        void *ptr1 = arenaAlloc(arena, 64);
        expectNotNull(ptr1);
        
        void *ptr2 = arenaAlloc(arena, 128);
        expectNotNull(ptr2);
        
        // Pointers should be different
        expectNot(ptr1, ptr2);
        
        // Used should be > 0
        size_t used = arenaGetUsed(arena);
        expectNot(used, toBe(0));
        
        arenaDestroy(arena);
    }
}

void test_arena_alloc_zero_size() {
    Arena *arena = arenaCreate();
    expectNotNull(arena);
    
    if (arena) {
        void *ptr = arenaAlloc(arena, 0);
        expectNull(ptr);  // Zero size should return NULL
        arenaDestroy(arena);
    }
}

void test_arena_strdup() {
    Arena *arena = arenaCreate();
    expectNotNull(arena);
    
    if (arena) {
        const char *original = "Hello, World!";
        char *copy = arenaStrdup(arena, original);
        
        expectNotNull(copy);
        if (copy) {
            expect(strcmp(copy, original), toBe(0));
        }
        
        // NULL should return NULL
        char *nullCopy = arenaStrdup(arena, NULL);
        expectNull(nullCopy);
        
        arenaDestroy(arena);
    }
}

void test_arena_strndup() {
    Arena *arena = arenaCreate();
    expectNotNull(arena);
    
    if (arena) {
        const char *original = "Hello, World!";
        char *copy = arenaStrndup(arena, original, 5);
        
        expectNotNull(copy);
        if (copy) {
            expect(strcmp(copy, "Hello"), toBe(0));
            expect(strlen(copy), toBe(5));
        }
        
        arenaDestroy(arena);
    }
}

void test_arena_calloc() {
    Arena *arena = arenaCreate();
    expectNotNull(arena);
    
    if (arena) {
        int *arr = arenaCalloc(arena, 10, sizeof(int));
        expectNotNull(arr);
        
        if (arr) {
            // All bytes should be zero
            for (int i = 0; i < 10; i++) {
                expect(arr[i], toBe(0));
            }
        }
        
        arenaDestroy(arena);
    }
}

void test_arena_reset() {
    Arena *arena = arenaCreate();
    expectNotNull(arena);
    
    if (arena) {
        // Allocate some memory
        arenaAlloc(arena, 1000);
        arenaAlloc(arena, 2000);
        
        size_t usedBefore = arenaGetUsed(arena);
        expectNot(usedBefore, toBe(0));
        
        // Reset arena
        arenaReset(arena);
        
        size_t usedAfter = arenaGetUsed(arena);
        expect(usedAfter, toBe(0));
        
        // Total allocated should remain the same (blocks not freed)
        size_t total = arenaGetTotalAllocated(arena);
        expectNot(total, toBe(0));
        
        arenaDestroy(arena);
    }
}

void test_arena_large_allocation() {
    Arena *arena = arenaCreateWithSize(1024);  // Small block size
    expectNotNull(arena);
    
    if (arena) {
        // Allocate larger than block size
        void *ptr = arenaAlloc(arena, 2048);
        expectNotNull(ptr);
        
        // Should have created a new larger block
        size_t total = arenaGetTotalAllocated(arena);
        expectNot(total, toBe(1024));  // Should be more than initial
        
        arenaDestroy(arena);
    }
}

void test_arena_many_allocations() {
    Arena *arena = arenaCreate();
    expectNotNull(arena);
    
    if (arena) {
        // Simulate many small allocations (like per-request)
        for (int i = 0; i < 1000; i++) {
            void *ptr = arenaAlloc(arena, 64);
            expectNotNull(ptr);
            if (!ptr) break;
        }
        
        arenaDestroy(arena);
    }
}

void runArenaTests() {
    printf("\n--- Arena Allocator Tests ---\n");
    runTest(test_arena_create);
    runTest(test_arena_create_with_size);
    runTest(test_arena_alloc_basic);
    runTest(test_arena_alloc_zero_size);
    runTest(test_arena_strdup);
    runTest(test_arena_strndup);
    runTest(test_arena_calloc);
    runTest(test_arena_reset);
    runTest(test_arena_large_allocation);
    runTest(test_arena_many_allocations);
}
