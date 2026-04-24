# Embedded Build Guide for Bencode_Lib

This guide explains how to build and use `Bencode_Lib` in constrained embedded environments.

## Embedded Build Goals

- Disable heap allocation and file I/O.
- Disable exceptions when supported by the toolchain.
- Use fixed-size containers and bounded string lengths.
- Provide a minimal buffer-only parse/stringify API.

## Embedded Build Configuration

Use CMake to configure the embedded build variant:

```bash
cmake -S . -B build-embedded \
  -DBENCODE_EMBEDDED_MODE=ON \
  -DBENCODE_ENABLE_EXCEPTIONS=OFF \
  -DBENCODE_ENABLE_FILE_IO=OFF \
  -DBENCODE_ENABLE_DYNAMIC_ALLOCATION=OFF \
  -DBENCODE_MAX_NODE_COUNT=128 \
  -DBENCODE_MAX_CONTAINER_SIZE=32 \
  -DBENCODE_MAX_STRING_LENGTH=1024
cmake --build build-embedded --config Release
```

### Resulting Embedded Targets

- `Bencode_Lib_Embedded`
- `Bencode_Lib_Embedded_Unit_Tests`
- `Bencode_Lib_Embedded_Benchmark`

## Embedded Macros

- `BENCODE_EMBEDDED_MODE`
- `BENCODE_ENABLE_EXCEPTIONS`
- `BENCODE_ENABLE_FILE_IO`
- `BENCODE_ENABLE_DYNAMIC_ALLOCATION`
- `BENCODE_MAX_NODE_COUNT`
- `BENCODE_MAX_CONTAINER_SIZE`
- `BENCODE_MAX_STRING_LENGTH`

These macros are exported by the library and affect both build-time behavior and public API compatibility.

## Embedded API Notes

### Buffer-based I/O only

Embedded builds should use `BufferSource` and `BufferDestination` rather than file-based sources/destinations.

```cpp
#include "Bencode.hpp"
#include "Bencode_Core.hpp"

using namespace Bencode_Lib;

std::string data = "d3:key5:valuee";
Bencode doc;

#if BENCODE_ENABLE_EXCEPTIONS
  doc.parse(BufferSource{data});
#else
  ParseStatus status = doc.parse(BufferSource{data});
  if (!status.ok()) {
    std::cerr << "Parse failed: " << status.message << std::endl;
    return 1;
  }
#endif

BufferDestination destination;
doc.stringify(destination);
std::string result = destination.toString();
```

### No-exceptions mode

When `BENCODE_ENABLE_EXCEPTIONS` is disabled, parse functions return `ParseStatus` instead of throwing. Use `ParseStatus::ok()` to verify success.

### Fixed-capacity containers

With `BENCODE_ENABLE_DYNAMIC_ALLOCATION=OFF`, lists and dictionaries use statically bounded containers:

- `BENCODE_MAX_CONTAINER_SIZE` controls capacity for lists and dictionaries.
- `BENCODE_MAX_STRING_LENGTH` controls the maximum embedded string length.
- `BENCODE_MAX_NODE_COUNT` is reserved for future embedded allocator support.

## Example Consumer CMake Configuration

A sample embedded consumer configuration is provided in `examples/embedded/CMakeLists.txt`.

```cmake
cmake_minimum_required(VERSION 3.21)
project("Bencode_Lib Embedded Example" LANGUAGES CXX)

set(BENCODE_EMBEDDED_MODE ON CACHE BOOL "Enable embedded build mode")
set(BENCODE_ENABLE_EXCEPTIONS OFF CACHE BOOL "Disable exceptions")
set(BENCODE_ENABLE_FILE_IO OFF CACHE BOOL "Disable file I/O")
set(BENCODE_ENABLE_DYNAMIC_ALLOCATION OFF CACHE BOOL "Disable dynamic allocation")
set(BENCODE_MAX_NODE_COUNT 128 CACHE STRING "Maximum number of nodes")
set(BENCODE_MAX_CONTAINER_SIZE 32 CACHE STRING "Maximum number of list/dictionary entries")
set(BENCODE_MAX_STRING_LENGTH 1024 CACHE STRING "Maximum string length")

add_subdirectory(../.. bencode-lib)

add_executable(EmbeddedExample main.cpp)
target_link_libraries(EmbeddedExample PRIVATE Bencode_Lib::Bencode_Lib_Embedded)
```

## Validation

Embedded mode is validated by building the embedded library and its embedded unit test and benchmark targets. The embedded configuration should compile cleanly and produce a minimal buffer-only runtime.
