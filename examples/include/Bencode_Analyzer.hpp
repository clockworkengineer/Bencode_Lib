#pragma once

#include <set>
#include <sstream>

#include "Bencode.hpp"
#include "Bencode_Sources.hpp"
#include "Bencode_Destinations.hpp"
#include "Bencode_Core.hpp"

class Bencode_Analyzer : public Bencode_Lib::IAction
{
public:
  Bencode_Analyzer() = default;
  virtual ~Bencode_Analyzer() = default;
  // Add BNode details to analysis
  virtual void onBNode([[maybe_unused]] const Bencode_Lib::BNode &bNode) override { totalNodes++; }
  // Add string details to analysis
  virtual void onString(const Bencode_Lib::BNode &bNode) override
  {
    const Bencode_Lib::String &bNodeString = BRef<Bencode_Lib::String>(bNode);
    totalStrings++;
    sizeInBytes += sizeof(Bencode_Lib::String);
    sizeInBytes += bNodeString.value().size();
    maxStringSize = std::max(bNodeString.value().size(), maxKeySize);
    uniqueStrings.insert(bNodeString.value());
  }
  // Add number details to analysis
  virtual void onInteger([[maybe_unused]]const Bencode_Lib::BNode &bNode) override
  {
    totalIntegers++;
    sizeInBytes += sizeof(Bencode_Lib::Integer);
  }
  // Add array details to analysis
  virtual void onList(const Bencode_Lib::BNode &bNode) override
  {
    const Bencode_Lib::List &bNodeList = BRef<Bencode_Lib::List>(bNode);
    totalLists++;
    sizeInBytes += sizeof(Bencode_Lib::List);
    maxListSize = std::max(bNodeList.size(), static_cast<int>(maxListSize));
    for ([[maybe_unused]] auto &bNodeEntry : bNodeList.value()) { sizeInBytes += sizeof(Bencode_Lib::BNode); }
  }
  // Add object details to analysis
  virtual void onDictionary(const Bencode_Lib::BNode &bNode) override
  {
    const Bencode_Lib::Dictionary &bNodeDictionary = BRef<Bencode_Lib::Dictionary>(bNode);
    totalDictionarys++;
    sizeInBytes += sizeof(Bencode_Lib::Dictionary);
    maxDictionarySize = std::max(bNodeDictionary.value().size(), maxDictionarySize);
    for (auto &entry : bNodeDictionary.value()) {
      auto &key = entry.getKey();
      uniqueKeys.insert(key);
      maxKeySize = std::max(key.size(), maxKeySize);
      sizeInBytes += key.size();
      sizeInBytes += sizeof(Bencode_Lib::Dictionary::Entry);
      totalKeys++;
    }
  }
  // Non-const api not used
  virtual void onInteger([[maybe_unused]] Bencode_Lib::BNode &bNode) override {}
  virtual void onList([[maybe_unused]] Bencode_Lib::BNode &bNode) override {}
  virtual void onDictionary([[maybe_unused]] Bencode_Lib::BNode &bNode) override {}
  virtual void onBNode([[maybe_unused]] Bencode_Lib::BNode &bNode) override {}
  virtual void onString([[maybe_unused]] Bencode_Lib::BNode &bNode) override {}
  // Output analysis details
  std::string dump()
  {
    std::stringstream os;
    os << "\n------------------Bencode Tree Stats------------------\n";
    os << "Bencode Tree contains " << totalNodes << " nodes.\n";
    os << "Bencode Tree size " << sizeInBytes << " in bytes.\n";
    os << "------------------Bencode Bencode_Lib::Dictionary Stats------------------\n";
    os << "Bencode Tree contains " << totalDictionarys << " objectEntries.\n";
    os << "Bencode Tree max object size " << maxDictionarySize << ".\n";
    os << "Bencode Tree total " << totalKeys << " keys.\n";
    os << "Bencode Tree contains " << uniqueKeys.size() << " unique keys.\n";
    os << "Bencode Tree max key size " << maxKeySize << " in bytes.\n";
    os << "------------------Bencode Bencode_Lib::List Stats------------------\n";
    os << "Bencode Tree contains " << totalLists << " arrays.\n";
    os << "Bencode Tree max array size " << maxListSize << ".\n";
    os << "------------------Bencode Bencode_Lib::String Stats------------------\n";
    os << "Bencode Tree total " << totalStrings << " strings.\n";
    os << "Bencode Tree contains " << uniqueStrings.size() << " unique strings.\n";
    os << "Bencode Tree max string size " << maxStringSize << " in bytes.\n";
    os << "------------------Bencode Bencode_Lib::Integer Stats------------------\n";
    os << "Bencode Tree contains " << totalIntegers << " numbers.\n";
    os << "Bencode Tree contains " << totalInteger << " integers.\n";
    os << "Bencode Tree contains " << totalLong << " longs.\n";
    os << "Bencode Tree contains " << totalLongLong << " long longs.\n";
    os << "Bencode Tree contains " << totalFloat << " floats.\n";
    os << "Bencode Tree contains " << totalDouble << " doubles.\n";
    os << "Bencode Tree contains " << totalLongDouble << " long doubles.\n";
    os << "------------------Bencode Bencode_Lib::Boolean Stats------------------\n";
    os << "Bencode Tree contains " << totalBoolean << " booleans.\n";
    os << "------------------Bencode Bencode_Lib::Null Stats------------------\n";
    os << "Bencode Tree contains " << totalNull << " nulls.\n";
    os << "----------------------------------------------------";
    return (os.str());
  }
  static std::string dumpBNodeSizes()
  {
    std::stringstream os;
    os << "\n--------------------Bencode_Lib::BNode Sizes---------------------\n";
    os << "Bencode_Lib::BNode size " << sizeof(Bencode_Lib::BNode) << " in bytes.\n";
    os << "Bencode_Lib::Dictionary size " << sizeof(Bencode_Lib::Dictionary) << " in bytes.\n";
    os << "Bencode_Lib::Dictionary Entry size " << sizeof(Bencode_Lib::Dictionary::Entry) << " in bytes.\n";
    os << "Bencode_Lib::List size " << sizeof(Bencode_Lib::List) << " in bytes.\n";
    os << "Bencode_Lib::Integer size " << sizeof(Bencode_Lib::Integer) << " in bytes.\n";
    os << "Bencode_Lib::String size " << sizeof(Bencode_Lib::String) << " in bytes.\n";
    return (os.str());
  }
  static std::string dumpNumericSizes()
  {
    std::stringstream os;
    os << "\n--------------------System Numeric Sizes---------------------\n";
    os << "Short size " << sizeof(short) << " in bytes.\n";
    os << "Int size " << sizeof(int) << " in bytes.\n";
    os << "Long Long size " << sizeof(long long) << " in bytes.\n";
    os << "Float size " << sizeof(float) << " in bytes.\n";
    os << "Double size " << sizeof(double) << " in bytes.\n";
    os << "Long Double size " << sizeof(long double) << " in bytes.\n";
    return (os.str());
  }

private:
  // Bencode analysis data
  // Node
  int64_t totalNodes{};
  size_t sizeInBytes{};
  // Bencode_Lib::Dictionary
  int64_t totalDictionarys{};
  size_t maxDictionarySize{};
  int64_t totalKeys{};
  size_t maxKeySize{};
  std::set<std::string> uniqueKeys{};
  // Bencode_Lib::List
  size_t maxListSize{};
  int64_t totalLists{};
  // Bencode_Lib::String
  int64_t totalStrings{};
  std::set<std::string> uniqueStrings{};
  size_t maxStringSize{};
  // Bencode_Lib::Integer
  int64_t totalIntegers{};
  int64_t totalInteger{};
  int64_t totalLong{};
  int64_t totalLongLong{};
  int64_t totalFloat{};
  int64_t totalDouble{};
  int64_t totalLongDouble{};
  // Bencode_Lib::Boolean
  int64_t totalBoolean{};
  // Bencode_Lib::Null
  int64_t totalNull{};
};