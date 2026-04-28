# DRY Refactor Plan for Bencode_Lib

## Objective
Create a more maintainable, lower-risk Bencode library by removing duplicated behavior, centralizing shared helpers, and isolating configuration-specific code paths.

## Library architecture summary
- Public API in `classes/include/Bencode.hpp` and wrapper implementation in `classes/source/Bencode.cpp`.
- Core behavior is delegated to `Bencode_Impl` in `classes/include/implementation/Bencode_Impl.hpp` and `classes/source/implementation/Bencode_Impl.cpp`.
- Parser and stringify behavior are implemented through `IParser`, `IStringify`, `ISource`, and `IDestination` interfaces.
- File I/O support is platform-specific and conditionally compiled via `Bencode_FileIO.cpp`, `Bencode_File_POSIX.cpp`, `Bencode_File_MSVC.cpp`, and `Bencode_File_Disabled.cpp`.
- Optional build variants are supported through CMake flags: `BENCODE_ENABLE_FILE_IO`, `BENCODE_ENABLE_EXCEPTIONS`, `BENCODE_BUILD_MINIMAL`, and `BENCODE_EMBEDDED_MODE`.

## Key DRY issues
- `Bencode.cpp` duplicates overload forwarding for `parse` and `stringify`.
- `Bencode_Impl.cpp` duplicates traversal validation across const and non-const variants and replicate `operator[]` root initialization logic.
- File I/O error handling and buffer management are duplicated in `Bencode_FileIO.cpp` and appear across platform-specific wrappers.
- Parser mode branching exists in separate exception and status source files, but the core parse flow is conceptually the same.
- CMake source and include lists duplicate build configuration details across normal, minimal, and embedded targets.

## Refactor targets

### 1. Public forwarding helpers
Files:
- `classes/source/Bencode.cpp`
- `classes/include/Bencode.hpp`

Goals:
- Replace duplicate `parse(ISource&)` / `parse(ISource&&)` and `stringify(IDestination&)` / `stringify(IDestination&&)` with shared forwarding helpers.
- Keep public overloads simple and stable while centralizing `if constexpr`/return behavior.
- Prefer `implementation->parseSource(...)` / `implementation->stringifyDestination(...)` to repeated code.

### 2. Shared `Bencode_Impl` behavior
Files:
- `classes/source/implementation/Bencode_Impl.cpp`
- `classes/include/implementation/Bencode_Impl.hpp`

Goals:
- Centralize parse validation via a single `parseImpl` helper.
- Unify const and non-const traversal through a shared helper that checks emptiness.
- Extract repeated root creation logic into `ensureDictionaryRoot()` and `ensureListRoot()`.
- Optionally add helpers like `ensureDictionaryOrList` for mixed entry path handling.

### 3. File I/O helper unification
Files:
- `classes/include/implementation/io/Bencode_FileIO_Internal.hpp`
- `classes/source/implementation/file/Bencode_FileIO.cpp`
- `classes/source/implementation/file/Bencode_File_Disabled.cpp`
- `classes/source/implementation/file/Bencode_File_POSIX.cpp`
- `classes/source/implementation/file/Bencode_File_MSVC.cpp`

Goals:
- Move file-open, read, and write helpers into a single shared internal header.
- Introduce an RAII `Bencode_FileHandle` wrapper for `FILE*` to manage lifetime and reduce manual `fclose` calls.
- Keep platform-open logic isolated to `openBencodeFileForRead` / `openBencodeFileForWrite` while centralizing `readBencodeString` / `writeBencodeString`.
- Standardize `Error` message construction across file I/O errors.

### 4. Parser mode consolidation
Files:
- `classes/source/implementation/parser/Default_Parser_Body.hpp`
- `classes/include/implementation/parser/Default_Parser.hpp`
- `classes/source/implementation/Bencode_Impl.cpp`

Goals:
- Factor shared parse semantics into a single helper regardless of exception support.
- Keep `BENCODE_ENABLE_EXCEPTIONS` and non-exception build-specific details only where behavior differs.
- Avoid duplicate parse state and boundary checks in separate code paths.
- Preserve the current public API and build-mode behavior while reducing maintenance burden.

### 5. Build configuration and CMake cleanup
Files:
- `CMakeLists.txt`

Goals:
- Reduce duplicated include directory declarations by using a grouped include list or function.
- Avoid repeated source list duplication between main, minimal, and embedded variants by using shared source groups and conditionally appending platform-specific or disabled implementations.
- Keep optional target feature flags and aliases readable and easy to extend.

## Suggested implementation plan

1. Add DRY helpers in `classes/include/implementation/Bencode_Impl.hpp`:
   - `template <typename Source> ParseResultType parseSource(Source &&source);`
   - `template <typename Destination> void stringifyDestination(Destination &&destination) const;`
   - `Node &ensureDictionaryRoot();`
   - `Node &ensureListRoot();`
   - `ParseResultType parseImpl(ISource &source);`
   - `void traverseImpl(IAction &action) const;`
2. Refactor `classes/source/Bencode.cpp` to use the new forwarding helpers.
3. Refactor `classes/source/implementation/Bencode_Impl.cpp`:
   - Implement shared traversal and root creation helpers.
   - Centralize exception/status parse validation with a single `handleParseResult` variant.
4. Refactor file I/O support:
   - Add `Bencode_FileHandle` and shared helpers in `Bencode_FileIO_Internal.hpp`.
   - Update `Bencode_FileIO.cpp` to use RAII and shared helpers.
   - Keep platform helpers minimal and isolated to open semantics.
5. Review CMake lists for repeated patterns and extract common source/include groups.
6. Run existing unit tests and examples after each step to verify behavior.

## Benefits
- Reduced duplicated code paths across public API, implementation, parser, and file I/O.
- Faster maintenance and lower risk of behavioral drift between builds.
- More consistent error handling and fewer platform-specific branches.
- Easier future extension for new stringify modes, parser options, or embedded/minimal builds.

## Next files to inspect after refactor
- `classes/source/Bencode.cpp`
- `classes/source/implementation/Bencode_Impl.cpp`
- `classes/source/implementation/file/Bencode_FileIO.cpp`
- `classes/include/implementation/Bencode_FileIO_Internal.hpp`
- `CMakeLists.txt`
