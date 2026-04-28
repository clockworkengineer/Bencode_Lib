# DRY Refactor Plan for Bencode_Lib

## Objective
Create a more maintainable, less duplicated codebase by centralizing common logic, reducing repeated interface forwarding, and improving shared file I/O and traversal behavior.

## High-level findings
- `classes/source/Bencode.cpp` contains repeated overloads for `parse`, `stringify`, and forwarding logic to `Bencode_Impl`.
- `classes/source/implementation/Bencode_Impl.cpp` duplicates validation and traversal behavior across const/non-const methods and list/dictionary root creation.
- File I/O support is split between platform-specific source files and duplicated `fromFile`/`toFile` wrappers.
- There is repeated error-handling and stream management logic in `classes/source/implementation/file/Bencode_FileIO.cpp`.
- Public API header `classes/include/Bencode.hpp` exposes multiple overloads and type aliases that can be simplified with helper functions or forwarding utilities.

## Refactor targets

### 1. Centralize `Bencode` overload forwarding
Files: `classes/source/Bencode.cpp`, `classes/include/Bencode.hpp`

- Replace duplicate `parse(ISource&)` and `parse(ISource&&)` code with a single forwarding helper template.
- Do the same for `stringify(IDestination&)` and `stringify(IDestination&&)`.
- Remove direct duplication by forwarding both ref and rvalue overloads to a common implementation method, e.g. `parseImpl(source)` / `stringifyImpl(destination)`.
- Consider `std::conditional_t` / `if constexpr` factoring into one helper rather than repeating the branch twice.

### 2. Consolidate traversal and root access logic in `Bencode_Impl`
Files: `classes/source/implementation/Bencode_Impl.cpp`, `classes/include/implementation/Bencode_Impl.hpp`

- Create a single private helper for traversal that handles empty-root checks and then delegates to `traverseNodes`.
- Use the same helper from both const and non-const `traverse(IAction&)` methods.
- Extract repeated root initialization code from `operator[](string_view)` and `operator[](size_t)` into helpers like `ensureDictionaryRoot()` and `ensureListRoot()`.
- Factor the duplicate catch/retry logic into `Bencode_Impl::getOrCreateDictionaryEntry` / `getOrCreateListEntry`.

### 3. Unify file I/O platform abstraction and helpers
Files: `classes/source/implementation/file/Bencode_FileIO.cpp`, `classes/source/implementation/file/Bencode_File_Disabled.cpp`, `classes/include/implementation/io/Bencode_FileIO_Internal.hpp`

- Introduce an RAII file wrapper or helper class to manage open/read/write/close and reduce repeated `FILE*` logic.
- Centralize `readBencodeString` and `writeBencodeString` behavior for both enabled and disabled variants, perhaps by moving common logic into `Bencode_FileIO_Internal.hpp`.
- Standardize error messages and remove duplicated `throw Error(...)` code paths.
- Consider using `std::ifstream` / `std::ofstream` if portable behavior is acceptable, removing the need for platform-specific open helpers.

### 4. Reduce duplicated parser/exception status handling
Files: `classes/source/implementation/Bencode_Impl.cpp`, `classes/source/implementation/parser/Default_Parser_*.cpp`

- Factor common parse validation into a helper method that returns a status or throws depending on build configuration.
- For `BENCODE_ENABLE_EXCEPTIONS` and non-exception builds, use one implementation with compile-time branching instead of separate code bodies.

### 5. Improve `version()` and common helper code clarity
Files: `classes/source/implementation/Bencode_Impl.cpp`

- Replace repeated `std::stringstream` usage with a smaller helper or `std::format` (C++23) if available.
- If `Bencode_Impl::version` is the only place building version strings, keep it centralized to avoid future duplication.

## Suggested refactor steps

1. Add a small helper API in `classes/include/implementation/Bencode_Impl.hpp`:
   - `template <typename Source> static ParseResultType parseSource(Source &&source);`
   - `template <typename Dest> void stringifyDestination(Dest &&destination) const;`
   - `Node &ensureDictionaryRoot();`
   - `Node &ensureListRoot();`
2. Refactor `classes/source/Bencode.cpp` to forward through the helper instead of repeating `if constexpr` logic.
3. Refactor `classes/source/implementation/Bencode_Impl.cpp`:
   - Implement shared traversal and root-creation helpers.
   - Implement single parse helper for exception/status branches.
4. Refactor file I/O implementation:
   - Move `writeBencodeString` and `readBencodeString` into reusable helpers.
   - Introduce RAII file open class or use standard streams behind a unified interface.
5. Update tests as necessary to ensure all public overloads still behave identically.
6. Add a small regression test for refactored traversal/common helper behavior if not already covered.

## Benefits
- Stronger DRY compliance in public API and implementation layers.
- Easier maintenance of parse/stringify forwarding logic.
- Reduced risk of inconsistencies between const/ref overloads.
- Cleaner file-based I/O support with one central error-handling path.
- Better future extensibility for optional build variants (`BENCODE_ENABLE_FILE_IO`, `BENCODE_BUILD_MINIMAL`, `BENCODE_EMBEDDED_MODE`).

## Recommended file for follow-up
- `classes/source/Bencode.cpp`
- `classes/source/implementation/Bencode_Impl.cpp`
- `classes/source/implementation/file/Bencode_FileIO.cpp`
- `classes/include/implementation/Bencode_Impl.hpp`
- `classes/include/Bencode.hpp`

## Notes
- The current code already uses C++23 and modern idioms; the refactor should preserve that style.
- The plan intentionally avoids changing public behavior while cleaning implementation duplication.
- If the library adopts `std::span` or `std::string_view` more uniformly, additional DRY opportunities will likely emerge in parser/stringify internals.
