# Bencode_Lib

Bencode_Lib is a C++23 library for encoding and decoding data using the Bencode format. Bencode is a simple and efficient serialization format commonly used in peer-to-peer (P2P) file sharing applications, such as BitTorrent.

## Features

- **Bencode Encoding**: Stringify typical data structures — strings, integers, lists, and dictionaries — into Bencode format.
- **Bencode Decoding**: Parse Bencode-formatted data into a native C++ node tree.
- **Flexible I/O**: Read and write Bencode via in-memory buffers (`BufferSource`/`BufferDestination`) or files (`FileSource`/`FileDestination`).
- **Custom Stringifiers**: Built-in support for converting Bencode to JSON, XML, and YAML via pluggable stringify interfaces.
- **Runtime Construction**: Build Bencode trees programmatically using operator overloading and initializer lists.
- **Traversal**: Walk the node tree with a custom `IAction` visitor.
- **Error Handling**: Parsing and stringification errors throw typed exceptions with descriptive messages.

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
   mkdir build
   cd build
   ```

3. Configure and compile the project:
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build .
   ```

4. (Optional) Install the library:
   ```bash
   cmake --install .
   ```

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `BENCODE_BUILD_TESTS` | `ON` | Build the Catch2 unit tests |
| `BENCODE_BUILD_EXAMPLES` | `ON` | Build the example programs |
| `BENCODE_EMBEDDED_MODE` | `OFF` | Enable the embedded build configuration |
| `BENCODE_BUILD_MINIMAL` | `OFF` | Build a smaller, buffer-only library variant without optional file I/O or stringifiers |
| `BENCODE_ENABLE_EXCEPTIONS` | `ON` | Enable exception support |
| `BENCODE_ENABLE_FILE_IO` | `ON` | Enable file-based I/O support |
| `BENCODE_ENABLE_DYNAMIC_ALLOCATION` | `ON` | Enable dynamic allocations for containers |
| `BENCODE_MAX_NODE_COUNT` | `256` | Maximum total node count in embedded mode |
| `BENCODE_MAX_CONTAINER_SIZE` | `64` | Maximum number of elements in embedded lists/dictionaries |
| `BENCODE_MAX_STRING_LENGTH` | `16384` | Maximum string length in embedded mode |
| `BENCODE_WARNINGS_AS_ERRORS` | `OFF` | Treat compiler warnings as errors |

For embedded mode details, see `docs/EMBEDDED.md`.

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