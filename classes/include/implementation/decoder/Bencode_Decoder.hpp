#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

namespace Bencode_Lib {

class Bencode_Decoder final : public IDecoder {

public:
  // Constructors/Destructors
  Bencode_Decoder() = default;
  Bencode_Decoder(const Bencode_Decoder &other) = delete;
  Bencode_Decoder &operator=(const Bencode_Decoder &other) = delete;
  Bencode_Decoder(Bencode_Decoder &&other) = delete;
  Bencode_Decoder &operator=(Bencode_Decoder &&other) = delete;
  ~Bencode_Decoder() override = default;
  // Decode bencode BNode tree
  BNode decode(ISource &source) override;

private:
  // Decoder functions
  [[nodiscard]] static int64_t extractInteger(ISource &source);
  [[nodiscard]] static std::string extractString(ISource &source);
  [[nodiscard]] static BNode decodeString(ISource &source);
  [[nodiscard]] static BNode decodeInteger(ISource &source);
  [[nodiscard]] static BNode decodeDictionary(ISource &source);
  [[nodiscard]] static BNode decodeList(ISource &source);
  void static confirmBoundary(ISource &source, char expectedBoundary);
  [[nodiscard]] static BNode decodeTree(ISource &source);
  // Decoder routing tabke
  using DecoderFunc = std::function<BNode(ISource &)>;
  inline static std::map<char, DecoderFunc> decoders = {
      {'d', decodeDictionary}, {'l', decodeList},   {'i', decodeInteger},
      {'0', decodeString},     {'1', decodeString}, {'2', decodeString},
      {'3', decodeString},     {'4', decodeString}, {'5', decodeString},
      {'6', decodeString},     {'7', decodeString}, {'8', decodeString},
      {'9', decodeString}};
};

} // namespace Bencode_Lib