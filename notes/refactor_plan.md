# Bencode_Lib Refactor Plan

## Goal
Improve Bencode_Lib so it aligns more closely with the target library attributes: intuitive API design, comprehensive documentation, reliability, performance, maintainability, flexibility, security, testability, compatibility, and a low dependency footprint.

## Findings

### Strengths
- Good high-level documentation in `README.md`, `docs/Guide.md`, and `docs/API.md`.
- Clear support for optional build modes: minimal, embedded, and file I/O toggles.
- Existing unit tests and benchmark targets.
- CMake-based cross-platform build structure with MSVC/POSIX file wrappers.
- Low runtime dependency footprint for the library itself.

### Improvement Areas
- Public API exposure is too broad: `Bencode_Core.hpp` pulls implementation headers and internal details into the public include surface.
- `Bencode` construction and ownership semantics for custom `IParser`/`IStringify` are not ergonomically defined.
- Operator[] and root access are useful but need clearer documented semantics and safer access helpers.
- Documentation is good, but the library would benefit from more explicit quick-start, API stability, and configuration guidance.
- CMake install/export/package metadata is not currently exposed in the repo documentation.
- Security hardening is limited to compile-time options; no explicit fuzz tests or boundary test cases were found.
- Test coverage is strong for normal behavior, but reliability should be improved with invalid input and integration scenarios.
- Build system still exposes too much implementation include path information to consumers.

## Refactor Plan

### 1. Refine public API and header boundary
- Keep public headers limited to `Bencode.hpp`, `Bencode_Core.hpp`, interface headers, and stable node type headers.
- Hide implementation-specific headers under a private/internal headers directory that is not installed or exported.
- Refactor `Bencode_Core.hpp` so it re-exports only public API types, not implementation internals.
- Change custom parser/stringifier APIs to use clear ownership semantics, such as `std::unique_ptr<IParser>` / `std::unique_ptr<IStringify>` or factory helper types.
- Add safer container access helpers:
  - `contains(const std::string_view &key)`
  - `at(const std::string_view &key)` with bounds checking
  - `try_get<T>(const std::string_view &key)`
- Document operator[] behaviors explicitly: dictionary insertion vs list indexing errors.

### 2. Improve documentation and examples
- Add a concise Quick Start section to `README.md` and `docs/Guide.md`.
- Add a `docs/Usage.md` or extend `docs/Guide.md` with explicit minimal/embedded examples.
- Document all public CMake options and recommended default build profiles.
- Add a `docs/Package.md` describing CMake export names, install targets, and `find_package` usage.
- Add an API stability note describing which headers are public and which are internal.

### 3. Harden reliability and security
- Add tests for invalid and malformed Bencode inputs, including:
  - incomplete dictionaries/lists
  - missing terminators
  - incorrect length prefixes
  - deeply nested structures
- Add boundary tests for embedded and minimal modes using maximum limits.
- Add explicit file I/O failure tests for missing files, permission errors, and corrupt input.
- Add static analysis or sanitizer configuration to the CMake test targets.
- Ensure safe defaults for parser limits and fail-fast behavior on invalid data.

### 4. Improve build maintainability and packaging
- Restructure CMake targets so public include directories are only the stable API headers.
- Add `GNUInstallDirs` and export/install targets for `Bencode_Lib::Bencode_Lib` and optional variants.
- Add a `Bencode_LibConfig.cmake` export file and package metadata for consumers.
- Consolidate compiler warning settings into target properties and enable `-Werror` in CI if possible.
- Separate source into `include/public`, `include/internal`, and `source/` if needed to clarify API boundaries.

### 5. Expand tests and CI coverage
- Add a public-header compile test that includes only `Bencode.hpp` and `Bencode_Core.hpp`.
- Add integration tests for the `fromFile`/`toFile` helpers and JSON/XML/YAML stringify extensions.
- Add tests for custom parser/stringifier implementations using the `IParser` and `IStringify` interfaces.
- Add benchmark verification and optional CI run of the benchmark targets.
- Document the test strategy in `docs/Testing.md`.

### 6. Improve compatibility and portability
- Ensure the public API works cleanly on GCC, Clang, and MSVC in both debug and release.
- Add a compatibility note for embedded mode and minimal build mode.
- Validate the POSIX/MSVC file I/O wrapper behavior on both platforms.
- Keep the library dependency footprint low by avoiding unnecessary external headers in public install targets.

## Implementation Roadmap

1. Refactor headers and public API boundary.
2. Update CMake to export/install public targets and hide internal includes.
3. Add missing documentation pages and cross-link build/use cases.
4. Expand tests for invalid inputs, boundary conditions, and custom extension points.
5. Add CI-friendly build/test targets and package metadata.
6. Review and validate against the 10 library attributes.

## Deliverables
- `notes/refactor_plan.md` (this file)
- Public API boundary cleanup in `classes/include`
- Updated `CMakeLists.txt` with install/export targets
- Improved docs in `README.md`, `docs/Guide.md`, and new package/test docs
- Additional unit/integration tests for robustness and security

## Success Criteria
- Public headers expose only stable API; internal implementation headers are not installed.
- Build options are documented and reproducible.
- Invalid input and edge-case tests pass in both normal and embedded build modes.
- `Bencode_Lib::Bencode_Lib` can be consumed via `find_package` with documented usage.
- The library demonstrates a clear alignment with the 10 attributes described in `notes/attributes.md`.
