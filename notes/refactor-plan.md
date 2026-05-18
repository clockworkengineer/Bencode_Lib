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
- Audit public public API in `classes/include/Bencode_Core.hpp`, `classes/include/Bencode.hpp`, and `classes/source/Bencode.cpp`.
- Reduce `Bencode_Core.hpp` exposure to only core public declarations and move implementation-only includes to private headers.
- Ensure `Bencode_Optional_Stringify.hpp` is truly optional, not forced into every public translation unit.
- Verify `Bencode::parse()`, `stringify()`, `root()`, and operator overloads work consistently for both buffer and file variants.
- Consider renaming unclear methods or adding aliases if it improves clarity without breaking compatibility (for example, retain `toFile()`/`fromFile()` but document them clearly).

## 2. Comprehensive Documentation

### What to improve
- Document the 10 attributes in a way that matches the library's build and runtime behavior.
- Keep README, docs/Guide.md, docs/API.md, and docs/Package.md consistent.

### Concrete tasks
- Add a new section in `README.md` or `docs/Guide.md` summarizing the library’s design goals and the attributes it satisfies.
- Clarify build variants and optional features in `README.md` and `docs/Package.md`.
- Add explicit guidance for embedded/minimal builds and custom stringify/parser extension points.
- Add examples showing how to use `Bencode_Optional_Stringify.hpp` and how to build without optional stringifiers.
- Ensure doc examples use the same public API names as the code.

## 3. High Reliability

### What to improve
- Ensure parse/stringify behavior is deterministic and error-safe.
- Make build variants produce predictable behavior.

### Concrete tasks
- Expand regression tests covering malformed input, truncated data, invalid file paths, and empty trees.
- Add tests for the optional stringify variants (`JSON_Stringify`, `XML_Stringify`, `YAML_Stringify`) and verify they are excluded when their CMake options are off.
- Verify `BENCODE_BUILD_MINIMAL` and `BENCODE_EMBEDDED_MODE` both compile and behave correctly with their reduced feature sets.
- Add tests for `IParser` and `IStringify` custom injection points.

## 4. Performance and Efficiency

### What to improve
- Preserve efficient buffer-only operation and minimize unnecessary allocations.
- Keep benchmarks current for critical code paths.

### Concrete tasks
- Review and optimize the buffer-based source/destination interfaces in `classes/include/implementation/io/Bencode_BufferSource.hpp` and `Bencode_BufferDestination.hpp`.
- Identify any public wrapper code in `Bencode.cpp` that may be copying data unnecessarily.
- Add or update benchmark targets in `tests/` and `examples/` to validate stringification and parsing speed.
- Ensure `BENCODE_BUILD_MINIMAL` does not burden the resulting library with optional I/O or stringifier dependencies.

## 5. Maintainability

### What to improve
- Keep the code organized and easy to update.
- Make build logic explicit and avoid widely-scoped include directories.

### Concrete tasks
- Refactor `CMakeLists.txt` so the public `target_include_directories()` only exports the actual public include directories.
- Avoid exposing implementation directories in the public include path for the main library target.
- Group headers and sources by purpose: core API, file I/O, parser, stringify, embedded support.
- Add header-level comments where public interfaces are defined to clearly document contracts.

## 6. Flexibility and Customization

### What to improve
- Keep optional stringifiers and file I/O modular.
- Preserve extension points for custom parser/stringify implementations.

### Concrete tasks
- Audit `classes/include/interface/IStringify.hpp`, `IParser.hpp`, and `IDestination.hpp` for clarity and inheritance safety.
- Verify `makeStringify<T>()` and similar helpers are documented and easy to use.
- Ensure the library can be built and used with custom stringify/parser objects without requiring optional modules.
- Add documentation examples in `docs/Examples.md` and `docs/Guide.md` for custom stringify/parser integration.

## 7. Strong Security

### What to improve
- Harden parsing and file I/O to reject invalid data safely.

### Concrete tasks
- Add parser validation tests for malformed Bencode that could otherwise lead to undefined behavior.
- Confirm file-based input/output paths are handled safely and do not allow unintended reads/writes when file I/O is disabled.
- Ensure `BENCODE_ENABLE_EXCEPTIONS=OFF` mode has safe fallback semantics and does not hide parse failures.

## 8. High Testability

### What to improve
- Keep test coverage broad and build-variant-aware.
- Make the library easy to verify in CI.

### Concrete tasks
- Add tests for all public interfaces, including `Bencode::operator[]`, `root()`, `traverse()`, `toFile()/fromFile()`, buffer sources, and custom stringifiers.
- Add explicit tests for build variants: normal, minimal, and embedded.
- Add a `tests/` README or `docs/Testing.md` checklist describing what each test target validates.

## 9. Compatibility and Portability

### What to improve
- Keep the library cross-platform and future-proof.

### Concrete tasks
- Ensure `CMakeLists.txt` uses `target_compile_features(${BENCODE_LIBRARY_NAME} PUBLIC cxx_std_23)` or equivalent.
- Verify the file I/O implementation exposes only the correct backend for MSVC vs POSIX.
- Keep `Bencode_LibConfig.cmake` generation consistent across supported build variants.
- Document supported platforms and build flags in `README.md` and `docs/Package.md`.

## 10. Low Dependency Footprint

### What to improve
- Keep optional modules optional.
- Avoid pulling in extra headers or source files unless requested.

### Concrete tasks
- Refactor `Bencode_Optional_Stringify.hpp` so optional stringify includes are only available when the corresponding CMake options are enabled.
- Verify `BENCODE_BUILD_MINIMAL` produces a library variant free of `JSON_Stringify`, `XML_Stringify`, and `YAML_Stringify` code.
- Keep Catch2 and other test-only dependencies confined to test targets.

---

## Implementation Roadmap

### Phase 1: Audit and structural clean-up
1. Audit public headers and determine which implementation-only files are leaking into public includes.
2. Refactor `classes/include/Bencode_Core.hpp` to export only core public interfaces.
3. Refactor `CMakeLists.txt` include directories to remove private implementation folders from the library's public interface.
4. Audit `Bencode_Optional_Stringify.hpp` and make it opt-in.

### Phase 2: API and documentation alignment
1. Update `README.md`, `docs/Guide.md`, and `docs/API.md` to document the 10 library attributes and the actual build/runtime behavior.
2. Add or revise examples for optional stringify, minimal builds, embedded builds, and custom `IStringify`/`IParser` usage.
3. Add a short `notes/attributes.md` cross-reference in documentation.

### Phase 3: Reliability, security, and tests
1. Add or extend regression tests for malformed input, invalid file paths, and build variants.
2. Add tests to verify optional stringify modules are excluded when disabled.
3. Add targeted tests for embedded mode and non-exception fallback semantics.
4. Add a test checklist to `docs/Testing.md`.

### Phase 4: Performance and compatibility verification
1. Review buffer I/O paths and avoid unnecessary copies.
2. Add or refresh benchmark tests for parse/stringify speed.
3. Ensure CMake options produce consistent library exports and versioned package config files.
4. Validate on both POSIX and MSVC code paths where applicable.

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
- `tests/source/*` (regression coverage and variant checks)

---

## Success criteria
- Public API remains easy to use and clearly documented.
- Optional features are truly optional and build correctly when disabled.
- Test coverage covers both nominal and edge cases for parsing/stringifying.
- Build variants are documented and verified.
- The library is easier to maintain and less dependent on implementation-private headers.
- The library retains low dependency footprint while still offering optional customization through extension points.
