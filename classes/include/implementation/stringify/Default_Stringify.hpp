// File: Default_Stringify.hpp
//
// Description: Header declaring the default stringifier for serializing Bencode nodes back into text.
//

#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

#include <cstdint>
#include <string_view>

namespace Bencode_Lib {

class Default_Stringify final : public IStringify {
public:
  // Constructors/Destructors
  explicit Default_Stringify(std::unique_ptr<ITranslator> translator =
                                 std::make_unique<Default_Translator>())
      : bencodeTranslator(std::move(translator)) {}
  Default_Stringify(const Default_Stringify &other) = delete;
  Default_Stringify &operator=(const Default_Stringify &other) = delete;
  Default_Stringify(Default_Stringify &&other) = delete;
  Default_Stringify &operator=(Default_Stringify &&other) = delete;
  ~Default_Stringify() override = default;

  /// <summary>
  /// Recursively traverse Node structure encoding it into Bencode string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified
  /// Bencode.</param>
  void stringify(const Node &bNode, IDestination &destination) const override {
    stringifyNodes(bNode, destination);
  }

private:
  static void appendUnsigned(IDestination &destination,
                             unsigned long long value) {
    char buffer[32];
    int writeIndex = 0;
    if (value == 0) {
      buffer[writeIndex++] = '0';
    } else {
      char temp[32];
      int tempIndex = 0;
      while (value > 0) {
        temp[tempIndex++] = static_cast<char>('0' + (value % 10));
        value /= 10;
      }
      while (tempIndex > 0) {
        buffer[writeIndex++] = temp[--tempIndex];
      }
    }
    destination.add(std::string_view(buffer, writeIndex));
  }

  static unsigned long long integerToUnsigned(Bencode::IntegerType value) {
    if (value < 0) {
      return static_cast<unsigned long long>(-(value + 1)) + 1ull;
    }
    return static_cast<unsigned long long>(value);
  }

  static void appendInteger(IDestination &destination,
                            Bencode::IntegerType value) {
    if (value < 0) {
      destination.add('-');
      appendUnsigned(destination, integerToUnsigned(value));
      return;
    }
    appendUnsigned(destination, integerToUnsigned(value));
  }

  static void appendSize(IDestination &destination, std::size_t value) {
    appendUnsigned(destination, static_cast<unsigned long long>(value));
  }

  static void stringifyNodes(const Node &bNode, IDestination &destination) {
    if (isA<Dictionary>(bNode)) {
      stringifyDictionary(bNode, destination);
    } else if (isA<List>(bNode)) {
      stringifyList(bNode, destination);
    } else if (isA<Integer>(bNode)) {
      stringifyInteger(bNode, destination);
    } else if (isA<String>(bNode)) {
      stringifyString(bNode, destination);
    } else if (isA<Hole>(bNode)) {
    } else {
      throw Error("Unknown Node type encountered during encoding.");
    }
  }
  static void stringifyDictionary(const Node &bNode,
                                  IDestination &destination) {
    destination.add('d');
    for (const auto &bNodeNext : NRef<Dictionary>(bNode).value()) {
      appendSize(destination, bNodeNext.getKey().length());
      destination.add(':');
      destination.add(bNodeNext.getKey());
      stringifyNodes(bNodeNext.getNode(), destination);
    }
    destination.add('e');
  }
  static void stringifyList(const Node &bNode, IDestination &destination) {
    destination.add('l');
    for (const auto &bNodeNext : NRef<List>(bNode).value()) {
      stringifyNodes(bNodeNext, destination);
    }
    destination.add('e');
  }
  static void stringifyInteger(const Node &bNode, IDestination &destination) {
    destination.add('i');
    appendInteger(destination, NRef<Integer>(bNode).value());
    destination.add('e');
  }
  static void stringifyString(const Node &bNode, IDestination &destination) {
    appendSize(destination, NRef<String>(bNode).value().length());
    destination.add(':');
    destination.add(NRef<String>(bNode).value());
  }

  [[maybe_unused]] std::unique_ptr<ITranslator> bencodeTranslator;
};
} // namespace Bencode_Lib
