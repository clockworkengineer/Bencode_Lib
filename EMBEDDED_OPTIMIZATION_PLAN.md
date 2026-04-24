# Embedded Systems Suitability Plan for Bencode_Lib

## Objective

Make `Bencode_Lib` suitable for constrained embedded systems by reducing dynamic memory usage, eliminating optional heap allocations, removing non-portable runtime dependencies, and providing a minimal build configuration.

## Current Embedded Challenges

1. **Heavy dynamic allocation**
   - `Node` is backed by `std::unique_ptr<Variant>`, causing one allocation per node.
   - `String` and `DictionaryEntry` use `std::string`.
   - `List` and `Dictionary` use `std::vector` for element storage.
   - `BufferSource` copies input into `std::vector<std::byte>`.
   - `BufferDestination` stores output in `std::vector<std::byte>` and constructs `std::string` output.

2. **Exceptions everywhere**
   - `std::runtime_error` and derived exception types are used for parse and I/O errors.
   - Embedded deployments often disable exceptions or pay a large code-size penalty.

3. **Non-embedded file I/O dependencies**
   - `FileSource` uses `<fstream>` and `std::ifstream`.
   - `FileDestination` likely uses `<ofstream>`.
   - These are too heavy or unavailable on many embedded toolchains.

4. **Standard library dependencies and utility overhead**
   - `std::to_string` and `std::string_view` are used in stringify and parse.
   - `std::ranges` and `std::function` are used in the parser and dictionary logic.
   - `std::variant`/`std::unique_ptr` are used in the node implementation.

5. **Lack of explicit bounded memory mode**
   - There is no compile-time mode for fixed-size nodes, fixed capacity dictionaries/lists, or arena-backed parsing.

## Embedded Suitability Goals

- Add a minimal embedded build mode via CMake configuration.
- Provide compile-time options for:
  - exceptions off
  - file I/O off
  - dynamic allocation off or limited
  - small-string optimization / fixed-size buffers
- Replace heap-allocation-heavy data structures with static/arena-backed equivalents.
- Support error reporting by return codes/status objects instead of exceptions.
- Reduce footprint by avoiding iostreams, `std::vector`, `std::string` when possible.
- Maintain a small subset of the public API for parsing and stringifying.

## Concrete Plan

### Phase 1: Add explicit embedded build configuration

1. Add CMake options in `CMakeLists.txt`:
   - `BENCODE_EMBEDDED_MODE` (OFF by default)
   - `BENCODE_ENABLE_EXCEPTIONS` (ON by default)
   - `BENCODE_ENABLE_FILE_IO` (ON by default)
   - `BENCODE_ENABLE_DYNAMIC_ALLOCATION` (ON by default)
   - `BENCODE_ENABLE_JSON_STRINGIFY` / `BENCODE_ENABLE_XML_STRINGIFY` / `BENCODE_ENABLE_YAML_STRINGIFY`
   - `BENCODE_MAX_NODE_COUNT`, `BENCODE_MAX_CONTAINER_SIZE`, `BENCODE_MAX_STRING_LENGTH`
2. Expose these definitions to consumers via `target_compile_definitions`.
3. Add a new embedded example/build profile to document usage.

### Phase 1 Status

- Added explicit embedded mode CMake options in `CMakeLists.txt`.
- Added `BENCODE_EMBEDDED_MODE`, `BENCODE_ENABLE_EXCEPTIONS`, `BENCODE_ENABLE_FILE_IO`, and `BENCODE_ENABLE_DYNAMIC_ALLOCATION`.
- Exposed embedded configuration flags to consumers via `target_compile_definitions`.
- Verified `Bencode_Lib` still builds successfully after the new embedded-mode configuration options were added.

### Phase 2: Separate core functionality from optional components

1. Split public header inclusion in `Bencode_Core.hpp` so embedded mode only includes core types and buffer-based I/O.
2. Make file-based I/O optional via `BENCODE_ENABLE_FILE_IO`.
3. Make extra stringifier headers optional with feature macros.
4. Add new core-only API documentation for embedded mode: parse from buffer, stringify to buffer, root only.

### Phase 2 Status

- Added `BENCODE_EMBEDDED_MODE`, `BENCODE_ENABLE_EXCEPTIONS`, `BENCODE_ENABLE_FILE_IO`, and `BENCODE_ENABLE_DYNAMIC_ALLOCATION` as CMake options.
- Guarded `Bencode_Sources.hpp` and `Bencode_Destinations.hpp` so file classes are only included when `BENCODE_ENABLE_FILE_IO` is enabled.
- Wrapped `Bencode` and `Bencode_Impl` file-based API declarations in `#if defined(BENCODE_ENABLE_FILE_IO)`.
- Conditionally included `Bencode_File.cpp` in the main library sources only when file I/O is enabled.
- Added compile-time guards to `Bencode_FileSource.hpp` and `Bencode_FileDestination.hpp`.

### Phase 3: Replace dynamic runtime memory with bounded or arena-based storage

### Phase 3 Status

