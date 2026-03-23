# Bencode_Lib Examples

## Minimal Encode/Decode
```cpp
#include "Bencode.hpp"
using namespace Bencode_Lib;

// Decode
std::string bencoded = "i42e";
Bencode doc(bencoded);
int value = NRef<Integer>(doc.root()).value();

// Encode
Bencode::ListInitializerType list = { Node::make<Integer>(1), Node::make<String>("foo") };
Bencode doc2(list);
```

## Parsing from a File
```cpp
#include "Bencode.hpp"
using namespace Bencode_Lib;

std::string fileData = Bencode::fromFile("example.torrent");
Bencode doc(fileData);
```

## Error Handling Example
```cpp
try {
    Bencode doc("invalid_data");
} catch (const SyntaxError &e) {
    std::cerr << "Parse error: " << e.what() << std::endl;
}
```

## Custom Parser Example
```cpp
class MyParser : public Bencode_Lib::IParser {
public:
    Node parse(ISource &source) override {
        // Custom parsing logic
        return Node::make<Integer>(123);
    }
};
Bencode doc(nullptr, new MyParser());
```

## Custom Stringify Example
```cpp
class MyStringify : public Bencode_Lib::IStringify {
public:
    void stringify(const Node &bNode, IDestination &destination) const override {
        // Custom encoding logic
        destination.add("custom output");
    }
};
Bencode doc(new MyStringify());
```

## Traversing the Node Tree
```cpp
class MyAction : public Bencode_Lib::IAction {
public:
    void onNode(const Node &bNode) override {
        // Handle node
    }
    void onString(const Node &bNode) override {
        // Handle string
    }
    // ...
};
Bencode doc("d3:foo3:bare");
doc.traverse(MyAction());
```

---
See the Guide and API reference for more usage details.