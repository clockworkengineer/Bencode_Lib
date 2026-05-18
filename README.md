# Bencode_Lib

Bencode_Lib is a C++23 library for encoding and decoding data using the Bencode format. Bencode is a simple and efficient serialization format commonly used in peer-to-peer (P2P) file sharing applications, such as BitTorrent.

## Features

- **Bencode Encoding**: Stringify typical data structures — strings, integers, lists, and dictionaries — into Bencode format.
- **Bencode Decoding**: Parse Bencode-formatted data into a native C++ node tree.
- **Flexible I/O**: Read and write Bencode via in-memory buffers (`BufferSource`/`BufferDestination`) or files (`FileSource`/`FileDestination`). The buffer adapters are optimized for direct memory copy when transferring bytes.
- **Custom Stringifiers**: Built-in support for converting Bencode to JSON, XML, and YAML via pluggable stringify interfaces.
- **Custom Extension Points**: Implement `IParser` or `IStringify` directly to customize parsing or output formatting in any build variant.
- **Runtime Construction**: Build Bencode trees programmatically using operator overloading and initializer lists.
- **Traversal**: Walk the node tree with a custom `IAction` visitor.
- **Error Handling**: Parsing and stringification errors throw typed exceptions with descriptive messages.

## Library Design Principles

Bencode_Lib is designed around the following attributes of a high-quality library:

- **Intuitive API Design:** clean root headers, explicit optional feature imports, and consistent public interfaces.
- **Comprehensive Documentation:** README, guide, API reference, package docs, and examples document usage and build variants.
- **High Reliability:** deterministic parse/stringify behavior, clear error reporting, and stable build profiles.
- **Performance and Efficiency:** buffer-based I/O, minimal build variants, and optional extensions only when enabled.
- **Maintainability:** private implementation headers, consistent CMake target boundaries, and modular build options.
- **Flexibility and Customization:** pluggable `IStringify`/`IParser` extension points and optional JSON/XML/YAML stringify modules.
- **Strong Security:** explicit parser limits and safe behavior when file I/O or exceptions are disabled.
- **High Testability:** built-in unit tests, public header compile checks, and dedicated variant coverage.
- **Compatibility and Portability:** modern C++23 support across GCC, Clang, and MSVC with platform-specific I/O backends.
- **Low Dependency Footprint:** optional features are gated so consumers only use the code they need.

## Security Guarantees

- When `BENCODE_ENABLE_EXCEPTIONS=OFF`, parse operations return a `ParseStatus` result instead of throwing exceptions.
- No-exceptions mode is explicitly tested for failure reporting, so invalid or malformed input is handled safely.
- When `BENCODE_ENABLE_FILE_IO=OFF`, file-based helpers are disabled at compile time and `fromFile()`/`toFile()` calls fail cleanly through the disabled file I/O implementation.
- The library avoids hidden file I/O or exception assumptions in minimal and embedded build profiles.
- Embedded mode additionally enforces bounded container sizes and deterministic failure behavior for reduced-risk environments.

## Quick Start

1. Create a build directory and configure the project:
   ```bash
   mkdir -p build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release -DBENCODE_BUILD_TESTS=ON -DBENCODE_BUILD_EXAMPLES=ON
   cmake --build .
   ```
2. Install the library (optional):
   ```bash
   cmake --install .
   ```
3. Link against the installed target:
   ```cmake
   find_package(Bencode_Lib REQUIRED)
   target_link_libraries(my_target PRIVATE Bencode_Lib::Bencode_Lib)
   ```
4. In your code include only the public root header:
   ```cpp
   #include "Bencode.hpp"
   ```

> Use `Bencode_Core.hpp` when you need node types, buffer I/O helpers, or low-level access. Keep implementation headers private.
>
> Optional stringify helpers are available via `Bencode_Optional_Stringify.hpp` when the corresponding CMake options are enabled.
>
## Recommended Build Profiles

