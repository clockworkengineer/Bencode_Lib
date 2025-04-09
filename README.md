# Bencode_Lib
**Bencode_Lib** is a lightweight and efficient C++ library designed to work with the [Bencode format](https://en.wikipedia.org/wiki/Bencode). The library supports Bencode stringification and parsing from various data sources, including byte buffers, files, or custom sources/destinations configured using the provided `ISource` and `IDestination` interfaces.
## Features
- **Parsing**: Decode Bencode data from files, memory buffers, or custom streams.
- **Encoding**: Generate Bencode-compatible data from C++ structures.
- **Tree Structure Handling**:
    - Interrogate the `BNode` structure after parsing.
    - Modify the BNode tree structure directly in-memory.
    - Construct new BNode trees from scratch using C++ operators.

- **Interface Customization**: Use `ISource` and `IDestination` interfaces to integrate custom input/output sources.
- **Error Reporting**: Detect and report incorrect Bencode syntax (note: the library is lightweight, and specific error details are not included to reduce overhead).

## Why Use Bencode_Lib?
Bencode_Lib is ideal for simple and lightweight applications that require Bencode encoding/decoding functionality. It provides flexibility for users who want to work with custom data streams while maintaining compliance with the Bencode specification.
## Getting Started
### Prerequisites
- **C++20 or later**: The library uses modern C++ features and requires a compatible compiler.
- A basic understanding of the Bencode serialization format. Refer to [Bencode Wiki](https://en.wikipedia.org/wiki/Bencode) for detailed specifications.

### Installation
1. Clone this repository:
``` bash
   git clone <repo-url>
```
1. Add the project to your CMake configuration:
``` cmake
   add_subdirectory(Bencode_Lib)
   target_link_libraries(YourProject PRIVATE Bencode_Lib)
```
1. Make sure to compile your project with a C++20 compatible compiler.

## Usage
### Parsing Bencode Data
Use the provided interfaces to parse Bencode data from buffers, files, or custom input streams.
### Encoding Bencode Data
Construct a `BNode` structure using C++ operators, customize and modify it if needed, and encode it into a Bencode-compliant format.
### Examples
For examples on using `Bencode_Lib` to parse and encode data, refer to the [examples](examples/) directory or check the documentation comments within the source code.
## Limitations
- While the library identifies syntax errors in Bencode data, it does not provide specific details regarding the errors to optimize parsing performance.
- Currently, Bencode_Lib does not include utilities for managing highly complex encoding scenarios.

## Contributing
Contributions to **Bencode_Lib** are welcome! To contribute, follow these steps:
1. Fork the repository and create a new branch.
2. Commit your changes and ensure the code passes all tests.
3. Submit a pull request with a detailed description of the changes you made.

## License
This project is distributed under the MIT License. See the [LICENSE](LICENSE) file for more details.
## References
- [Bencode Format - Wikipedia](https://en.wikipedia.org/wiki/Bencode)

**Bencode_Lib** – Simplifying Bencode for C++ Developers
