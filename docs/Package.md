# Bencode_Lib Packaging and Consumption

## Overview

Bencode_Lib can be built as a normal desktop library, a minimal core variant, or an embedded variant.

The library exports the following targets:

- `Bencode_Lib::Bencode_Lib` — standard library with all enabled features.
- `Bencode_Lib::Bencode_Lib_Minimal` — minimal variant without optional file I/O or stringify extensions.
- `Bencode_Lib::Bencode_Lib_Embedded` — embedded variant with fixed limits, no exceptions, and no dynamic allocation.

## Public Headers

The public API is exposed through the top-level header directory:

- `classes/include/Bencode.hpp`
- `classes/include/Bencode_Core.hpp`
- `classes/include/Bencode_Optional_Stringify.hpp`

When installed, these headers are available under `include/`.

Internal headers under `classes/include/implementation/` are not part of the public API and are not included in the installed include path.

### Optional Stringifiers and Minimal Builds

`Bencode_Optional_Stringify.hpp` is the public helper for JSON, XML, and YAML stringifier headers. The actual implementation classes are enabled only when the corresponding CMake options are set to `ON`. In the minimal variant (`BENCODE_BUILD_MINIMAL=ON`), optional stringifiers and file-based I/O are disabled.

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

## API Stability

The stable, published API includes:

- `Bencode.hpp`
- `Bencode_Core.hpp`
- `Bencode_Optional_Stringify.hpp`

These headers are the supported public surface for downstream consumers.

Headers in `implementation/` are private and may change between releases.