- Added embedded compile-time capacity macros in `CMakeLists.txt`: `BENCODE_MAX_NODE_COUNT`, `BENCODE_MAX_CONTAINER_SIZE`, and `BENCODE_MAX_STRING_LENGTH`.
- Added `classes/include/implementation/variants/Bencode_FixedVector.hpp` and used it to implement bounded list and dictionary storage when `BENCODE_ENABLE_DYNAMIC_ALLOCATION` is disabled.
- Added `BENCODE_MAX_STRING_LENGTH` support to `classes/include/implementation/variants/Bencode_String.hpp`.
- Verified the library builds successfully after the bounded-container changes.
- Remaining phase 3 work: inline node storage and allocator abstractions are still pending.

1. Refactor `Node` to use an inline tagged union instead of `std::unique_ptr<Variant>`.
   - Use a custom variant-like type with explicit storage, or `std::variant` only in non-embedded mode.
2. Introduce an `Allocator` abstraction for embedded mode.
   - Default allocator uses static arena or preallocated block.
   - Optional fallback allocator uses heap.
3. Refactor `String` to support small-string storage or external view semantics.
   - In embedded mode, support fixed-length string storage with a templated capacity.
4. Refactor `List` and `Dictionary` to support capacity-bounded containers.
   - Use a custom `FixedVector<Node, N>` type in embedded builds.
   - Keep current dynamic `std::vector` only when dynamic allocation is enabled.
5. Refactor `BufferSource` to avoid copying from `std::string_view` in embedded mode.
   - Provide a zero-copy view-based source implementation.
6. Refactor `BufferDestination` to support user-provided output buffer with reserved capacity.
   - Provide a `BufferDestinationStatic` or `SimpleOutputBuffer` in embedded mode.

### Phase 4: Remove exceptions and use error codes/status objects

1. Provide a lower-level `Result<T, ErrorCode>` or `ParseStatus` return type for embedded parsing.
2. Make exceptions optional via `BENCODE_ENABLE_EXCEPTIONS`:
   - If enabled: keep existing `throw` semantics.
   - If disabled: use status returns and `Bencode::lastError()` or output parameter.
3. Replace `throw Error(...)` and `throw SyntaxError(...)` with macros that map to `return ErrorCode` in embedded mode.
4. Maintain a compatibility layer so high-level API still works in normal builds.

### Phase 5: Add embedded parser/stringifier optimizations

1. Remove `std::function`+`std::map` dispatch from `Default_Parser` in embedded mode.
   - Keep switch dispatch or lookup table.
2. Avoid temporary `std::string` allocations in `parseString()`.
   - Read string data into the final storage directly or into a preallocated buffer.
3. Avoid `std::to_string` in stringification when embedded mode is enabled.
   - Use integer-to-ascii helpers that write into a destination buffer.
4. Provide a non-recursive parse option or explicit max-depth stack to reduce stack usage.

### Phase 6: Make File I/O optional and portable

1. Wrap `FileSource` and `FileDestination` behind `BENCODE_ENABLE_FILE_IO`.
2. Replace `<fstream>` usage with a pluggable file abstraction in embedded mode:
   - `EmbeddedFileSource` / `EmbeddedFileDestination`
   - Build on `fopen`/`fread`/`fwrite` or custom HAL APIs.
3. Provide `BufferSource` and `BufferDestination` as the default embedded I/O path.

### Phase 7: Add embedded tests and benchmarks

1. Add a new embedded-target test suite or compile-time test configuration.
2. Include tests for fixed-capacity node/list/dictionary behavior.
3. Add benchmarks for heap-free parsing and stringification.
4. Verify low-memory behavior and no-heap correctness via tests.

### Phase 8: Document and validate

1. Add an `EMBEDDED.md` guide describing:
   - supported feature macros
   - required configuration for no-exceptions builds
   - supported input/output types
   - memory budget settings
2. Add a sample embedded `CMakeLists.txt` or `platform` example.
3. Run validation on a representative embedded toolchain or cross-compile target.
4. Measure:
   - binary size
   - heap allocations (ideally zero)
   - stack depth
   - parse/stringify throughput on target hardware

## Suggested Embedded-Friendly API Changes

- `Bencode::parse(ISource &source)` -> `Bencode::parse(const BufferSource &source, ParseResult &result)` in embedded mode.
- `Bencode::stringify(IDestination &destination)` -> `Bencode::stringify(SimpleBufferDestination &destination)`.
- `BufferSource` should accept a raw `const char *`/pointer and length and avoid copying.
- `BufferDestination` should accept a preallocated buffer and return size used.
- Add `Bencode::setAllocator(IAllocator*)` or template parameters for memory backing.

## Immediate Implementation Priorities

1. Add embedded configuration macros in `CMakeLists.txt`.
2. Create an embedded core subset of headers and disable file/stringifier extras.
3. Refactor `Node` to inline variant storage and add a bounded container mode.
4. Replace exceptions with optional status returns for embedded builds.
5. Add minimal `BufferSource`/`BufferDestination` implementations that avoid allocations.
6. Validate with a cross-compile build and a small-footprint example.

## Summary

The current library is not embedded-ready because it relies on heap allocation, exceptions, `std::ifstream`, and flexible STL containers. The concrete plan is to add an explicit embedded mode, provide bounded memory containers, disable exceptions and file I/O by default, and supply a minimal buffer-only API with optional custom allocators.
