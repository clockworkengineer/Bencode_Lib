# Bencode_Lib DRY Refactor Plan

## Goal
Create a more maintainable, less repetitive library by moving build-variant behavior out of headers and high-level implementation files, keeping the public API stable, and centralizing variant selection through CMake and lean translation units.

## Current Architecture

- `Bencode.hpp` exposes the public API and currently uses `ParseResultType` to hide exception vs status-based parsing.
- `Bencode_Impl.hpp` and `Bencode_Impl.cpp` manage parser/stringify state and currently rely on build-time macros to route parsing.
- `IParser.hpp` has two mutually exclusive virtual interfaces depending on `BENCODE_ENABLE_EXCEPTIONS`, with a compile-time fallback shim.
- `Bencode_Core.hpp` conditionally includes optional stringify headers based on build options.
- CMake selects parser source files and optional headers via `BENCODE_ENABLE_*` options.
- File I/O is isolated into wrapper and platform-specific translation units, but some public `#if` logic still remains in headers.

## Findings

1. Public headers still contain build-option branching, which raises maintenance and compile-order complexity.
2. The parser interface mixes public API and build-variant dispatch logic, making it harder to keep shared parser behavior DRY.
3. Optional stringify format headers are conditionally included in `Bencode_Core.hpp`, increasing header-level coupling.
4. The current source-level parser variant implementation duplicates wrapper logic between exception and status builds.
5. File I/O support is already separated well in CMake, but public headers can be made independent of `BENCODE_ENABLE_FILE_IO`.

## Refactor Strategy

### 1. Stabilize the public API

- Preserve `Bencode` as the public entry point.
- Keep `Bencode::parse` overloads and `Bencode::fromFile`/`toFile` available unconditionally.
- Use compile-time aliases only internally where necessary, not to expose separate APIs across builds.
- Avoid `#if BENCODE_ENABLE_EXCEPTIONS` in public headers such as `Bencode.hpp` and `Bencode_Impl.hpp`.

### 2. Simplify parser interface and implementation

- In `classes/include/interface/IParser.hpp`:
  - Expose both `parse(ISource &source)` and `parse(ISource &source, Node &destination)` unconditionally.
  - Implement one method in terms of the other using a protected `parseImpl(...)` hook.
  - Keep pure virtual methods small and build-variant-specific in derived parser implementations.
- In `classes/include/implementation/parser/Default_Parser.hpp`:
  - Change implementation class to override only the build-specific `parseImpl(...)` method.
- In `classes/source/implementation/parser/Default_Parser_Exceptions.cpp` and `Default_Parser_Status.cpp`:
  - Share common parsing logic through `Default_Parser_Body.hpp` or `Default_Parser_Internal.hpp`.
  - Keep exception/status differences limited to wrapper and outcome handling.

### 3. Move optional stringify headers into a dedicated module

- Introduce `classes/include/Bencode_Optional_Stringify.hpp`.
- Replace conditional header includes in `Bencode_Core.hpp` with a single import of the optional module.
- Let CMake continue to control whether optional stringify source headers are installed/built.

### 4. Keep file I/O declarations unconditional

- Ensure `Bencode.hpp` and `Bencode_Impl.hpp` declare `fromFile`/`toFile` regardless of `BENCODE_ENABLE_FILE_IO`.
- Use `Bencode_File_Disabled.cpp` for no-file-IO builds, so the public API remains stable.

### 5. Reduce duplication in core implementation

- Refactor `Bencode.cpp` and `Bencode_Impl.cpp` to remove `#if BENCODE_ENABLE_EXCEPTIONS` where possible.
- Use helper functions or small wrapper dispatches instead of duplicated parsing code.
- Keep `Bencode_Impl` free of direct exception-vs-status logic except in the lowest-level parser adapter.

### 6. Centralize build configuration in CMake

- Keep `CMakeLists.txt` as the authoritative source selection mechanism.
- Ensure parser variant and file I/O variant source lists are clearly defined and appended.
- Use alias targets for embedded/minimal variants to keep source selection explicit.

## Action Plan

1. Audit all public headers for `#if` and `#ifdef` around build options.
2. Refactor `IParser` to expose a stable unbranched interface.
3. Introduce `Bencode_Optional_Stringify.hpp` and remove conditional stringify includes from `Bencode_Core.hpp`.
4. Refactor `Default_Parser` into shared helpers plus two thin translation units.
5. Clean up `Bencode.cpp`/`Bencode_Impl.cpp` `parse` wrappers and remove duplicated branches.
6. Confirm `Bencode::fromFile`/`toFile` remain available in all build variants.
7. Validate with full build of main, minimal, and embedded targets.
8. Run unit tests after the refactor and verify examples compile.

## Expected Outcome

- Public headers become build-option independent.
- Parser variant behavior is isolated to a small set of implementation files.
- Optional stringify support is modular and easier to maintain.
- File I/O remains configurable without changing the public interface.
- Build logic is driven by CMake source selection, not by scattered preprocessor branches.
- The library is easier to reason about and extend for future build variants.
