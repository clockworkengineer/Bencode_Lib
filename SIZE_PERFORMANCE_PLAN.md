# Bencode_Lib Size and Performance Optimization Plan

## Purpose

Provide a concrete, repository-specific roadmap to reduce binary size and improve runtime performance for `Bencode_Lib`. This plan maps analysis to exact source files, build options, and refactor steps.

## Current repository analysis

### High-impact findings

- `classes/include/implementation/node/Bencode_Node.hpp`
  - `Node` stores `std::variant<std::monostate, Hole, Integer, String, std::unique_ptr<List>, std::unique_ptr<Dictionary>>`.
  - List and dictionary nodes are heap-allocated, so every container node adds an extra allocation and pointer indirection.

- `classes/include/implementation/variants/Bencode_Dictionary.hpp`
  - Dictionary entries are `std::string key` + `Node bNode`.
  - Parsing currently constructs keys then checks for duplicates with `contains()` before inserting.
  - `Dictionary::add()` already uses binary-search insertion, but parse flow still performs extra work.

- `classes/include/implementation/variants/Bencode_String.hpp`
  - `String` is a custom small-string wrapper with inline storage and heap fallback.
  - It is moderately optimized, but the wrapper itself is complex and may inflate binary size.

- `classes/source/implementation/parser/Default_Parser.cpp`
  - The parser uses an explicit `ParserFrame` stack and string key buffering.
  - `parseStringKey()` and the dictionary parse path allocate and move keys multiple times.

- `CMakeLists.txt`
  - Default library build enables optional file I/O and JSON/XML/YAML stringifiers.
  - A minimal variant exists, but the main build still includes extra features by default.

### Good existing foundations

- Build-time feature gating already exists for file I/O, dynamic allocation, and optional serializers.
- Minimal and embedded library targets are already defined in `CMakeLists.txt`.
- `Dictionary` lookup already uses `std::lower_bound` and sorted key semantics.
- Parser supports both throwing and non-throwing paths.

## Primary targets for reduction and speedup

1. Eliminate heap allocation indirection in `Node` for list and dictionary storage.
2. Simplify string wrapper and ensure parser writes directly into destination buffers.
3. Tighten dictionary parse insertion and avoid duplicate or redundant key checks.
4. Reduce default build footprint by clearly separating core, file I/O, and optional serializers.
5. Add measurable benchmark regression tracking.

## Concrete engineering roadmap

### Phase 1: Baseline measurement

Tasks:
- Build `Release` and record sizes of:
  - `build/Release/Bencode_Lib.lib`
  - `build/Release/Bencode_Lib_Embedded.lib`
  - `build/Release/Bencode_Lib_Minimal.lib` (if built)
- Capture default feature settings from `CMakeLists.txt`.
- Run the existing unit test suite.
- Add or extend the benchmark harness in `tests/source/benchmark/Bencode_Lib_Benchmark.cpp`.
- Measure representative workloads for parse/stringify, including a large dictionary and large string payload.

Deliverables:
- Exact baseline binary sizes.
- Baseline parse and stringify throughput values.
- Baseline functional test results.

Baseline results:
- Release build artifacts:
  - `build/Release/Bencode_Lib.lib` = 345.17 KB
  - `build/Release/Bencode_Lib_Embedded.lib` = 302.36 KB
- Debug build artifacts:
  - `build/Debug/Bencode_Lib.lib` = 3329.23 KB
  - `build/Debug/Bencode_Lib_Embedded.lib` = 2687.21 KB
  - `build/Debug/Bencode_Lib_Minimal.lib` = 3058.10 KB
- Benchmark throughput (`Bencode_Lib_Benchmark`):
  - Parse = 6.31829 MB/s
  - Stringify = 13.241 MB/s
- Validation results:
  - `Bencode_Lib_Unit_Tests` passed
  - `Bencode_Lib_Minimal_Unit_Tests` passed
  - `Bencode_Lib_Embedded_Unit_Tests` passed
  - `Bencode_Lib_Benchmark` passed
  - `Bencode_Lib_Embedded_Benchmark` passed

### Phase 2: Inline node container storage

Goal: remove per-node heap allocations for `List` and `Dictionary` while keeping the current `Node`-based container semantics.

