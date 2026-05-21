# Bencode_Lib Hardening Plan

## Summary

This document captures a concrete hardening plan for `Bencode_Lib`. The library already includes a modular parser/stringify architecture, optional embedded and minimal builds, and a unit test suite. The plan below focuses on improving robustness across parser input handling, memory safety, build-time hardening, and test coverage.

## Current Security-Relevant Baseline

- C++23 with strong warning options enabled by default for library and tests.
- Optional build flags for exceptions, file I/O, dynamic allocation, JSON/XML/YAML stringify support.
- Embedded mode disables exceptions, file I/O, and dynamic allocation, and applies compile-time limits.
- Public headers expose top-level API; implementation headers are installed because public headers reference them by relative path.
- Parser source includes explicit syntax checks for integers, string lengths, maximum parser depth, and container structure.
- Tests include regular, embedded, minimal, and public-header compile targets.
- Sanitizers are optional in tests via `BENCODE_ENABLE_SANITIZERS`.

## Hardening Objectives

1. Eliminate undefined behavior and memory unsafety in parsing and I/O.
2. Increase test coverage for malformed, boundary, and hostile input.
3. Strengthen build-time detection with warnings, sanitizers, and static analysis.
4. Harden public API surface and runtime configuration options.
5. Reduce attack surface in optional/embedded modes and installed headers.

## Concrete Hardening Actions

### 1. Parser Resilience

- Add explicit runtime limits for all parser resources, not only embedded mode.
  - enforce `BENCODE_MAX_STRING_LENGTH` and `DEFAULT_MAX_PARSER_DEPTH` in release builds.
  - add a configurable `maxNodeCount` or `maxContainerSize` for normal builds.
- Ensure all string length parsing paths validate before resizing buffers.
  - any `std::string` or `std::vector` resize must be bounded by `maxStringLength`.
- Harden integer parsing and overflow detection.
  - validate numeric values and reject invalid forms before conversion.
  - add tests for edge cases such as `-0`, leading zeros, overflow, and very large length prefixes.
- Increase the number of parser assertion-free no-fallthrough states.
- Audit `Default_Parser_Body.hpp` and `Bencode_Parser_Constants.hpp` for any missing state validation.

### 2. Input Boundary & Structure Validation

- Add negative test cases for all malformed Bencode variants:
  - truncated data, missing terminators, wrong separators, invalid character bytes.
  - invalid list/dictionary nesting, duplicate or non-sorted dictionary keys, invalid key types.
- Add size- and depth-focused DoS tests:
  - extremely large but valid length prefixes.
  - deeply nested structures that exceed configured parser depth.
- Add explicit bounds checks to `Bencode_Dictionary`, `Bencode_List`, and `Bencode_FixedVector`.
  - any index or insert operations should return or throw a consistent error on invalid indices.

### 3. Memory and Ownership Safety

- Review and document lifetime semantics for APIs that return `std::string_view`.
  - if any public interface exposes a view into temporary data, change it to return `std::string` or document required ownership.
- Verify RAII correctness in file I/O wrappers.
  - `Bencode_FileHandle` must always close on destruction.
  - avoid `FILE*` use if it can be replaced with safer C++ streams or `std::filesystem` abstractions.
- Review all raw pointer and manual buffer operations in the implementation headers.
  - replace manual `memcpy`-style loops with bounds-checked C++ container operations where possible.

### 4. Build-Time Hardening

- Enable `-Wconversion`, `-Wshadow`, and `-Wformat` in addition to the current warning set.
- Add `-fsanitize=address,undefined,integer,bounds` to the test targets when `BENCODE_ENABLE_SANITIZERS` is enabled.
- Add a CI build matrix covering:
  - normal build with `-Werror` and sanitizers.
  - embedded build with `BENCODE_ENABLE_EXCEPTIONS=0`, `BENCODE_ENABLE_FILE_IO=0`, `BENCODE_ENABLE_DYNAMIC_ALLOCATION=0`.
  - minimal build without optional stringify support.
- Introduce static analysis checks by adding one of:
  - clang-tidy config with `cppcoreguidelines-*`, `bugprone-*`, `performance-*`, `readability-*`.
  - Cppcheck target for the library and headers.

### 5. Testing & Fuzzing

- Add a fuzzing harness around the parser input path.
  - target `Bencode` parsing from a raw buffer and from an `ISource` implementation.
  - run corpus cases including valid examples and malformed edge cases.
- Expand tests for all public API flavors:
  - `Bencode::parse()`, `Bencode::stringify()`, public `operator[]`, file I/O wrappers, and optional stringify formats.
- Add negative tests for file I/O failure cases.
  - invalid path names, read/write permission denials, partial reads, and stream failures.
- Add regression tests for public header compile and include hygiene.
  - ensure no unintended internal dependency leaks.

### 6. API Surface and Header Hygiene

- Audit the public API to ensure all headers in `classes/include` are safe to use without dragging unintended implementation details.
- If implementation headers are installed only to support relative includes, add a stronger separation or wrapper layer to reduce the exposed surface.
- Validate that any public header using `std::string_view` or `std::string` does not create dangling references.
- Add an explicit compatibility and safety note to public headers explaining `BENCODE_ENABLE_*` macros and usage constraints.

### 7. Optional / Embedded Mode Hardening

- Harden embedded mode by adding compile-time `static_assert` checks for sizes and container limits.
- Confirm that `BENCODE_ENABLE_FILE_IO=0` mode does not expose file APIs at runtime.
- Add matrix tests that build the embedded variant and run its test harness.
- Evaluate whether embedded mode should also disable optional stringify support by default to minimize surface area.

### 8. Packaging and Release Hardening

- Add release packaging checks to ensure the installed include set does not expose unwanted implementation internals.
- Document the minimum compiler and sanitizer support required for release builds.
- Add a `hardening` or `security` section to the package docs describing the supported safe modes.

## Suggested Immediate Tasks

1. Create a dedicated `notes/hardening-plan.md` (this file) and track progress there.
2. Add targeted unit tests for parser edge cases and malformed input.
3. Add a fuzz target and integrate it into CI at least as a nightly or approval-required job.
4. Extend CMake warning options and sanitizer support in `CMakeLists.txt`.
5. Audit `classes/include/implementation` APIs for safe ownership semantics and remove or encapsulate any unsafe raw pointer usage.

## Progress

- Implemented the parser fuzz harness in `tests/source/fuzz/Bencode_Lib_Fuzz_Parser.cpp`.
- Added optional fuzz-target build support in `tests/CMakeLists.txt` and `CMakeLists.txt` via `BENCODE_BUILD_FUZZERS`.
- Audited implementation APIs for ownership safety and replaced manual `FILE*` management with `std::unique_ptr<FILE, decltype(&std::fclose)>` in file I/O adapters.
- Extended public-header compilation hygiene coverage by including all public headers in the public header compile test.

## Notes

- The library already has a good foundation for hardening due to its configurability and test scaffolding.
- The main remaining gaps are broader runtime limits, stronger build-time enforcement, and systematic adversarial input coverage.
- This plan is intentionally concrete, with actionable changes rather than only high-level goals.
