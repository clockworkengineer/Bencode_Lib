# Bencode_Lib Packaging and Consumption

## Overview

Bencode_Lib can be built as a normal desktop library, a minimal core variant, or an embedded variant.

The library is intended for modern C++23 toolchains and is tested against:

- GCC
- Clang
- MSVC

The library exports the following targets:

- `Bencode_Lib::Bencode_Lib` — standard library with all enabled features.
- `Bencode_Lib::Bencode_Lib_Minimal` — minimal variant without optional file I/O or stringify extensions.
- `Bencode_Lib::Bencode_Lib_Embedded` — embedded variant with fixed limits, no exceptions, and no dynamic allocation.

## Public Headers

The public API is exposed through the top-level header directory:

- `classes/include/Bencode.hpp`
- `classes/include/Bencode_Core.hpp`
- `classes/include/Bencode_Status.hpp`
- `classes/include/Bencode_Optional_Stringify.hpp`

When installed, these headers are available under `include/`.

Internal headers under `classes/include/implementation/` are not part of the public API and are not intended for direct inclusion. The installed package includes only the implementation headers required by the public API surface, and optional stringify headers are installed only when the corresponding build options are enabled.

### Optional Stringifiers and Minimal Builds

`Bencode_Optional_Stringify.hpp` is the public helper for JSON, XML, and YAML stringifier headers. The actual implementation classes are enabled only when the corresponding CMake options are set to `ON`.

In the minimal variant (`BENCODE_BUILD_MINIMAL=ON`), optional stringifiers and file-based I/O are disabled and the library builds as a lean core-only variant. The install rules also keep only the headers required by the enabled public API and optional features, minimizing the installed footprint for consumers.

### CMake Configuration Options

The following options are the primary knobs used to configure the library:

- `BENCODE_BUILD_MINIMAL`
  - Build a smaller, core-only library variant with buffer-based parse/stringify and no optional file or stringify features.
- `BENCODE_EMBEDDED_MODE`
  - Build an embedded variant with the embedded runtime constraints in effect.
- `BENCODE_ENABLE_FILE_IO`
  - Enable or disable file-based helpers such as `Bencode::fromFile()` and `Bencode::toFile()`.
- `BENCODE_ENABLE_JSON_STRINGIFY`, `BENCODE_ENABLE_XML_STRINGIFY`, `BENCODE_ENABLE_YAML_STRINGIFY`
  - Enable or disable the built-in optional stringify modules.
- `BENCODE_ENABLE_EXCEPTIONS`
  - When disabled, parse entry points return `ParseStatus` instead of throwing exceptions.

### Public Headers

The following headers provide the supported public API:

- `Bencode.hpp`
- `Bencode_Core.hpp`
- `Bencode_Status.hpp`
- `Bencode_Optional_Stringify.hpp`

Internal headers under `classes/include/implementation/` are private and should not be included directly.

### Package Configuration Files

The installed package exports the following CMake files under `lib/cmake/Bencode_Lib/`:

- `Bencode_LibConfig.cmake`
- `Bencode_LibConfigVersion.cmake`
- `BencodeTargets.cmake`

The package exports these targets:

- `Bencode_Lib::Bencode_Lib`
- `Bencode_Lib::Bencode_Lib_Minimal`
- `Bencode_Lib::Bencode_Lib_Embedded`

Use `find_package(Bencode_Lib REQUIRED)` and link against the appropriate target in downstream projects.

## Installing the Library

