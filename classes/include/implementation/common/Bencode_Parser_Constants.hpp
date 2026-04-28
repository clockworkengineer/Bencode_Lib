// File: Bencode_Parser_Constants.hpp
//
// Description: Constants and token definitions used by the Bencode parser implementation.
//

#pragma once

namespace Bencode_Lib {
namespace ParserConstants {
// Bencode type indicators
constexpr char DICTIONARY = 'd';
constexpr char LIST = 'l';
constexpr char INTEGER = 'i';
constexpr char STRING_0 = '0';
constexpr char STRING_1 = '1';
constexpr char STRING_2 = '2';
constexpr char STRING_3 = '3';
constexpr char STRING_4 = '4';
constexpr char STRING_5 = '5';
constexpr char STRING_6 = '6';
constexpr char STRING_7 = '7';
constexpr char STRING_8 = '8';
constexpr char STRING_9 = '9';
constexpr char STRING_MINUS = '-';
constexpr char STRING_PLUS = '+';
constexpr char COLON = ':';
constexpr char END = 'e';
// Parser settings
constexpr unsigned long DEFAULT_MAX_PARSER_DEPTH = 10;
} // namespace ParserConstants
} // namespace Bencode_Lib
