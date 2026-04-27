# Bencode_Lib Size and Performance Optimization Roadmap

## Objective

Create a practical, implementation-first roadmap to reduce binary size and improve runtime performance across the core Bencode library.

## Current state

### Architecture observations

- `classes/include/implementation/node/Bencode_Node.hpp`
  - `Node` is currently backed by `std::unique_ptr<Variant>`.
  - This means every non-scalar node is a separate heap allocation, which hurts memory usage and cache locality.

- `classes/include/implementation/variants/Bencode_Dictionary.hpp`
  - Dictionary entries store `std::string key` and `Node bNode`.
  - With dynamic allocation enabled, storage is `std::vector<Entry>`.
  - Dictionary insertion is sorted, but the current design still allocates per entry and keeps string keys in heap-backed `std::string` objects.

- `classes/include/implementation/variants/Bencode_String.hpp`
  - String content is stored through a custom wrapper.
  - Parsing still performs extra allocations and copies for string values.

- `classes/source/implementation/parser/Default_Parser.cpp`
  - Parser dispatch currently routes through function objects and may allocate temporary nodes and strings during parse.
  - Dictionary and list parse paths build nodes recursively, which is functionally correct but not optimal for allocation pressure.

- `CMakeLists.txt`
  - Good feature-gating foundation exists already:
    - `BENCODE_EMBEDDED_MODE`
    - `BENCODE_ENABLE_EXCEPTIONS`
    - `BENCODE_ENABLE_FILE_IO`
    - `BENCODE_ENABLE_DYNAMIC_ALLOCATION`
    - `BENCODE_ENABLE_JSON_STRINGIFY`
    - `BENCODE_ENABLE_XML_STRINGIFY`
    - `BENCODE_ENABLE_YAML_STRINGIFY`
  - The library already exposes a separate embedded variant with minimal allocation and I/O support.

### Benchmark status

- Benchmark harness exists in `tests/source/benchmark/Bencode_Lib_Benchmark.cpp`.
- Benchmark targets are registered in `tests/CMakeLists.txt`.
- This provides a measurement path for parse/stringify throughput and regression tracking.

## Primary performance and size issues

1. `Node` heap indirection for variant storage.
2. Dynamic string allocation for Bencode data and dictionary keys.
3. Container overhead for lists and dictionaries.
4. Optional serialization and file I/O included by default rather than being trimmed for minimal builds.
5. Parser uses temporary objects and dynamic dispatch in hot paths.

## Concrete optimization roadmap

### Phase 1: Benchmark and baseline

Actions:
- Build the library in Release mode and note binary sizes.
- Run the benchmark harness against representative inputs.
- Capture current parse and stringify throughput, and note any allocation spikes.
- Record the active CMake configuration.

Why:
- A strong baseline is required to validate every later change.

Baseline results:
- Release artifacts present and measured on disk:
  - `build/Release/Bencode_Lib.lib` = 353,456 bytes
  - `build/Release/Bencode_Lib_Embedded.lib` = 309,614 bytes
- Current active build artifacts:
  - `build/Debug/Bencode_Lib.lib` = 3,411,684 bytes
  - `build/Debug/Bencode_Lib_Embedded.lib` = 2,751,620 bytes
- Active CMake flags:
  - `BENCODE_BUILD_BENCHMARKS=ON`
  - `BENCODE_BUILD_EXAMPLES=ON`
  - `BENCODE_BUILD_TESTS=ON`
  - `BENCODE_EMBEDDED_MODE=OFF`
  - `BENCODE_ENABLE_DYNAMIC_ALLOCATION=ON`
  - `BENCODE_ENABLE_EXCEPTIONS=ON`
  - `BENCODE_ENABLE_FILE_IO=ON`
  - `BENCODE_ENABLE_JSON_STRINGIFY=ON`
  - `BENCODE_ENABLE_XML_STRINGIFY=ON`
  - `BENCODE_ENABLE_YAML_STRINGIFY=ON`
  - `BENCODE_MAX_CONTAINER_SIZE=64`
  - `BENCODE_MAX_NODE_COUNT=256`
  - `BENCODE_MAX_STRING_LENGTH=16384`
- Validation results:
  - `Bencode_Lib_Unit_Tests` passed
  - `Bencode_Lib_Embedded_Unit_Tests` passed
  - `Bencode_Lib_Benchmark` passed
  - `Bencode_Lib_Embedded_Benchmark` passed
