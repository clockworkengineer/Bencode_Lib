# Testing Strategy

This document describes the Bencode_Lib test coverage strategy, including unit, integration, and benchmark targets.

## Test Coverage Goals

- Verify the public API boundary through a dedicated public-header compile test.
- Validate file I/O helpers with `Bencode::fromFile()` and `Bencode::toFile()`.
- Ensure custom extension points work by testing `IParser` and `IStringify` injection.
- Verify `Bencode::operator[]` access patterns for dictionaries and lists.
- Validate traversal semantics with `IAction`-based tree visitor tests.
- Keep the embedded-mode tests separate from the main library tests.
- Expose benchmark targets for optional performance verification.

## Available Test Targets

- `Bencode_Lib_Unit_Tests`
  - Main unit/integration test executable.
  - Executes the full Catch2-based test suite.
  - Verifies the normal build configuration and full public API surface.
- `Bencode_Lib_Embedded_Unit_Tests`
  - Embedded-mode test harness with reduced runtime features.
- `Bencode_Lib_PublicHeader_CompileTest`
  - Compile-only check that includes only `Bencode.hpp` and `Bencode_Core.hpp`.
- `Bencode_Lib_Benchmark`
  - Benchmark target for performance verification.
- `Bencode_Lib_Embedded_Benchmark`
  - Embedded benchmark target built with reduced mode and lightweight settings.

## Running Tests

From the build directory:

```bash
cmake --build . --target Bencode_Lib_Unit_Tests
ctest -R Bencode_Lib_Unit_Tests --output-on-failure
```

For the public-header compile test:

```bash
cmake --build . --target Bencode_Lib_PublicHeader_CompileTest
ctest -R Bencode_Lib_PublicHeader_CompileTest --output-on-failure
```

To run all configured tests:

```bash
ctest --output-on-failure
```

## CI Coverage Notes

- The public-header compile test helps detect unintended internal include dependencies.
- `fromFile()` / `toFile()` helpers are validated through integration-style tests that use temporary filesystem artifacts.
- Minimal and embedded builds validate file I/O disabled behavior, ensuring file operations fail cleanly instead of causing undefined behavior.
- Minimal builds also verify optional stringify support is safely disabled and the public stringify helper header remains compile-safe.
- Regression tests cover malformed input, truncated data, invalid file paths, empty trees, and parse failure safety.
- `IParser` and `IStringify` interfaces are covered with custom implementations so the extension points remain stable.
- Benchmark targets are optional for CI, but they are configured as CTest targets and can be included in performance-oriented pipelines.
- `Bencode_Lib_Benchmark` exercises parse/stringify throughput and validates buffer-based I/O performance for large documents.