Tasks:
- Audit `classes/include/implementation/node/Bencode_Node.hpp` and its helpers in `Bencode_Node_Creation.hpp` / `Bencode_Node_Reference.hpp`.
- Plan the refactor in two substeps:
  1. Break the current `Node`/`List`/`Dictionary` initialization dependency so `Node` can store `List` and `Dictionary` directly instead of via `std::unique_ptr`.
  2. Update `Node::Storage` to use inline `List`/`Dictionary` variants or a custom in-place container payload.
- Replace pointer-based container variants in `Node` with direct container objects.
- Simplify `Node::make<T>` and raw value constructors so container construction uses inline storage.
- Remove `dereferenceList()` / `dereferenceDictionary()` pointer helpers once direct storage is in place.
- Preserve `List::add()`, `Dictionary::add()`, `appendSorted()`, and `operator[]` semantics after the refactor.
- Add unit tests for:
  - nested list/dictionary construction and traversal,
  - move-only `Node` behavior,
  - dictionary ordered insertion and duplicate detection.

Implementation notes:
- `List` and `Dictionary` currently hold `Node` values by value, which introduces a recursive type dependency that makes direct inline `List`/`Dictionary` storage in `Node` infeasible without a deeper redesign.
- The practical Phase 2 implementation should therefore begin with a feasibility assessment for two paths:
  1. redesigning container entry storage so `Node` can own inline container payloads, or
  2. introducing an arena/pool for `List` and `Dictionary` objects to eliminate per-node heap allocations while preserving existing container-value semantics.
- Phase 2 now implements the pooled allocator path: `List` and `Dictionary` nodes are allocated from a thread-local pool and recycled when destroyed.
- Avoid committing to direct inline container storage until the mutual recursion issue is resolved in the type architecture.

Expected impact:
- Remove one heap allocation per list/dictionary container node object, or eliminate the allocation overhead with a pooled allocator.
- Improve cache locality and runtime traversal speed for container-heavy parse paths.
- Reduce pointer indirection in parse and stringify hot paths.

### Phase 3: Simplify string storage and parse input handling

Goal: keep strings efficient while reducing code weight, copies, and allocation overhead.

Tasks:
- Audit `classes/include/implementation/variants/Bencode_String.hpp` and its public API.
  - Confirm current wrapper size, heap fallback threshold, and embedded-friendly behavior.
  - Identify where the wrapper can be simplified to a `std::string`-backed representation or a smaller custom inline buffer.
- Define a target string storage design:
  - option A: use `std::string` with small-string optimization for general use,
  - option B: keep the current wrapper but reduce its heap fallback complexity.
- Update the parser string path in `classes/source/implementation/parser/Default_Parser.cpp`:
  - parse the string length first,
  - allocate the final string size once,
  - read raw bytes directly into the destination buffer,
  - for dictionary keys, reserve and fill the key buffer directly rather than using temporary strings.
- Consolidate exception and no-exception parse flows so the string path shares the same allocation logic.
- Add or update unit tests to ensure:
  - strings parse correctly with exact length allocation,
  - no intermediate string copies occur in the parser path,
  - embedded mode still honors `BENCODE_MAX_STRING_LENGTH`.
- Add a benchmark workload to validate string parse/stringify performance and allocation behavior.

Status:
- Implemented string wrapper simplification to use `std::string` with built-in SSO.
- Parser already uses direct `resize()` + `data()` writes for both string values and dictionary keys.
- Verified `Bencode_Lib` build and `Bencode_Lib_Unit_Tests`.

Implementation notes:
- If using `std::string`, prefer `resize()` + direct `data()` writes over incremental append.
- For the custom wrapper, expose a `reserveForParse(size_t length)` helper and a `data()` pointer for parser fills.
- Keep the string node API compatible with existing `Bencode` consumers.

Expected impact:
- Maintain or improve parse speed for strings.
- Reduce parser temporary allocations and copy overhead.
- Simplify wrapper code and reduce binary size from fewer allocation helpers.
- Preserve embedded and no-exceptions build correctness.

### Phase 4: Tighten dictionary parsing and lookup

Goal: improve dictionary parse efficiency and reduce redundant checks.

Status: Implemented.