- Benchmark results:
  - `Bencode_Lib_Benchmark`: parse = 7.16849 MB/s, stringify = 14.0291 MB/s
  - `Bencode_Lib_Embedded_Benchmark`: parse = 7.52445 MB/s, stringify = 14.0553 MB/s

### Phase 2: Inline node storage

Actions:
- Refactor `Node` so variant data is stored inline rather than through `std::unique_ptr<Variant>`.
- Use a tagged union or direct inline storage for scalars, strings, lists, and dictionaries.
- Keep `List` and `Dictionary` as separate container types, but avoid per-node heap wrappers.

Why:
- Eliminates one heap allocation per node.
- Improves traversal speed and reduces fragmentation.

### Phase 3: Compact string handling

Actions:
- Simplify `Bencode_String` storage to avoid manual heap management.
- Parse strings directly into their final storage buffer.
- Use small-string optimization or reserve capacity before reading raw bytes.
- Maintain embedded limits via `BENCODE_MAX_STRING_LENGTH`.

Status:
- Added direct char-buffer assignment support to `String`.
- Added parser helper `copySourceToBuffer()` to centralize string byte copying.
- Reduced duplication in exception and no-exception string parse paths.
- Verified with full unit tests and benchmark suite.

Why:
- Reduces string copy and allocation overhead.
- Improves parse/stringify throughput for common string sizes.

### Phase 4: Lean dictionary storage and lookup

Actions:
- Keep dictionary entries sorted, but avoid per-entry wrapper overhead.
- Use `std::vector<Entry>` or `FixedVector<Entry, N>` with binary search for lookup.
- Remove any full-vector resorting on insert when possible.
- Store keys as `std::string` directly and keep values as `Node`.

Why:
- Lowers dictionary memory overhead.
- Preserves deterministic ordering and fast lookup.

### Phase 5: Trim optional features for minimal builds

Actions:
- Make JSON/XML/YAML stringify support optional via feature macros.
- Keep file I/O behind `BENCODE_ENABLE_FILE_IO`.
- Ensure `Bencode.hpp` and `Bencode_Core.hpp` only expose necessary headers.
- Build a minimal consumer profile with just core parse/stringify and buffer-based I/O.

Why:
- Shrinks binary size for embedded/minimal use cases.
- Reduces compile-time dependencies.

### Phase 6: Harden embedded/no-exceptions mode

Actions:
- Confirm `BENCODE_EMBEDDED_MODE` compiles with `BENCODE_ENABLE_DYNAMIC_ALLOCATION=0`, `BENCODE_ENABLE_EXCEPTIONS=0`, and `BENCODE_ENABLE_FILE_IO=0`.
- Add tests for fixed-capacity containers and parser `ParseStatus` return behavior.
- Ensure embedded benchmark inputs honor `BENCODE_MAX_CONTAINER_SIZE`.

Why:
- Validates the low-footprint build path.
- Ensures minimal builds remain stable and predictable.

### Phase 7: Regression benchmark harness

Status: Implemented.

Actions:
- Use the existing benchmark executable as the regression harness.
- Add CTest entries so benchmarks can be invoked consistently.
- Track metrics for:
  - parse MB/s
  - stringify MB/s
  - binary size
  - allocation behavior
- Document benchmark results in the repository.
- Support `roundtrip`, `parse`, and `stringify` benchmark modes for targeted regression measurement.

Why:
- Provides objective feedback for each refactor.
- Enables size/performance regression detection.

## Recommended implementation targets

- `classes/include/implementation/node/Bencode_Node.hpp`
- `classes/include/implementation/variants/Bencode_String.hpp`
- `classes/include/implementation/variants/Bencode_Dictionary.hpp`
- `classes/source/implementation/parser/Default_Parser.cpp`
- `classes/include/implementation/io/Bencode_Sources.hpp`
- `classes/include/implementation/io/Bencode_Destinations.hpp`
- `CMakeLists.txt`
- `tests/CMakeLists.txt`

## Expected outcomes

- Significantly reduced heap allocations for parsed data.
- Faster parse and stringify paths for both dynamic and embedded builds.
- Smaller binary size by excluding non-essential features.
- A cleaner minimal build configuration for embedded users.
- Repeatable performance metrics for future refactors.

## Next step

Start by refactoring `Node` storage and then measure the impact with the benchmark harness. After that, optimize string handling and dictionary storage in a second pass.
