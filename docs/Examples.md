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

## Writing raw Bencode to a file
```cpp
#include "Bencode.hpp"
using namespace Bencode_Lib;

Bencode::toFile("output.bencode", "d3:foo3:bare");
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
#include <memory>

class MyParser : public Bencode_Lib::IParser {
public:
    Node parseImpl(ISource &source) override {
        // Custom parsing logic
        return Node::make<Integer>(123);
    }
};
Bencode doc(nullptr, std::make_unique<MyParser>());
```

## Optional Stringifier Example
```cpp
#include "Bencode.hpp"
#include "Bencode_Optional_Stringify.hpp"
using namespace Bencode_Lib;

// Only compile this example when JSON stringify is enabled.
Bencode doc(makeStringify<JSON_Stringify>());
```

## Minimal Build Example

In a minimal build, file-based helpers and built-in stringify modules are disabled. Use buffer-based I/O and custom extension points instead.

```bash
cmake .. -DBENCODE_BUILD_MINIMAL=ON \
  -DBENCODE_ENABLE_FILE_IO=OFF \
  -DBENCODE_ENABLE_JSON_STRINGIFY=OFF \
  -DBENCODE_ENABLE_XML_STRINGIFY=OFF \
  -DBENCODE_ENABLE_YAML_STRINGIFY=OFF
```

```cpp
#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Status.hpp"
using namespace Bencode_Lib;

class MyStringify : public IStringify {
public:
    void stringify(const Node &bNode, IDestination &destination) const override {
        destination.add("minimal output");
    }
};

Bencode doc(makeStringify<MyStringify>());
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
Bencode doc(makeStringify<MyStringify>());
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
MyAction action;
doc.traverse(action);
```

---
See the Guide and API reference for more usage details.