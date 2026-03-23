# Bencode_Lib API Reference

This document provides a detailed overview of the main classes, types, and functions in the Bencode_Lib library, including usage notes and error handling.

## Namespaces
- `Bencode_Lib` — All public API is contained within this namespace.

## Core Classes and Interfaces

### Bencode
Represents a Bencode document (the root node of a Bencode tree).

**Type Aliases:**
- `IntegerType` — `int64_t`, used for integer values.
- `StringType` — `std::string`, used for string values.
- `ListType` — `std::vector<Node>`, used for lists.
- `DictionaryType` — `std::map<std::string, Node>`, used for dictionaries.
- `ListInitializerType` — `std::initializer_list<InitializerListTypes>`
- `DictionaryInitializerType` — `std::initializer_list<std::pair<std::string, InitializerListTypes>>`

**Constructors:**
- `Bencode([[maybe_unused]] IStringify *stringify = nullptr, [[maybe_unused]] IParser *parser = nullptr)` — Create with optional custom stringify/parser.
- `explicit Bencode(const std::string_view &bencodeString)` — Parse from a Bencode string.
- `Bencode(const ListInitializerType &list)` — Create from a list.
- `Bencode(const DictionaryInitializerType &dictionary)` — Create from a dictionary.

**Key Methods:**
- `void parse(ISource &source) const` — Parse Bencode from a source stream.
- `void stringify(IDestination &destination) const` — Write Bencode to a destination stream.
- `Node &root()` / `const Node &root() const` — Access the root node.
- `Node &operator[](const std::string_view &key)` — Access dictionary/list elements.
- `static std::string version()` — Get library version string.
- `static std::string fromFile(const std::string_view &fileName)` — Read Bencode from file.
- `static void toFile(const std::string_view &fileName, const std::string_view &bencodeString)` — Write Bencode to file.

**Usage Notes:**
- The root node can be a dictionary, list, integer, or string.
- Use `NRef<T>(node)` or `isA<T>(node)` to cast or check node types.

### Node
Represents a value in the Bencode tree (integer, string, list, dictionary).

**Key Methods:**
- `Type getNodeType() const` — Get the type of node (enum: integer, string, list, dictionary).
- `template <typename T> T &as()` — Cast to a specific type.
- `bool isEmpty() const` — Check if the node is empty.
- `Variant &getVariant()` / `const Variant &getVariant() const` — Access the underlying variant.
- `static Node make<T>(Args &&...args)` — Create a node of type T.

### IParser
Interface for custom parsers.

**Key Method:**
- `virtual Node parse(ISource &source) = 0;`

**Usage:**
- Implement this interface to provide custom parsing logic.
- Pass your parser to the `Bencode` constructor.

### ISource / IDestination
Abstract interfaces for reading/writing data.

- `ISource` provides `char current()`, `void next()`, `bool more()`.
- `IDestination` provides `void add(const std::string &bytes)`, etc.

### IStringify
Interface for custom stringification (encoding) logic.

- `virtual void stringify(const Node &bNode, IDestination &destination) const = 0;`
- Implement and pass to `Bencode` for custom output formats.

## Variants
- `Integer`, `String`, `List`, `Dictionary` — Node types, each with their own value accessors and constructors.
- Example: `Integer::value()`, `String::value()`, `List::add()`, `Dictionary::add()`

## Error Handling
- All parsing errors throw `SyntaxError` (or `IParser::Error`) exceptions.
- Stringification errors throw `IStringify::Error`.
- Node errors throw `Node::Error`.
- Use try/catch to handle errors:
  ```cpp
  try {
      Bencode doc("invalid_data");
  } catch (const SyntaxError &e) {
      std::cerr << "Parse error: " << e.what() << std::endl;
  }
  ```

## Example Usage
```cpp
#include "Bencode.hpp"
using namespace Bencode_Lib;

std::string bencoded = "d3:foo3:bare";
Bencode doc(bencoded);
const Node &root = doc.root();
if (isA<Dictionary>(root)) {
    auto &dict = NRef<Dictionary>(root);
    // ...
}
```

---
For more details, see the user guide and the source code.