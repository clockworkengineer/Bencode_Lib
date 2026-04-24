# Bencode_Lib Size and Performance Optimization Plan

## Objectives

- Reduce library binary and memory footprint.
- Improve runtime performance for parsing, node access, and serialization.
- Preserve compatibility with existing public API while enabling safer, smaller data structures.

## Key Findings

1. `Node` is backed by `std::unique_ptr<Variant>`.
   - Every node allocation is a separate heap allocation.
   - This creates high memory overhead and poor locality for nested structures.

2. `Dictionary` uses `std::vector<DictionaryEntry>`.
   - Every insert sorts the entire dictionary via `std::ranges::sort`.
   - Lookup is linear and expensive, especially when parsing large dictionaries.
   - Keys are stored as full `Node` objects wrapped in heap-allocated `String` nodes.

3. `String` storage uses a custom `BencodeString` wrapper.
   - It allocates raw memory via `malloc` and wraps it in `std::unique_ptr<char[]>`.
   - This is both error-prone and slower than `std::string` or a small-string optimization.

4. The parser dispatch uses `std::function<Node(ISource&, unsigned long)>` and `std::map<char, ParserFunc>`.
   - This adds dynamic dispatch cost on every parsed node.
   - The parser also copies every string into a temporary `std::string` before constructing the Bencode string node.

5. Public API uses `std::map<std::string, Node>` semantics in documentation, but actual implementation is custom vector dictionary.
   - This discrepancy increases maintenance risk and can confuse consumers.

## Concrete Optimization Recommendations

### 1. Replace `std::unique_ptr<Variant>` with an inline or tagged union representation

- Use a single `struct Node` containing a `Variant` discriminated union instead of heap-allocating each variant.
- Example representation:
  - `enum class Type { Empty, Integer, String, List, Dictionary, Hole }`
  - `union { int64_t integer; std::string string; std::vector<Node> list; Dictionary dictionary; }`
- Benefits:
  - Eliminates per-node heap allocation for most cases.
  - Improves cache locality and traversal speed.
  - Reduces overhead for shallow and medium-depth Bencode data.

### 2. Rework dictionary storage to use ordered map semantics efficiently

- Option A: Use `std::map<std::string, Node>` or `std::map<std::string_view, Node>` with stable storage.
- Option B: Use `std::vector<std::pair<std::string, Node>>` but maintain the sorted order only after all inserts.
- Option C: Use `std::vector` with binary search insert/lookup and reserve capacity when parsing.

- Remove per-insert sort in `Dictionary::add()`.
- Change `Dictionary::contains()` and `operator[]` to binary-search by key instead of linear scan.
- Store keys as `std::string` directly, not as `Node` wrappers, to reduce indirection and overhead.

### 3. Simplify string representation

- Replace custom `BencodeString` and manual `malloc` with either:
  - `std::string` with reserve/resize for parsed payloads, or
  - a small-string-optimized custom string class if maximum size is important.
- If immutability is desired, keep a `std::string` but avoid repeated `std::string_view` copies.
- Parse string data directly into the final storage buffer with reserved capacity.

### 4. Optimize parser dispatch and buffer handling

- Replace the `std::map<char, ParserFunc>` with a switch statement in `parseNodes()`.
- Avoid `std::function` overhead by using direct calls to `parseDictionary`, `parseList`, `parseInteger`, and `parseString`.
- In `parseString()`, reserve buffer capacity and read bytes directly instead of appending character-by-character.
- Consider parsing string bytes with one bulk read from the source when supported by `ISource`.

### 5. Reduce object and include footprint

- Audit headers in `Bencode_Core.hpp`. Prefer forward declarations where possible.
- Remove or refactor any headers that are only included for inline helpers and not required by public API.
- Consider splitting implementation headers from public API headers to reduce compile-time dependency breadth.

### 6. Make parser and stringify optional components

- Add CMake options for optional features that inflate the library size, such as:
  - `BENCODE_ENABLE_JSON_STRINGIFY`
  - `BENCODE_ENABLE_XML_STRINGIFY`
  - `BENCODE_ENABLE_YAML_STRINGIFY`
- If not needed for a minimal build, these stringifiers can be excluded and reduce the binary/code size.

## Implementation Plan

### Phase 1: Benchmark and measure current behavior

- Build the library in `Release` mode.
- Measure:
  - binary size of the static/shared library.
  - heap allocations during parsing of large dictionary/list input.
  - parse throughput and stringify throughput.
- Use representative test files from `tests/files` or generated large Bencode data.

### Phase 1 Baseline Results

- Current Release library size: `455,436` bytes (`build\Release\Bencode_Lib.lib`).
- Current Debug library size: `3,128,962` bytes (`build\Debug\Bencode_Lib.lib`).
- Unit test baseline: `Bencode_Lib_Unit_Tests` passed successfully (`1/1` tests).
- Notes:
  - The project already contains existing Release and Debug build artifacts.
  - A dedicated performance benchmark harness is not present yet, so next phase should add one.

### Phase 2: Core data structure refactor

- Replace `Node` heap allocation with inline `Variant` union.
- Refactor `String`, `List`, and `Dictionary` to avoid wrapping keys and scalar values in extra node allocations.
- Verify correctness with existing tests.

### Phase 2 Status

- Implemented direct `std::string` storage for Bencode string nodes in `classes/include/implementation/variants/Bencode_String.hpp`.
- Refactored dictionary keys to store `std::string` directly in `classes/include/implementation/variants/Bencode_Dictionary.hpp`.
- Optimized `Dictionary::add()` to use ordered insertion instead of full-side sorting.
- Fixed parser dictionary-key lifetime handling in `classes/source/implementation/parser/Default_Parser.cpp`.
- Verified build and unit tests:
  - `Bencode_Lib` built successfully.
  - `Bencode_Lib_Unit_Tests` built and passed in `build\tests\Debug`.

### Phase 3: Dictionary optimization

- Rework `Dictionary` storage and lookup.
- Keep insertion sorted only when necessary.
- Add unit tests for dictionary order, duplicate detection, and performance-critical paths.

### Phase 4: Parser optimization

- Replace dynamic dispatch map with a switch-based parser.
- Optimize string buffer construction and source reads.
- Verify parser depth and syntax validation still work.

### Phase 5: Optional features and build options

- Introduce feature flags for stringifiers.
- Make example/test builds optional with `BENCODE_BUILD_EXAMPLES` and `BENCODE_BUILD_TESTS` already present.
- Make sure minimal consumers can build only core parse/stringify functionality.

### Phase 6: Regression and performance validation

- Re-run unit tests and examples.
- Re-measure binary size and runtime.
- Compare against baseline with:
  - `Release` build size
  - parse rate (MB/s)
  - memory allocations count
  - dictionary access latency

## Expected Benefits

- Lower runtime memory usage in nested Bencode trees.
- Faster parsing and traversal due to reduced heap indirection.
- Smaller C++ object overhead and library binary size.
- Better dictionary lookup performance, especially for large key sets.
- Cleaner, safer storage model aligned with public API semantics.

## Recommended Next Steps

1. Create a performance regression benchmark in `tests` or a new `benchmarks/` folder.
2. Refactor `Node` to remove `std::unique_ptr<Variant>` indirection.
3. Refactor `Dictionary` to use ordered associative lookup.
4. Simplify parser dispatch and string parsing.
5. Add build options for optional serialization formats.

---

File generated on 2026-04-24 for the `Bencode_Lib` repository.
