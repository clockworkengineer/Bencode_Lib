# Bencode_Lib User Guide

This guide explains how to use the Bencode_Lib library to parse, create, and manipulate Bencode data, with practical examples and advanced usage notes.

## Getting Started

1. **Include the library headers:**
   ```cpp
   #include "Bencode.hpp"
   ```
2. **Link against the library** (if building separately).

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

## Advanced Tips
- Use `setMaxParserDepth()` and `setMaxStringLength()` to control parsing limits.
- Use `traverse()` to walk the node tree with an `IAction` implementation.
- See the API reference for more details and advanced features.

---
See the API reference and Examples for more usage details.