Tasks:
- Update `classes/source/implementation/parser/Default_Parser.cpp`:
  - In `parseDictionary()`, parse the key once and append it in-order instead of using `contains()` then `add()`.
  - Use `Dictionary::appendSorted()` for parser-ordered dictionary construction and fail fast on out-of-order keys.
- In `parseIterative()`, use `appendSorted()` for dictionary entry insertion so nested dictionary parsing also avoids redundant lookup.
- Keep `Dictionary::add()` for general insertion but ensure parser path uses the most efficient path.
- Validate `Dictionary::contains()` and `operator[]` are using binary search efficiently.

Expected impact:
- Avoid repeated key comparisons during dictionary parsing.
- Preserve sorted ordering with lower insertion overhead.

### Phase 5: Narrow default build footprint

Goal: make small-size builds easier and default core builds leaner.

Status: Implemented.

Tasks:
- In `CMakeLists.txt`, clearly document the purpose of `BENCODE_BUILD_MINIMAL` and `BENCODE_LIB_Embedded`.
- Ensure `classes/include/implementation/io/Bencode_FileSource.hpp` and `Bencode_FileDestination.hpp` are only included when `BENCODE_ENABLE_FILE_IO=ON`.
- Confirm `Bencode_Core.hpp` only includes optional stringify headers if the respective build flags are enabled.
- Optionally set `BENCODE_BUILD_MINIMAL=ON` for a dedicated small build profile in documentation.

Expected impact:
- Smaller binary size in the minimal target.
- Cleaner public header surface.
- Easier consumer use for embedded/limited targets.

### Phase 6: Optimize parser structure and control flow

Goal: reduce parser overhead while preserving correctness.

Tasks:
- Review `classes/source/implementation/parser/Default_Parser.cpp` for opportunities to reduce temporary allocations in `ParserFrame` and `std::string` key handling.
- Confirm `copySourceToBuffer()` is the only byte-copy path for strings.
- Where possible, reuse parser memory for repeated frames or keys without reallocation.
- Keep `parseIterative()` logic simple and avoid unnecessary intermediate nodes.

Expected impact:
- Faster parsing for deeply nested structures.
- Reduced temporary allocation overhead.

### Phase 7: Add regression benchmarks and measurement

Goal: make size and speed improvements quantifiable and repeatable.

Tasks:
- Extend `tests/source/benchmark/Bencode_Lib_Benchmark.cpp` with feature-specific workloads.
- Add a benchmark target to `tests/CMakeLists.txt` if needed.
- Track metrics for:
  - parse MB/s
  - stringify MB/s
  - binary size change
  - optionally heap allocation count
- Save benchmark results in a repository note or doc.
- Add baseline reference output so the benchmark harness reports current throughput against known baseline numbers.
- Add allocation-count statistics for pool-backed `List` and `Dictionary` objects.
- Current pooled allocator benchmark results: parse ~6.67 MB/s, stringify ~12.90 MB/s for the standard 5000-entry dictionary payload.

Expected impact:
- Objective validation of each refactor.
- Early detection of regressions.

## Concrete implementation tasks

1. Refactor `classes/include/implementation/node/Bencode_Node.hpp` to inline container storage.
2. Audit and simplify `classes/include/implementation/variants/Bencode_String.hpp`.
3. Refactor `classes/include/implementation/variants/Bencode_Dictionary.hpp` for parser-friendly insertion.
4. Optimize `classes/source/implementation/parser/Default_Parser.cpp` dictionary and string parse paths.
5. Harden `CMakeLists.txt` feature gates and minimal/embedded build profiles.
6. Keep file I/O headers in `classes/include/implementation/io/` optional behind `BENCODE_ENABLE_FILE_IO`.
7. Strengthen benchmark support in `tests/CMakeLists.txt` and `tests/source/benchmark/Bencode_Lib_Benchmark.cpp`.

## Success criteria

- Release binary size shrinks measurably, especially for minimal/embedded builds.
- Library keeps the same public API and test coverage.
- Parse/stringify throughput improves from the baseline.
- `BENCODE_ENABLE_FILE_IO=OFF` and `BENCODE_ENABLE_DYNAMIC_ALLOCATION=OFF` builds still compile.
- Benchmark harness captures regressions going forward.

## Immediate next step

Start with `Bencode_Node.hpp` refactor and benchmark baseline results. Then implement dictionary parse flow improvements and tighten build gating.
