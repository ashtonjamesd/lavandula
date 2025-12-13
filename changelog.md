# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

<hr/>

## [0.0.1] - 2025-11-12

### Added

- Initial release


## [0.0.2] - 2025-12-13

### Added

- **Arena Allocator**: New per-request memory pool allocator (`src/arena.c`, `include/arena.h`)
  - Efficient allocation with `arenaAlloc()`, `arenaStrdup()`, `arenaStrndup()`, `arenaCalloc()`
  - Single-operation cleanup with `arenaDestroy()`
  - Configurable block sizes with `arenaCreateWithSize()`
  - Memory usage tracking with `arenaGetUsed()` and `arenaGetTotalAllocated()`
  
- **Error Handling System**: Structured error codes and macros (`src/lavu_error.c`, `include/lavu_error.h`)
  - `LavuError` enum with common error codes
  - `lavuErrorStr()` for human-readable error messages
  - `LAVU_TRY` and `LAVU_TRY_CLEANUP` macros for error propagation
  - Safe allocation macros: `LAVU_ALLOC`, `LAVU_STRDUP`, `LAVU_ARENA_ALLOC`

- **RequestContext Arena**: Per-request arena attached to `RequestContext.arena`

- **Safe Validator API**: New functions that return error codes instead of exiting
  - `addRuleSafe()` - add validation rule with error return
  - `requiredSafe()` - add required field rule with error return

- **Documentation**: Added `doc/api/arena.md` and `doc/api/error_handling.md`

- **Tests**: Added comprehensive test suites
  - `test/arena_test.c` - arena allocator tests
  - `test/error_test.c` - error handling tests  
  - `test/validator_test.c` - validator tests

### Changed

- Server now uses arena allocator for per-request memory management
- Server no longer exits on write failures, logs error and continues
- `freeValidator()` now properly frees all strdup'd field/message strings
- `freeValidator()` is now safe to call multiple times
- Validator initial capacity increased from 1 to 4

### Fixed

- **Memory leak in validator**: `freeValidator()` was not freeing rule strings
- **Memory leak in server**: HTTP parser was not being freed after request
- **Validator logic bug**: NULL checks were inverted in `addRule()`
- **Missing NULL checks**: Added NULL checks after realloc in validator

### Security

- Improved memory safety with arena allocator pattern
- Graceful error handling prevents server crashes on memory failures
