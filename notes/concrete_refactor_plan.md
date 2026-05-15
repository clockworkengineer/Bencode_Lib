# Bencode_Lib Concrete Refactor Plan (Attribute-Driven)

## Goal
Align Bencode_Lib with the 10 high-quality library attributes from `notes/attributes.md`:
1. Intuitive API Design
2. Comprehensive Documentation
3. High Reliability
4. Performance and Efficiency
5. Maintainability
6. Flexibility and Customization
7. Strong Security
8. High Testability
9. Compatibility and Portability
10. Low Dependency Footprint

---

## Concrete Refactor Plan

### 1. Intuitive API Design
- Refactor public API to expose only stable, minimal headers (`Bencode.hpp`, `Bencode_Core.hpp`, `Bencode_Optional_Stringify.hpp`).
- Move implementation details to internal/private headers.
- Add safer container access helpers: `contains(key)`, `at(key)`, `try_get<T>(key)`.
- Clarify and document operator[] semantics for dictionary/list access.
- Use clear ownership semantics for custom parser/stringifier injection (e.g., `std::unique_ptr<IParser>`).

### 2. Comprehensive Documentation
- Add a Quick Start section to `README.md` and `docs/Guide.md`.
- Document all public CMake options, build profiles, and install/export usage in `docs/Package.md`.
- Add API stability notes and clarify public vs. internal headers.
- Expand usage examples, especially for minimal/embedded modes and custom extensions.

### 3. High Reliability
- Add tests for invalid/malformed Bencode, edge cases, and file I/O errors.
- Add boundary tests for embedded/minimal modes.
- Integrate static analysis/sanitizer options in CMake/test targets.
- Ensure fail-fast behavior and safe defaults for parser limits.

### 4. Performance and Efficiency
- Benchmark core operations; optimize hot paths.
- Expose benchmark targets for CI and user verification.
- Document performance characteristics and tradeoffs.

### 5. Maintainability
- Restructure CMake to export only public headers and targets.
- Consolidate compiler warning settings and enable `-Werror` in CI.
- Separate source into `include/public`, `include/internal`, and `source/` if needed.
- Add/expand contributing guidelines.

### 6. Flexibility and Customization
- Ensure extension points for custom parser/stringifier are well-documented and tested.
- Support user-defined node types and output formats via interfaces.
- Allow configuration of limits and features via CMake options.

### 7. Strong Security
- Add explicit tests for invalid input, fuzzing, and file I/O errors.
- Document and enforce safe defaults for all user-facing APIs.
- Review for exception safety and resource leaks, especially in embedded mode.

### 8. High Testability
- Maintain/expand unit, integration, and compile-only tests.
- Add tests for all public API entry points and extension interfaces.
- Document test strategy in `docs/Testing.md`.

### 9. Compatibility and Portability
- Validate builds on GCC, Clang, MSVC (Debug/Release, minimal/embedded modes).
- Ensure file I/O wrappers work on POSIX/MSVC; document platform-specifics.
- Keep public API free of platform-specific types.

### 10. Low Dependency Footprint
- Avoid external dependencies in public headers and install targets.
- Keep optional features (e.g., JSON/XML/YAML stringify) off by default.
- Document all dependencies and their scope.

---

## Implementation Roadmap
1. Refactor headers and public API boundary.
2. Update CMake to export/install public targets and hide internal includes.
3. Add missing documentation pages and cross-link build/use cases.
4. Expand tests for invalid inputs, boundary conditions, and custom extension points.
5. Add CI-friendly build/test targets and package metadata.
6. Review and validate against the 10 library attributes.

## Deliverables
- Updated `notes/refactor_plan.md` (this file)
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