Build and install:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
cmake --install .
```

If you use package managers or a custom install prefix, pass `-DCMAKE_INSTALL_PREFIX=/your/install/path`.

If you install from a build directory, use the same `CMAKE_INSTALL_PREFIX` in the `cmake --install` step:

```bash
cmake --install . --prefix /your/install/path
```

## Using `find_package`

After installation, consume the library like this:

```cmake
find_package(Bencode_Lib REQUIRED)
add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE Bencode_Lib::Bencode_Lib)
```

This project installs CMake package configuration files to:

- `lib/cmake/Bencode_Lib/Bencode_LibConfig.cmake`
- `lib/cmake/Bencode_Lib/Bencode_LibConfigVersion.cmake`
- `lib/cmake/Bencode_Lib/BencodeTargets.cmake`

For the minimal or embedded variants, use:

```cmake
find_package(Bencode_Lib REQUIRED)
# Minimal variant
target_link_libraries(my_app PRIVATE Bencode_Lib::Bencode_Lib_Minimal)
# Embedded variant
target_link_libraries(my_app PRIVATE Bencode_Lib::Bencode_Lib_Embedded)
```

## CMake Options

Recommended runtime profiles:

- **Default**: Enable tests and examples, keep feature support on.
  - `-DBENCODE_BUILD_TESTS=ON`
  - `-DBENCODE_BUILD_EXAMPLES=ON`
- **Minimal**: Disable optional I/O and stringify extensions.
  - `-DBENCODE_BUILD_MINIMAL=ON`
  - `-DBENCODE_ENABLE_FILE_IO=OFF`
  - `-DBENCODE_ENABLE_JSON_STRINGIFY=OFF`
  - `-DBENCODE_ENABLE_XML_STRINGIFY=OFF`
  - `-DBENCODE_ENABLE_YAML_STRINGIFY=OFF`
- **Embedded**: Disable exceptions, dynamic allocation, and file I/O.
  - `-DBENCODE_EMBEDDED_MODE=ON`
  - `-DBENCODE_ENABLE_EXCEPTIONS=OFF`
  - `-DBENCODE_ENABLE_DYNAMIC_ALLOCATION=OFF`
  - `-DBENCODE_ENABLE_FILE_IO=OFF`

## Compatibility and Portability

Bencode_Lib is compatible with modern C++23 compilers and supports the following toolchains:

- GCC
- Clang
- MSVC

All library targets request C++23 support directly using `target_compile_features(... PUBLIC cxx_std_23)`.

File I/O is selected based on the platform and build options:

- `Bencode_File_POSIX.cpp` on POSIX-compatible systems.
- `Bencode_File_MSVC.cpp` on Microsoft Visual C++.
- `Bencode_File_Disabled.cpp` when `BENCODE_ENABLE_FILE_IO=OFF`.

The installed CMake package configuration is generated consistently for all builds and exports separate targets for the standard, minimal, and embedded variants.

## Release Packaging Hardening

Bencode_Lib includes a package validation step that installs the library into a temporary staging area and verifies the installed header set.

Run the release packaging check after building the library:

```bash
cmake --build build --target Bencode_Lib_PackageCheck
```

This validation ensures the installed package contains only the supported public headers and the implementation headers required by the public API. It also rejects unintended private/internal headers from the installation layout.

### Minimum Release Build Requirements

- Compiler support:
  - GCC 11 or newer
  - Clang 12 or newer
  - MSVC 2019 or newer
- Build type:
  - `-DCMAKE_BUILD_TYPE=Release`
- Sanitizer support (recommended for release testing):
  - `-DBENCODE_ENABLE_SANITIZERS=ON`
  - enables AddressSanitizer, UndefinedBehaviorSanitizer, IntegerSanitizer, and BoundsSanitizer for test targets.

### Supported safe modes

- `BENCODE_BUILD_MINIMAL=ON`
  - builds a lean core-only variant with optional file I/O/stringify support disabled.
- `BENCODE_EMBEDDED_MODE=ON`
  - builds a hardened embedded variant with fixed limits, no exceptions, no file I/O, and no dynamic allocation.

## API Stability

The stable, published API includes:

- `Bencode.hpp`
- `Bencode_Core.hpp`
- `Bencode_Optional_Stringify.hpp`

These headers are the supported public surface for downstream consumers.

Headers in `implementation/` are private and may change between releases.