- **Default**: `-DBENCODE_BUILD_TESTS=ON -DBENCODE_BUILD_EXAMPLES=ON`
- **Minimal**: `-DBENCODE_BUILD_MINIMAL=ON -DBENCODE_ENABLE_FILE_IO=OFF -DBENCODE_ENABLE_JSON_STRINGIFY=OFF -DBENCODE_ENABLE_XML_STRINGIFY=OFF -DBENCODE_ENABLE_YAML_STRINGIFY=OFF`
- **Embedded**: `-DBENCODE_EMBEDDED_MODE=ON -DBENCODE_ENABLE_EXCEPTIONS=OFF -DBENCODE_ENABLE_DYNAMIC_ALLOCATION=OFF -DBENCODE_ENABLE_FILE_IO=OFF`

## Public Header Boundary

The public API is exposed through the top-level headers under `classes/include`. Consumers should include only:

- `Bencode.hpp`
- `Bencode_Core.hpp`
- `Bencode_Optional_Stringify.hpp` (optional helper)

Internal implementation headers are intentionally hidden from public include paths.

## Compatibility and Portability

Bencode_Lib is designed to compile cleanly with modern C++23 toolchains on:

- GCC
- Clang
- MSVC

The build supports both Debug and Release profiles, and all library targets request C++23 via `target_compile_features(... PUBLIC cxx_std_23)`.

The library selects file I/O implementation automatically:

- `classes/source/implementation/file/Bencode_File_POSIX.cpp` for POSIX platforms
- `classes/source/implementation/file/Bencode_File_MSVC.cpp` for MSVC
- `classes/source/implementation/file/Bencode_File_Disabled.cpp` when `BENCODE_ENABLE_FILE_IO=OFF`

Build variants are exposed as separate targets in installed CMake package configuration files:

- `Bencode_Lib::Bencode_Lib`
- `Bencode_Lib::Bencode_Lib_Minimal`
- `Bencode_Lib::Bencode_Lib_Embedded`

The package config files are installed under `lib/cmake/Bencode_Lib/`, and downstream consumers can select the appropriate variant regardless of the build profile.

The `BENCODE_BUILD_MINIMAL` variant disables optional stringifiers and file-based I/O, while `BENCODE_EMBEDDED_MODE` additionally disables exceptions and dynamic allocation.

## What is Bencode?

Bencode is a data serialization format with the following encoding rules:

1. **Strings**: Represented as `<length>:<data>`.  
   Example: `4:spam` represents the string `"spam"`.
2. **Integers**: Represented as `i<number>e`.  
   Example: `i42e` represents the integer `42`.
3. **Lists**: Represented as `l<items>e`.  
   Example: `l4:spami42ee` represents the list `["spam", 42]`.
4. **Dictionaries**: Represented as `d<pairs>e` (keys are strings, pairs are sorted key-value).  
   Example: `d3:cow3:moo4:spam4:eggse` represents the dictionary `{"cow": "moo", "spam": "eggs"}`.

## Installation

### Prerequisites

- A C++23 compatible compiler (GCC 13+, Clang 16+, MSVC 19.35+)
- CMake 3.21 or later
- Make, Ninja, or Visual Studio (build system)

### Building the Library

1. Clone the repository:
   ```bash
   git clone <repository-url> Bencode_Lib
   cd Bencode_Lib
   ```

2. Create a build directory:
   ```bash
   mkdir -p build
   cd build
   ```

