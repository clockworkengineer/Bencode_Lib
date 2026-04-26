# Bencode_Lib Size and Performance Optimization Plan

## Purpose

Analyze the current library architecture and provide a concrete, implementation-first plan to reduce binary size and increase runtime performance. This file is intended as a roadmap for engineering changes in the core library, parser, and build configuration.

## Current Analysis

### Key implementation findings

- `classes/include/implementation/node/Bencode_Node.hpp`
  - `Node` currently stores a `std::unique_ptr<Variant>`.
  - Every node is a separate heap allocation, causing high memory overhead and poor cache locality.

- `classes/include/implementation/variants/Bencode_Dictionary.hpp`
  - `DictionaryEntry` stores `std::string key` and `Node bNode`.
  - Dictionary storage is a `std::vector<Entry>` when dynamic allocation is enabled.
  - `Dictionary::add()` already uses `std::lower_bound`, but the design still allocates each entry and uses `std::string` for every key.

- `classes/include/implementation/variants/Bencode_String.hpp`
  - `String` stores `std::string` directly and allows a configurable max length.
  - Parsing and destination code still perform string allocations and copies.

- `classes/source/implementation/parser/Default_Parser.cpp`
  - Parser logic is mostly `switch`-based, which is good, but it still allocates a fresh `Node` on each parse and often assembles strings via temporary allocations.
  - The parse path for dictionary keys and list values uses recursive node construction.

- `CMakeLists.txt`
  - Already includes useful build options:
    - `BENCODE_EMBEDDED_MODE`
    - `BENCODE_ENABLE_EXCEPTIONS`
    - `BENCODE_ENABLE_FILE_IO`
    - `BENCODE_ENABLE_DYNAMIC_ALLOCATION`
    - optional stringify flags for JSON / XML / YAML
  - This is a strong foundation for size/performance tuning via build-time feature gating.

## Primary Performance & Size Problems

1. Heap allocation per node via `std::unique_ptr<Variant>`.
2. `std::string` for every parsed Bencode string and dictionary key.
3. Per-node variant indirection and dynamic allocation for lists/dictionaries.
4. Runtime exception-heavy control flow in parser and node access.
5. Public API includes optional stringifiers and file I/O by default, increasing compile/binary footprint.
6. No current benchmark harness to quantify performance regressions or improvements.

## Concrete Optimization Strategy

### Phase 1: Measure and baseline

1. Build `Release` and measure library size and binary artifacts.
2. Record current runtime performance for representative parse/stringify workloads.
3. Capture memory-allocation behavior for deeply nested Bencode input.
4. Document current `CMake` configuration values and test coverage.

### Phase 1 Baseline Results

- Existing Release artifact: `build/Release/Bencode_Lib.lib` = 353,456 bytes
- Existing Release embedded artifact: `build/Release/Bencode_Lib_Embedded.lib` = 309,614 bytes
- Existing Debug artifact: `build/Debug/Bencode_Lib.lib` = 2,408,240 bytes
- Existing Debug embedded artifact: `build/Debug/Bencode_Lib_Embedded.lib` = 1,749,270 bytes

- Current CMake options from `build/CMakeCache.txt`:
  - `BENCODE_EMBEDDED_MODE=OFF`
  - `BENCODE_ENABLE_EXCEPTIONS=ON`
  - `BENCODE_ENABLE_FILE_IO=ON`
  - `BENCODE_ENABLE_DYNAMIC_ALLOCATION=ON`
  - `BENCODE_ENABLE_JSON_STRINGIFY=ON`
  - `BENCODE_ENABLE_XML_STRINGIFY=ON`
  - `BENCODE_ENABLE_YAML_STRINGIFY=ON`
  - `BENCODE_BUILD_TESTS=ON`
  - `BENCODE_BUILD_EXAMPLES=ON`
  - `BENCODE_BUILD_BENCHMARKS=ON`

- Current status:
  - Build configuration was fixed by defining the `Bencode_Lib_Embedded` target and excluding file I/O sources from the embedded variant.
  - Release build completed successfully and produced baseline artifacts for both the main and embedded libraries.

### Phase 2: Inline Node storage

Objective: eliminate the per-node heap allocation.

### Phase 2 Status

- Implemented a new `Node` storage strategy where scalar variants are stored inline.
- List and dictionary nodes now use `std::unique_ptr<List>` / `std::unique_ptr<Dictionary>` in the `Node` variant, preserving container semantics while removing per-node `Variant` allocation.
- Rebuilt `Bencode_Lib` and `Bencode_Lib_Unit_Tests` successfully in Debug.

### Phase 3 Status

- Added a compact `String` variant with inline storage for short strings and heap fallback for larger strings.
- Updated parser string handling to allocate string storage once and write bytes directly into `String` payload.
- Eliminated temporary parse-buffer copies for Bencode string values.

### Phase 4 Status

- Simplified dictionary entry storage by moving key and node into a lightweight `DictionaryEntry` struct.
- Added direct dictionary key parsing to avoid temporary `Node` allocations during parse.
- Preserved sorted-key lookup while reducing dictionary parse and insertion overhead.

1. Replace `Node::bNodeVariant` from `std::unique_ptr<Variant>` to an inline tagged union.
2. Example structure:
   - `enum class Type { Empty, Integer, String, List, Dictionary, Hole }`.
   - `union Storage { Integer integer; String string; List list; Dictionary dictionary; }`.
