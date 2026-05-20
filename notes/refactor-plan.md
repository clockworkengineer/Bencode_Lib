# Bencode_Lib Refactor Plan

This plan implements the 10 library attributes from `notes/attributes.md` in a concrete, actionable way for `Bencode_Lib`.

## Goal
Align the library with the attributes of a high-quality library by improving API clarity, documentation, reliability, performance, maintainability, flexibility, security, testability, compatibility, and dependency footprint.

---

## 1. Intuitive API Design

### What to improve
- Make public usage patterns easier to read and understand.
- Standardize names and remove hidden implementation exposure.
- Keep the public header surface minimal and explicit.

### Concrete tasks
- Audit the public API in `classes/include/Bencode_Core.hpp`, `classes/include/Bencode.hpp`, and `classes/source/Bencode.cpp`.
- Ensure `Bencode.hpp` does not depend on implementation headers beyond `Bencode_Config.hpp` and `Bencode_Status.hpp`.
- Confirm `Bencode_Optional_Stringify.hpp` is truly opt-in, and that optional stringify headers are not transitively exposed through `Bencode.hpp`.
- Document the exact public header boundary in `README.md`, `docs/Guide.md`, and `docs/API.md`.
- Ensure method names and overloads are intuitive and consistent across file/ buffer sources, e.g. `parse()`, `stringify()`, `fromFile()`, `toFile()`.

## 2. Comprehensive Documentation

### What to improve
- Document the 10 attributes in a way that matches actual build and runtime behavior.
- Keep README, docs/Guide.md, docs/API.md, and docs/Package.md consistent.

### Concrete tasks
- Add a dedicated section in `README.md` and `docs/Guide.md` summarizing the library's design attributes and supported variants.
- Document optional build flags clearly, including `BENCODE_BUILD_MINIMAL`, `BENCODE_EMBEDDED_MODE`, `BENCODE_ENABLE_FILE_IO`, and the optional stringify flags.
- Add usage examples for the optional stringify helpers in `Bencode_Optional_Stringify.hpp`.
- Add build examples for minimal and embedded modes, including `-DBENCODE_ENABLE_EXCEPTIONS=OFF` and `-DBENCODE_ENABLE_DYNAMIC_ALLOCATION=OFF`.
- Cross-link `notes/attributes.md` from the public documentation.

## 3. High Reliability

### What to improve
- Ensure parse/stringify behavior remains deterministic and error-safe.
- Make build variants behave predictably.

### Concrete tasks
- Extend regression tests in `tests/source/` for malformed input, truncated Bencode, invalid file paths, and empty roots.
- Add coverage for `BENCODE_ENABLE_EXCEPTIONS=OFF` behavior by verifying `ParseStatus` is returned and error codes are observable.
- Add tests for disabled file I/O and optional stringify compilation with `BENCODE_ENABLE_FILE_IO=OFF`, `BENCODE_ENABLE_JSON_STRINGIFY=OFF`, `BENCODE_ENABLE_XML_STRINGIFY=OFF`, and `BENCODE_ENABLE_YAML_STRINGIFY=OFF`.
- Verify the existing public header compile tests and embedded tests in `tests/CMakeLists.txt` remain complete for these variants.

## 4. Performance and Efficiency

### What to improve
- Preserve efficient buffer-only operation and minimize unnecessary allocations.
- Keep benchmark targets current.

### Concrete tasks
- Review `classes/include/implementation/io/Bencode_BufferSource.hpp` and `classes/include/implementation/io/Bencode_BufferDestination.hpp` for copy avoidance and direct-memory handling.
- Audit `Bencode.cpp` wrapper layers for unnecessary string or vector copies.
- Update benchmark test targets in `tests/CMakeLists.txt` and ensure runtime benchmarks exercise parser/stringifier throughput.
- Confirm `BENCODE_BUILD_MINIMAL` does not force optional modules into the final library.

## 5. Maintainability

### What to improve
- Keep code organization clean and build logic explicit.
- Avoid broad public include paths.

### Concrete tasks
- Ensure `CMakeLists.txt` only exports `classes/include` as the public include directory, not implementation directories.
- Refactor source/header groupings into clear areas: core API, parser, stringify, I/O, embedded support.
- Add header comments at the top of public interfaces to document their intended use and stability.
- Keep private implementation headers in `classes/include/implementation/` and out of installed public package paths.

## 6. Flexibility and Customization

### What to improve
- Keep optional stringifiers and I/O modular.
- Preserve extension points for custom parser/stringify implementations.

