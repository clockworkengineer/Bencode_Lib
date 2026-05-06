# Bencode_Lib User Guide

This guide explains how to use the Bencode_Lib library to parse, create, and manipulate Bencode data, with practical examples and advanced usage notes.

## Getting Started

1. **Include the library headers:**
   ```cpp
   #include "Bencode.hpp"
   ```
2. **Link against the library** (if building separately).

## Quick Start

1. Create a build directory and configure the library:
   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build .
   ```
2. Install the library if desired:
   ```bash
   cmake --install .
   ```
3. Consume the installed package from another project:
   ```cmake
   find_package(Bencode_Lib REQUIRED)
   target_link_libraries(my_target PRIVATE Bencode_Lib::Bencode_Lib)
   ```

## Public Header Boundary

Only the public root headers are intended for consumer inclusion:

- `Bencode.hpp`
- `Bencode_Core.hpp`
- `Bencode_Optional_Stringify.hpp`

All implementation-specific headers remain hidden from public include paths.

For installation and `find_package()` usage, see `docs/Package.md`.

## Parsing Bencode Data
```cpp
#include "Bencode.hpp"
using namespace Bencode_Lib;

std::string bencoded = "d3:foo3:bare";
Bencode doc(bencoded);
const Node &root = doc.root();
```
- The root node can be a dictionary, list, integer, or string.
- Use `isA<T>(node)` and `NRef<T>(node)` to check/cast node types.

## Creating Bencode Data
```cpp
Bencode::DictionaryInitializerType dict = { {"foo", Node::make<String>("bar")} };
Bencode doc(dict);
```
- You can also use `ListInitializerType` for lists.

## Accessing Data
```cpp
const Node &foo = doc["foo"];
std::string value = NRef<String>(foo).value();
```
- Use `operator[]` for dictionary/list access.
- Use `NRef<T>(node)` to get a reference to the underlying type.

## Writing/Reading from Files
```cpp
std::string fileData = Bencode::fromFile("example.torrent");
Bencode doc(fileData);
Bencode::toFile("output.torrent", doc.root().toString());
```

> Note: file-based helpers require `BENCODE_ENABLE_FILE_IO=ON`.

## Error Handling
All parsing errors throw `SyntaxError` exceptions. Use try/catch to handle errors.
```cpp
try {
    Bencode doc("invalid_data");
} catch (const SyntaxError &e) {
    std::cerr << "Parse error: " << e.what() << std::endl;
}
```

## Custom Parsing
You can implement your own parser by inheriting from `IParser` and passing it to the `Bencode` constructor.
```cpp
class MyParser : public Bencode_Lib::IParser {
public:
    Node parse(ISource &source) override {
        // Custom parsing logic
    }
};
Bencode doc(nullptr, new MyParser());
```

## Custom Stringify
Implement `IStringify` for custom output formats (e.g., JSON, XML).
```cpp
class MyStringify : public Bencode_Lib::IStringify {
public:
    void stringify(const Node &bNode, IDestination &destination) const override {
        // Custom encoding logic
    }
};
Bencode doc(makeStringify<MyStringify>());
```

## Embedded Mode
For embedded builds, see `docs/EMBEDDED.md` for configuration details, no-exceptions usage, and fixed-capacity container guidance.

In embedded mode, use `BufferSource` and `BufferDestination` instead of file-based I/O, and link against `Bencode_Lib::Bencode_Lib_Embedded` for the embedded library variant.

## Build Profiles

Recommended CMake profiles:

- **Default**: `cmake .. -DCMAKE_BUILD_TYPE=Release`
- **Minimal**: `cmake .. -DBENCODE_BUILD_MINIMAL=ON -DBENCODE_ENABLE_FILE_IO=OFF -DBENCODE_ENABLE_JSON_STRINGIFY=OFF -DBENCODE_ENABLE_XML_STRINGIFY=OFF -DBENCODE_ENABLE_YAML_STRINGIFY=OFF`
- **Embedded**: `cmake .. -DBENCODE_EMBEDDED_MODE=ON -DBENCODE_ENABLE_EXCEPTIONS=OFF -DBENCODE_ENABLE_DYNAMIC_ALLOCATION=OFF -DBENCODE_ENABLE_FILE_IO=OFF`

These profiles help keep the library small and appropriate for the target deployment environment.

## Advanced Tips
- Use `setMaxParserDepth()` and `setMaxStringLength()` to control parsing limits.
- Use `traverse()` to walk the node tree with an `IAction` implementation.
- See the API reference for more details and advanced features.

---
See the API reference and Examples for more usage details.