3. Keep the current public API but move storage into `Node` itself.
4. Ensure move semantics remain efficient and node copy is either deleted or explicitly handled.
5. Maintain `BENCODE_ENABLE_DYNAMIC_ALLOCATION` compatibility by using the same object model but with fewer allocations.

Expected benefit:
- Remove one heap allocation per node.
- Greatly improve traversal speed.
- Reduce total heap usage and fragmentation.

### Phase 3: Optimize string storage and parsing

1. Replace custom string parsing patterns with direct buffer allocation.
2. On parse, reserve output capacity once based on parsed length.
3. Avoid temporary `std::string` allocations when reading bytes from `ISource`.
4. If `BENCODE_ENABLE_DYNAMIC_ALLOCATION` is disabled, support fixed-size buffer or small-string optimization.
5. Consider a `String` specialization that stores small data inline up to a threshold.

Expected benefit:
- Reduce parse-time allocations and copies.
- Improve string encoding/decoding throughput.
- Lower binary size by removing unnecessary wrapper code.

### Phase 4: Optimize dictionary storage and lookup

1. Keep sorted dictionary entries but reduce overhead by storing keys as `std::string` directly without extra wrapper objects.
2. Use a stable `std::vector<Entry>` or `FixedVector<Entry, N>` for embedded mode.
3. Ensure binary-search lookup remains efficient.
4. Replace `DictionaryEntry` if possible with a lighter `struct { std::string key; Node node; }` design that avoids extra abstraction overhead.
5. For large dictionaries, consider `std::map<std::string, Node>` only if it is measurably better than `std::vector` for current use cases; otherwise, a sorted vector is usually best for compact builds.

Expected benefit:
- Lower dictionary access cost.
- Reduce memory usage per dictionary node.
- Preserve deterministic ordering while improving parse and lookup speed.

### Phase 5: Eliminate optional heavyweight features from minimal builds

1. Use existing CMake options to gate optional features.
2. In minimal builds, disable:
   - `BENCODE_ENABLE_FILE_IO`
   - `BENCODE_ENABLE_JSON_STRINGIFY`
   - `BENCODE_ENABLE_XML_STRINGIFY`
   - `BENCODE_ENABLE_YAML_STRINGIFY`
3. Keep `Bencode.hpp` and `Bencode_Core.hpp` slim by conditionally including optional headers only when enabled.
4. Prefer a small public include surface for the minimal library.

Expected benefit:
- Reduced compile times.
- Smaller installed headers and binary size.
- Cleaner embedded/minimal consumer experience.

### Phase 6: Improve embedded mode and no-exceptions path

1. Verify `BENCODE_EMBEDDED_MODE` is actively used to reduce dynamic allocation and I/O features.
2. When `BENCODE_ENABLE_EXCEPTIONS` is OFF, ensure the parser returns `ParseStatus` instead of throwing.
3. Optimize embedded object lifetimes and move semantics for low memory.
4. Add tests for no-exceptions and bounded allocation mode.

Expected benefit:
- Smaller code size for constrained targets.
- Reliable performance with predictable resource use.

### Phase 7: Add performance regression harness

1. Add a benchmark target under `tests/` or a dedicated `benchmark/` directory.
2. Measure:
   - parse throughput (MB/s)
   - stringify throughput (MB/s)
   - heap allocations / allocation count
   - binary size changes
3. Run benchmarks before and after each major refactor.
4. Store results as part of the repository documentation.

Expected benefit:
- Objective validation of improvements.
- Faster detection of regressions.

## Recommended implementation tasks

1. Add or extend a benchmark executable in `tests`.
2. Refactor `classes/include/implementation/node/Bencode_Node.hpp` to inline variant storage.
3. Refactor `classes/include/implementation/variants/Bencode_String.hpp` and parser string paths for direct storage.
4. Refactor `classes/include/implementation/variants/Bencode_Dictionary.hpp` for lighter entries and stable sorted insertion.
5. Audit `classes/include/implementation/io/` headers and source to separate mandatory core I/O from optional file I/O.
6. Add compile-time feature gating around optional stringifiers in `CMakeLists.txt` and header inclusion.
7. Add a minimal `BufferDestination`/`BufferSource` mode for no-heap embedded usage.

## Concrete expected outcomes

- Node heap allocations reduced by approximately 100% of all node objects.
- Parse throughput improved by 20–40% for nested structures.
- Dictionary lookups optimized from linear-like behavior to binary-search behavior with lower overhead.
- Library size lowered by removing unused optional features and reducing dynamic allocation support in embedded mode.
- Cleaner minimal build configuration for consumer use.

## File targets for implementation

- `classes/include/implementation/node/Bencode_Node.hpp`
- `classes/include/implementation/variants/Bencode_String.hpp`
- `classes/include/implementation/variants/Bencode_Dictionary.hpp`
- `classes/source/implementation/parser/Default_Parser.cpp`
- `classes/include/implementation/io/Bencode_FileSource.hpp`
- `classes/include/implementation/io/Bencode_FileDestination.hpp`
- `CMakeLists.txt`
- `tests/CMakeLists.txt`

## Next step

Use this plan as a base for the first refactor: inline `Node` storage and benchmark the performance delta. After that, proceed to string and dictionary optimizations in measured order.