### Concrete tasks
- Audit `classes/include/interface/IStringify.hpp`, `classes/include/interface/IParser.hpp`, and `classes/include/interface/IDestination.hpp` for minimal, clear interfaces.
- Confirm custom parser/stringifier injection works without requiring optional modules.
- Document custom `IParser`/`IStringify` examples in `docs/Guide.md` and `docs/Examples.md`.
- Ensure `Bencode_Optional_Stringify.hpp` only exposes JSON/XML/YAML helpers when the corresponding CMake flags are enabled.

## 7. Strong Security

### What to improve
- Harden parsing and file I/O to reject invalid input safely.

### Concrete tasks
- Add parser validation tests for malformed Bencode that could otherwise lead to undefined behavior.
- Verify file-based helpers fail cleanly or are unavailable when `BENCODE_ENABLE_FILE_IO=OFF`.
- Ensure embedded/non-exception builds do not assume exceptions or dynamic allocation.
- Confirm parse error conditions produce explicit diagnostics via `ParseStatus` in no-exception mode.

## 8. High Testability

### What to improve
- Keep test coverage broad and build-variant-aware.
- Make library correctness easy to verify in CI.

### Concrete tasks
- Add tests for public APIs: `Bencode::operator[]`, `root()`, `traverse()`, `toFile()`, `fromFile()`, and buffer I/O helpers.
- Add explicit build-variant tests for normal, minimal, and embedded configurations in `tests/CMakeLists.txt`.
- Document test scope and variants in `docs/Testing.md`.
- Keep Catch2 and all test-only dependencies isolated to the test targets.

## 9. Compatibility and Portability

### What to improve
- Keep the library cross-platform and future-proof.

### Concrete tasks
- Verify `CMakeLists.txt` already sets `target_compile_features(${target} PUBLIC cxx_std_23)` for the library targets.
- Confirm the correct file I/O backend is selected for MSVC, POSIX, and disabled-file I/O builds.
- Confirm `Bencode_LibConfig.cmake` installs the expected targets `Bencode_Lib::Bencode_Lib`, `Bencode_Lib::Bencode_Lib_Minimal`, and `Bencode_Lib::Bencode_Lib_Embedded`.
- Document supported toolchains and platform assumptions in `README.md` and `docs/Package.md`.

## 10. Low Dependency Footprint

### What to improve
- Keep optional modules gated.
- Avoid adding dependencies unless needed for the public API.

### Concrete tasks
- Refactor `Bencode_Optional_Stringify.hpp` so it only exposes optional sections when the corresponding CMake options are enabled.
- Prevent `BENCODE_BUILD_MINIMAL` from linking or compiling JSON/XML/YAML stringifier code.
- Ensure Catch2 remains test-only and is not part of the installed library footprint.

---

## Implementation Roadmap

### Phase 1: Audit and structural clean-up
1. Audit public headers and determine whether any implementation-only includes leak through the public API.
2. Refactor `classes/include/Bencode_Core.hpp` to export only core public interfaces.
3. Refactor `CMakeLists.txt` so only `classes/include` is exported to consumers, and implementation directories remain private.
4. Confirm `Bencode_Optional_Stringify.hpp` is truly opt-in and gated by the JSON/XML/YAML flags.

### Phase 2: API and documentation alignment
1. Update `README.md`, `docs/Guide.md`, and `docs/API.md` to document the 10 library attributes and expected build/runtime behavior.
2. Revise examples for optional stringify usage, minimal builds, embedded builds, and custom `IStringify`/`IParser` usage.
3. Add a cross-reference from public docs to `notes/attributes.md`.

### Phase 3: Reliability, security, and tests
1. Add or extend regression tests for malformed input, invalid file paths, and variant-specific behavior.
2. Add tests that verify optional stringify modules are excluded when their CMake flags are disabled.
3. Add tests for embedded mode and no-exception mode semantics.
4. Add a test checklist to `docs/Testing.md`.

### Phase 4: Performance and compatibility verification
1. Review buffer I/O paths for copy reduction and direct memory transfer.
2. Refresh benchmark tests for parse and stringify performance.
3. Verify package configuration and installed targets for all supported variants.
4. Validate the library on both POSIX and MSVC code paths where possible.

---

## Files and areas to update
- `notes/refactor-plan.md` (this file)
- `classes/include/Bencode_Core.hpp`
- `classes/include/Bencode.hpp`
- `classes/include/Bencode_Optional_Stringify.hpp`
- `classes/source/Bencode.cpp`
- `CMakeLists.txt`
- `README.md`
- `docs/Guide.md`
- `docs/API.md`
- `docs/Package.md`
- `docs/Testing.md`
- `tests/source/*`

---

## Success criteria
- Public API remains easy to use and clearly documented.
- Optional features are truly optional and build correctly when disabled.
- Test coverage covers both nominal and edge cases for parsing and stringifying.
- Build variants are documented and verified.
- The library is easier to maintain and less dependent on implementation-private headers.
- The library retains a low dependency footprint while still offering optional customization through extension points.
