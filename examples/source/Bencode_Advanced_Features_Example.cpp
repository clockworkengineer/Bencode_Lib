// Program: Bencode_Advanced_Features_Example
// Description: Demonstrates advanced features like parser depth and custom error handling in Bencode_Lib.
// Dependencies: C++20, Bencode_Lib.

#include "Bencode.hpp"
#include "implementation/parser/Default_Parser.hpp"
#include "implementation/variants/Bencode_String.hpp"
#include "implementation/common/Bencode_Error.hpp"
#include <iostream>

namespace be = Bencode_Lib;

int main() {
    // Set parser limits
    be::Default_Parser::setMaxParserDepth(5);
    be::String::setMaxStringLength(100);

    // Try parsing a deeply nested structure
    std::string deep = "l";
    for (int i = 0; i < 10; ++i) deep += "l";
    deep += "i1e";
    for (int i = 0; i < 11; ++i) deep += "e";

    try {
        be::Bencode doc(deep);
        std::cout << "Parsed deeply nested list successfully." << std::endl;
    } catch (const be::SyntaxError &e) {
        std::cerr << "Caught SyntaxError: " << e.what() << std::endl;
    }

    // Try parsing an oversized string
    std::string big = std::to_string(200) + ":" + std::string(200, 'x');
    try {
        be::Bencode doc(big);
        std::cout << "Parsed large string successfully." << std::endl;
    } catch (const be::SyntaxError &e) {
        std::cerr << "Caught SyntaxError: " << e.what() << std::endl;
    }
    return 0;
}
