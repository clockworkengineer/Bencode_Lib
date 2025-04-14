# Bencode_Lib

Bencode_Lib is a C++20 library for encoding and decoding data using the Bencode format. Bencode is a simple and efficient serialization format commonly used in peer-to-peer (P2P) file sharing applications, such as BitTorrent.

## Features

- **Bencode Encoding**: Serialize typical data structures such as strings, integers, lists, and dictionaries into Bencode format.
- **Bencode Decoding**: Parse Bencode-formatted strings into native C++ data structures.
- **Lightweight and Fast**: The library is optimized for performance and follows modern C++20 standards.
- **Simple and Intuitive API**: Convenient interfaces for encoding/decoding common data types.
- **Error Handling**: Provides clear error messages and safe decoding for malformed input.

## What is Bencode?

Bencode is a data serialization format with the following encoding rules:

1. **Strings**: Represented as `<length>:<data>`.  
   Example: `4:spam` represents the string `"spam"`.
2. **Integers**: Represented as `i<number>e`.  
   Example: `i42e` represents the integer `42`.
3. **Lists**: Represented as `l<items>e`.  
   Example: `l4:spami42ee` represents the list `["spam", 42]`.
4. **Dictionaries**: Represented as `d<pairs>e` (keys are strings, pairs are key-value).  
   Example: `d3:cow3:moo4:spam4:eggse` represents the dictionary `{"cow": "moo", "spam": "eggs"}`.

## Installation

### Prerequisites
To build and install Bencode_Lib, you need the following tools:

- A C++20 compatible compiler (e.g., GCC, Clang)
- `CMake` 3.20 or later
- Make or Ninja (build system)

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
   sudo cmake --install .
   ```

## Usage

You can use Bencode_Lib to encode and decode data in the Bencode format. Below is a minimal example:

### Encoding Example
```cpp
#include "BencodeLib.h"

int main() {
    // Example to encode a dictionary
    std::unordered_map<std::string, std::string> dictionary = {
        {"name", "John"},
        {"age", "30"}
    };
    
    std::string bencoded = Bencode::encode(dictionary);
    std::cout << "Bencoded string: " << bencoded << std::endl;
    return 0;
}
```

### Decoding Example
```cpp
#include "BencodeLib.h"

int main() {
    // Example of decoding a Bencoded string
    std::string bencoded = "d3:age2:303:name4:Johne";
    
    auto decoded = Bencode::decode(bencoded);
    // Process the decoded data...
    return 0;
}
```

## API Documentation

### Encoding
- `std::string encode(const std::string&)`
- `std::string encode(int)`
- `std::string encode(const std::vector<T>&)` — Encodes a list of elements.
- `std::string encode(const std::unordered_map<std::string, T>&)` — Encodes a dictionary.

### Decoding
- `std::variant<std::string, int, std::vector<any>, std::unordered_map<std::string, any>> decode(const std::string&)`

Refer to the inline documentation or source code for more details.

## Testing

To run unit tests:

1. Ensure that you configured the project with `BUILD_TESTS` enabled:
   ```bash
   cmake .. -DBUILD_TESTS=ON
   ```

2. Build and execute the tests:
   ```bash
   cmake --build .
   ./run_tests
   ```

## Contributing

We welcome contributions! Please follow these steps:

1. Fork the repository and create a new branch for your feature or bugfix.
2. Submit a pull request with a detailed explanation of your changes.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- Inspired by the simplicity of the Bencode format used in BitTorrent.
- Built with modern C++20 standards for simplicity and performance.