3. Configure and compile the project:
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build .
   ```

   The default build includes unit tests, examples, and benchmarks. To customize the build, pass CMake options such as `-DBENCODE_BUILD_MINIMAL=ON`, `-DBENCODE_EMBEDDED_MODE=ON`, or `-DBENCODE_ENABLE_FILE_IO=OFF`.

4. (Optional) Install the library:
   ```bash
   cmake --install .
   ```

### Install and Export Usage

After installation, the library exports CMake package files under the install prefix. Use the installed target from a downstream project like this:

```cmake
find_package(Bencode_Lib REQUIRED)
add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE Bencode_Lib::Bencode_Lib)
```

To explicitly use the minimal or embedded variant:

```cmake
find_package(Bencode_Lib REQUIRED)
# Minimal variant
target_link_libraries(my_app PRIVATE Bencode_Lib::Bencode_Lib_Minimal)
# Embedded variant
target_link_libraries(my_app PRIVATE Bencode_Lib::Bencode_Lib_Embedded)
```

If you install to a custom prefix, use `-DCMAKE_INSTALL_PREFIX=/your/install/path` when configuring the build.

### CI / Continuous Integration

For CI pipelines, use a clean out-of-source build and run the full test suite with output on failure:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target Bencode_Lib_Unit_Tests
ctest --test-dir build --output-on-failure
```

To verify the public header boundary in CI, add:

```bash
cmake --build build --target Bencode_Lib_PublicHeader_CompileTest
ctest -R Bencode_Lib_PublicHeader_CompileTest --output-on-failure
```

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `BENCODE_BUILD_TESTS` | `ON` | Build the Catch2 unit tests |
| `BENCODE_BUILD_EXAMPLES` | `ON` | Build the example programs |
| `BENCODE_BUILD_BENCHMARKS` | `ON` | Build the performance benchmark executable |
| `BENCODE_BUILD_MINIMAL` | `OFF` | Build a smaller, buffer-only library variant without optional file I/O or stringifiers |
| `BENCODE_EMBEDDED_MODE` | `OFF` | Enable the embedded build configuration |
| `BENCODE_ENABLE_EXCEPTIONS` | `ON` | Enable exception support |
| `BENCODE_ENABLE_FILE_IO` | `ON` | Enable file-based I/O support |
| `BENCODE_ENABLE_JSON_STRINGIFY` | `ON` | Enable JSON stringify support |
| `BENCODE_ENABLE_XML_STRINGIFY` | `ON` | Enable XML stringify support |
| `BENCODE_ENABLE_YAML_STRINGIFY` | `ON` | Enable YAML stringify support |
| `BENCODE_ENABLE_DYNAMIC_ALLOCATION` | `ON` | Enable dynamic allocations for containers |
| `BENCODE_MAX_NODE_COUNT` | `256` | Maximum total node count in embedded mode |
| `BENCODE_MAX_CONTAINER_SIZE` | `64` | Maximum number of elements in embedded lists/dictionaries |
| `BENCODE_MAX_STRING_LENGTH` | `16384` | Maximum string length in embedded mode |
| `BENCODE_WARNINGS_AS_ERRORS` | `OFF` | Treat compiler warnings as errors |

For embedded mode details, see `docs/EMBEDDED.md`.

For package and installation guidance, see `docs/Package.md`.

### Consuming via CMake

After installation, link against the library using its namespaced target:

```cmake
find_package(Bencode_Lib REQUIRED)
target_link_libraries(my_target PRIVATE Bencode_Lib::Bencode_Lib)
```

## Usage

All public API lives in the `Bencode_Lib` namespace. Include `Bencode.hpp` for the main class and `Bencode_Core.hpp` for node types and I/O helpers.

### Parsing (Decoding)

```cpp
#include "Bencode.hpp"
#include "Bencode_Core.hpp"

namespace be = Bencode_Lib;

int main() {
    // Parse from an in-memory buffer
    be::Bencode bencode;
    be::BufferSource source("d3:age2:303:name4:Johne");
    bencode.parse(source);

    // Access dictionary entries
    std::cout << be::NRef<be::String>(bencode["name"]).value() << "\n"; // John
    std::cout << be::NRef<be::String>(bencode["age"]).value()  << "\n"; // 30
}
```

### Stringifying (Encoding)

```cpp
#include "Bencode.hpp"
#include "Bencode_Core.hpp"

namespace be = Bencode_Lib;

int main() {
    be::Bencode bencode;
    bencode["name"] = std::string("John");
    bencode["age"]  = std::string("30");

    be::BufferDestination destination;
    bencode.stringify(destination);
    std::cout << destination.toString() << "\n"; // d3:age2:303:name4:Johne
}
```

