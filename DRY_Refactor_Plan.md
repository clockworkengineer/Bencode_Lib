# DRY Refactor Plan for Bencode_Lib

## Goal
Create a targeted refactor plan that reduces duplication, improves maintainability, and preserves the current public API surface.

## Key Findings

### 1. Wrapper duplication in `Bencode` / `Bencode_Impl`
- `classes/source/Bencode.cpp` and `classes/source/implementation/Bencode_Impl.cpp` contain repeated overloads for `parse`, `stringify`, and `version`.
- `Bencode` mainly forwards to `Bencode_Impl`, but both layers duplicate the same overload patterns and preprocessor `ParseResultType` handling.

### 2. Root container creation duplication
- `Bencode_Impl::ensureDictionaryRoot()` and `Bencode_Impl::ensureListRoot()` are separate methods, but the logic is the same except for the target container type.
- `getOrCreateDictionaryEntry()` and `getOrCreateListEntry()` duplicate root coercion and exception handling.

### 3. File I/O helper duplication
- `classes/source/implementation/file/Bencode_FileIO.cpp` has repeated low-level file open/read/write error-checking logic.
- `writeBencodeString()` and `readBencodeString()` use a similar validation pattern and should share a small utility layer.

### 4. Version string formatting is centralized but exposed twice
- `Bencode_Impl::version()` simply calls `makeVersionString()`, and `Bencode::version()` calls `Bencode_Impl::version()`.
- This is a minor duplication, yet it is a good candidate for consolidation into one stable entry point.

### 5. Test and example helper duplication
- Repeated temporary file path generation and file comparison helpers appear in `tests/source/misc/Bencode_Lib_Tests_Helper.cpp` and several I/O tests.
- Repeated usage of `while (source.more())` loops in I/O tests suggests a small test utility can be extracted.

## Recommended Refactor Areas

### A. Consolidate overload forwarding in `Bencode_Impl`
- Replace the two `parse` overloads with a single forwarding template or a shared private helper.
- Do the same for `stringify`.
- Example:
  - `template <typename Source> ParseResultType parse(Source &&source) { return parseImpl(source); }`
  - `template <typename Destination> void stringify(Destination &&destination) const { stringifyImpl(destination); }`
- Keep public `Bencode` overloads for API compatibility, but have them call one shared internal implementation.

### B. Use template-based root initialization helpers
- Introduce a generic `ensureRoot<Container>()` helper in `Bencode_Impl`.
- Replace `ensureDictionaryRoot()` and `ensureListRoot()` with calls to that helper.
- Likewise, combine `getOrCreateDictionaryEntry()` / `getOrCreateListEntry()` into a generalized `getOrCreateRootEntry<Container>(keyOrIndex)` or a `getOrCreateRoot()` helper.

### C. Factor file I/O utilities into reusable functions
- Create small helpers such as `readAllFromFile(FILE *)` and `writeAllToFile(FILE *, std::string_view)`.
- Move repeated file validation checks into a shared helper or `Bencode_FileHandle` method.
- Consider whether `std::ifstream`/`std::ofstream` are acceptable, but keep platform-specific open wrappers if required by CMake.

### D. Simplify version string handling
- Keep only one public `Bencode::version()` entry point if possible.
- Internally implement version formatting in a shared helper such as `makeVersionString()` that is only defined once.
- Remove redundant forwarding method in `Bencode_Impl` or make it an alias.

### E. Clean up `Bencode` API forwarding layer
- `Bencode::parse`, `Bencode::stringify`, `Bencode::traverse`, and `Bencode::operator[]` are mostly forwarding wrappers.
- Centralize the conditional `if constexpr (std::is_void_v<ParseResultType>)` logic in the implementation layer and keep the public class thin.
- This will reduce duplicated `if constexpr` branching in multiple overloads.

### F. Extract shared test helpers
- Move repeated temporary file creation, path prefixing, and file comparison code into a dedicated test helper module.
- Replace repeated `while (source.more())` loops in IO tests with a helper like `readAllSource(ISource &) -> std::string`.

## Suggested File Changes

### Primary source targets
- `classes/source/Bencode.cpp`
- `classes/source/implementation/Bencode_Impl.cpp`
- `classes/include/implementation/Bencode_Impl.hpp`
- `classes/source/implementation/file/Bencode_FileIO.cpp`
- `classes/include/implementation/file/Bencode_FileIO_Internal.hpp` (if present)

### Supporting cleanup targets
- `classes/include/implementation/node/Bencode_Node.hpp`
- `tests/source/misc/Bencode_Lib_Tests_Helper.cpp`
- `tests/source/io/Bencode_Lib_Tests_IDestination_File.cpp`
- `tests/source/io/Bencode_Lib_Tests_ISource_File.cpp`
- `tests/source/io/Bencode_Lib_Tests_ISource_Buffer.cpp`

## Refactor Execution Plan

1. Introduce small internal helpers for `parse`/`stringify` forwarding in `Bencode_Impl`.
2. Replace duplicate `ensure*Root()` methods with a generic template-based helper.
3. Factor file read/write error handling into reusable helpers in `Bencode_FileIO.cpp`.
4. Consolidate version formatting into one central function and remove duplicate wrappers.
5. Simplify `Bencode.cpp` so that method bodies are one-line forwards to a shared internal helper.
6. Add a dedicated test utility file for repeated test fixtures and path/file helper functions.
7. Run existing unit tests and I/O tests to verify the public API remains unchanged.

## Risk / Scope Notes
- The public API should remain stable; preserve overloads in `Bencode` and the file-based interfaces.
- Keep `BENCODE_ENABLE_EXCEPTIONS` conditional behavior unchanged.
- Avoid changing file open semantics unless refactoring only implementation details.

## Priority
1. `Bencode_Impl` forwarding and root initialization helpers
2. File I/O helper refactor
3. Version formatting consolidation
4. Test helper extraction
5. Additional Node helper cleanup if needed
