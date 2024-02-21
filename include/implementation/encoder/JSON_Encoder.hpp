#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "JSON_Translator.hpp"

namespace Bencode_Lib {

class JSON_Encoder : public IEncoder, protected JSON_Translator {

public:
  // Constructors/destructors
  explicit JSON_Encoder(ITranslator &translator) : jsonTranslator(translator) {}
  explicit JSON_Encoder(ITranslator &&translator)
      : jsonTranslator(translator) {}
  JSON_Encoder(const JSON_Encoder &other) = delete;
  JSON_Encoder &operator=(const JSON_Encoder &other) = delete;
  JSON_Encoder(JSON_Encoder &&other) = delete;
  JSON_Encoder &operator=(JSON_Encoder &&other) = delete;
  ~JSON_Encoder() = default;

  void encode(const BNode &bNode, IDestination &destination) const {
    if (bNode.isDictionary()) {
      destination.add('{');
      int commas = BRef<Dictionary>(bNode).value().size();
      for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
        destination.add("\"" + bNodeNext.getKey() + "\"" + " : ");
        encode(bNodeNext.getBNode(), destination);
        if (--commas > 0)
          destination.add(",");
      }
      destination.add('}');
    } else if (bNode.isList()) {
      int commas = BRef<List>(bNode).value().size();
      destination.add('[');
      for (const auto &bNodeNext : BRef<List>(bNode).value()) {
        encode(bNodeNext, destination);
        if (--commas > 0)
          destination.add(",");
      }
      destination.add(']');
    } else if (bNode.isInteger()) {
      destination.add(std::to_string(BRef<Integer>(bNode).value()));
    } else if (bNode.isString()) {
      destination.add("\"");
      destination.add(jsonTranslator.to(BRef<String>(bNode).value()));
      destination.add("\"");
    }
  }

private:
  ITranslator &jsonTranslator;
};

} // namespace Bencode_Lib
