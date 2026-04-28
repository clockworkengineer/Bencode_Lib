# Bencode_Lib DRY Refactor Plan

## Goal
Create a cleaner, less repetitive implementation for the library by removing scattered `#if` logic from source and header files and moving build-variant behavior into separate translation units and CMake source selection.

## Findings

1. `CMakeLists.txt` currently builds the library with conditional source inclusion for file I/O only, while many build options are still handled via `#if` in headers and source files.
2. `classes/include/Bencode.hpp` and `classes/include/implementation/Bencode_Impl.hpp` still contain conditional declarations for exceptions and file I/O.
3. `classes/source/Bencode.cpp`, `classes/source/implementation/Bencode_Impl.cpp`, and `classes/source/implementation/parser/Default_Parser.cpp` each contain duplicate code paths for `BENCODE_ENABLE_EXCEPTIONS`.
4. File I/O support is mixed with platform-specific `#ifdef _MSC_VER` blocks inside implementation files.
5. Stringify backend options are conditionally exposed through `Bencode_Core.hpp` instead of using a modular include/source design.
6. `embedded` and `minimal` library targets are currently built from the same source lists, increasing the need for `#if` logic.

## Refactor Strategy

### 1. Centralize build variants in CMake

- Keep `CMakeLists.txt` responsible for selecting translation units by build option.
- Define source groups:
  - `BENCODE_COMMON_SOURCES`
  - `BENCODE_EXCEPTIONS_SOURCES`
  - `BENCODE_STATUS_SOURCES`
  - `BENCODE_FILE_IO_SOURCES`
  - `BENCODE_FILE_IO_DISABLED_SOURCE`
- Append the correct groups based on `BENCODE_ENABLE_EXCEPTIONS` and `BENCODE_ENABLE_FILE_IO`.
- Ensure `Bencode_Lib_Embedded` targets always build with exception-free and file I/O-free sources.

### 2. Split exception vs status parser implementations

- Introduce a shared internal parser helper file such as `Default_Parser_Internal.hpp` / `.cpp`.
- Keep parser logic once, with low-level helpers and frame management in shared code.
- Create two thin translation units:
  - `Default_Parser_Exceptions.cpp`
  - `Default_Parser_Status.cpp`
- Move only the result/exception handling differences into those units.
- Keep the common header `Default_Parser.hpp` clean and avoid build-option duplication where possible.

### 3. Create a unified public API without build-option `#if`s

- In `Bencode.hpp`:
  - Remove `#if BENCODE_ENABLE_EXCEPTIONS` around parse declarations.
  - Expose a single pair of overloads using a compile-time alias for the return type:
    - `using ParseResultType = std::conditional_t<BENCODE_ENABLE_EXCEPTIONS, void, ParseStatus>`
    - `ParseResultType parse(ISource &source) const;`
- In `Bencode_Impl.hpp`:
  - Use the same alias for `parse(ISource &source)`.
- For file I/O:
  - Expose `toFile` / `fromFile` unconditionally in headers.
  - Provide a disabled stub implementation when file I/O is off.

### 4. Isolate file I/O implementation and platform detail

- Move platform-specific logic into separate files:
  - `Bencode_File_MSVC.cpp`
  - `Bencode_File_POSIX.cpp`
- Add a disabled stub source:
  - `Bencode_File_Disabled.cpp`
- Keep public wrappers in `Bencode_FileIO.cpp` so `Bencode::toFile` / `Bencode::fromFile` are always defined.
- Have CMake select the correct file I/O implementation file.

### 5. Reduce conditional headers for optional modules

- Use dedicated headers for optional stringify formats instead of `#if` in `Bencode_Core.hpp`:
  - `JSON_Stringify.hpp`
  - `XML_Stringify.hpp`
  - `YAML_Stringify.hpp`
- Optionally provide a single optional module header that imports the enabled variants.
- Keep implementation headers always present if the corresponding source is included.

### 6. Clean up `Bencode.cpp` and `Bencode_Impl.cpp`

- Move shared constructor/body implementation into common source files.
- Keep only common methods in `Bencode.cpp` and `Bencode_Impl.cpp`.
- Push exception-specific paths into dedicated translation units when needed.
- Use `Bencode_Impl::ParseResultType` to unify the parse method signature.

### 7. Use shared helper headers for parser and I/O utilities

- `Default_Parser_Internal.hpp` should contain:
  - shared `ParserFrame`
  - `copySourceToBuffer`
  - `convertToInteger`
- `Bencode_FileIO.cpp` should contain only wrapper methods.

### 8. Keep header-only definitions minimal and independent

- Avoid `#if` logic in public interface headers by moving variant-specific implementations to source files.
- Preserve the public API surface regardless of build options.
- Keep `IParser`, `ISource`, `IDestination`, `IStringify`, `ITranslator` stable and minimal.

## Action Plan

1. Create a `DRY` CMake source selection section.
2. Extract shared parser implementation helpers.
3. Create separate parser translation units for exceptions vs no-exceptions.
4. Create separate file I/O translation units for POSIX, MSVC, and disabled builds.
5. Introduce a public wrapper source for `Bencode::fromFile` / `Bencode::toFile`.
6. Refactor public headers to use compile-time alias types instead of `#if`.
7. Remove redundant `#if` blocks from `classes/source/Bencode.cpp`, `classes/source/implementation/Bencode_Impl.cpp`, and `classes/source/implementation/parser/Default_Parser.cpp`.
8. Validate with both normal and embedded builds.

## Expected Outcome

- Less source duplication.
- Fewer preprocessor branches in headers and implementation files.
- Build behavior driven primarily by CMake source selection.
- Clear separation between shared logic and build-specific variants.
- Easier maintenance for optional features and embedded/minimal variants.
