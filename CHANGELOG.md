# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.3.0] - 2026-09-28

### Fixed
- **Incomplete Type Unique Pointer Default Arguments**: Replaced constructor default `std::unique_ptr<T> = nullptr` parameters in `Bencode` and `Bencode_Impl` with explicit overloads to avoid Clang 18 incomplete-type `sizeof` instantiation errors.
- **Precompiled Header Feature Synchronization**: Propagated compiler and linker sanitizer flags (`-fsanitize=address,undefined,...`) with `PUBLIC` visibility across all library targets so AST/PCH files and test translation units share identical target features.
- **Embedded Mode Constraints Enforcement**: Automatically disable exceptions, file I/O, dynamic allocations, and optional stringification modules in CMake when `BENCODE_EMBEDDED_MODE=ON`, and guard heap-dependent test targets.

## [1.2.0] - 2026-09-28

### Added
- Move semantics for `Bencode` (`noexcept` move constructor and move assignment operator).
- High-level convenience methods: `Bencode::stringify()`, `Bencode::encode()`, and `Bencode::parse(std::string_view)`.
- Safe template accessors on `Node`: `Node::try_get<T>(key)` and `Node::try_get<T>(index)` returning typed pointers without exceptions.
- Ergonomic accessor methods on `Node`: `get_string()`, `get_int()`, `value_or()`, `as_string()`, and `as_int()`.
- End-to-end downstream consumer validation in `Bencode_Lib_PackageCheck` target testing `find_package(Bencode_Lib REQUIRED)` in a clean installation environment.
- Multi-platform GitHub Actions CI matrix workflow covering Linux (GCC & Clang), macOS (AppleClang), and Windows (MSVC 2022).
- `pkg-config` (`bencode_lib.pc`) generation and installation support.
- CPack packaging configuration supporting source and binary archive generation (`.tar.gz`, `.deb`, `.rpm`, `.zip`).
- Conan 2.0 recipe (`conanfile.py`) and vcpkg manifest (`vcpkg.json`) for package manager distribution.
- Doxygen documentation generation target (`make docs`).
- Release and contribution governance guides (`RELEASING.md`, `CHANGELOG.md`).

### Fixed
- **Critical Packaging Bug**: Missing `XML_Translator.hpp` and misplaced `Default_Translator.hpp` in CMake install rules, which previously prevented consumers from compiling when including `<Bencode_Optional_Stringify.hpp>`.
- **Exported Include Directories**: Added missing `$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>` to `BENCODE_PUBLIC_INCLUDE_DIRS` so downstream `target_link_libraries(target PRIVATE Bencode_Lib::Bencode_Lib)` automatically configures header search paths.
- **Missing Installed Headers**: Added `Bencode_Status.hpp` and `Bencode_FixedVector.hpp` to installed implementation header sets.
- **Sanitizer Configuration**: Fixed GCC compiler error (`-fsanitize=integer` unsupported on GCC) by separating Clang and GCC sanitizer compiler flags.
- **CTest Fuzz Timeout**: Fixed `Bencode_Lib_Fuzz_Parser` test hanging on interactive `std::cin` during automated `ctest` runs.
- **Node try_get Bug**: Fixed broken `std::get_if` call on non-variant wrapper type in `Node::try_get`.

### Changed
- Refactored core facades to complete SOLID architecture improvements across core, IO, parser, and stringifiers.
- Made `implementation` pointer non-const and protected against moved-from dereferencing via `ensureImplementation()`.