### Library Version

```cpp
#include "Bencode.hpp"

namespace be = Bencode_Lib;

int main() {
    std::cout << "Bencode_Lib version: " << be::Bencode::version() << "\n";
}
```

### File I/O

```cpp
// Read from file
be::Bencode bencode;
bencode.parse(be::FileSource("data.torrent"));

// Write to file
be::BufferDestination destination;
bencode.stringify(destination);
be::Bencode::toFile("output.bencode", destination.toString());

// Convenience helper
std::string raw = be::Bencode::fromFile("data.torrent");
```

### Runtime Construction

Bencode trees can be built at runtime using operator[] and initializer lists:

```cpp
be::Bencode bencode;
bencode["pi"]         = 3.141;
bencode["flag"]       = true;
bencode["name"]       = std::string("Alice");
bencode["nothing"]    = nullptr;
bencode["list"]       = {1, 0, 2};
bencode["object"]     = {{"currency", "USD"}, {"value", 42.99}};

// Or via a single initializer list
const be::Bencode bencode2 = {
    {"pi",   3.141},
    {"name", std::string("Alice")},
    {"list", be::Node{1, 0, 2}}
};
```

### Custom Stringifiers (JSON / XML / YAML)

```cpp
#include "JSON_Stringify.hpp"

// Construct with a custom stringify; parse a torrent and output JSON
const be::Bencode bencode(be::makeStringify<be::JSON_Stringify>());
bencode.parse(be::FileSource("data.torrent"));
bencode.stringify(be::FileDestination("data.json"));
```

Built-in stringifiers: `JSON_Stringify`, `XML_Stringify`, `YAML_Stringify`.

## API Overview

### `Bencode` class

| Member | Description |
|--------|-------------|
| `Bencode()` | Default constructor; empty tree |
| `Bencode(string_view)` | Parse directly from a Bencode string |
| `Bencode(ListInitializerType)` | Construct a list |
| `Bencode(DictionaryInitializerType)` | Construct a dictionary |
| `void parse(ISource &)` | Parse Bencode from a source |
| `void stringify(IDestination &)` | Encode tree to a destination |
| `Node &root()` | Access the root node |
| `Node &operator[](string_view)` | Index into a dictionary |
| `Node &operator[](size_t)` | Index into a list |
| `void traverse(IAction &)` | Walk the node tree |
| `static string version()` | Library version string |
| `static string fromFile(string_view)` | Read raw Bencode from a file |
| `static void toFile(string_view, string_view)` | Write raw Bencode to a file |

### Node Types

| Type alias | C++ type | Accessor |
|------------|----------|----------|
| `Integer` | `int64_t` | `NRef<Integer>(node).value()` |
| `String` | `std::string` | `NRef<String>(node).value()` |
| `List` | `std::vector<Node>` | `NRef<List>(node).value()` |
| `Dictionary` | `std::map<std::string, Node>` | `NRef<Dictionary>(node).value()` |

Use `isA<T>(node)` to check a node's type before casting.

### I/O Helpers

- `BufferSource(string)` — parse from an in-memory string.
- `FileSource(path)` — parse from a file.
- `BufferDestination` — stringify to an in-memory buffer; retrieve with `.toString()`.
- `FileDestination(path)` — stringify directly to a file.

### Error Handling

All errors are thrown as exceptions. Wrap calls in a try/catch block:

```cpp
try {
    bencode.parse(be::BufferSource("invalid"));
} catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << "\n";
}
```

## Testing

The test suite uses [Catch2](https://github.com/catchorg/Catch2) and is fetched automatically by CMake.

```bash
cmake .. -DBENCODE_BUILD_TESTS=ON
cmake --build .
ctest --output-on-failure
```

## Contributing

1. Fork the repository and create a branch for your feature or bugfix.
2. Submit a pull request with a clear description of the change.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- Inspired by the simplicity of the Bencode format used in BitTorrent.
- Built with modern C++23 standards.