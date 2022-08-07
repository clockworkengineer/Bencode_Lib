#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <stdexcept>
#include <limits>
#include <array>
#include <sstream>
// =============================
// Source/Destination interfaces
// =============================
#include "ISource.hpp"
#include "IDestination.hpp"
// =======
// Bencode
// =======
#include "Bencode_Config.hpp"
#include "Bencode_Types.hpp"
// =========
// NAMESPACE
// =========
namespace BencodeLib {
// ================
// CLASS DEFINITION
// ================
class Bencode_Impl {
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  Bencode_Impl() = default;
  Bencode_Impl(const Bencode_Impl &other) = delete;
  Bencode_Impl &operator=(const Bencode_Impl &other) = delete;
  Bencode_Impl(Bencode_Impl &&other) = delete;
  Bencode_Impl &operator=(Bencode_Impl &&other) = delete;
  ~Bencode_Impl() = default;
  // ==============
  // PUBLIC METHODS
  // ==============
  void decode(ISource &source);
  void encode(IDestination &destination)const ;
  std::string version() const ;
  BNode &root() { return (m_bNodeRoot); }
  [[nodiscard]] const BNode &root() const { return (m_bNodeRoot); }
  // ================
  // PUBLIC VARIABLES
  // ================
private:
  // ===========================
  // PRIVATE TYPES AND CONSTANTS
  // ===========================
  // ===============
  // PRIVATE METHODS
  // ===============
  static int64_t extractInteger(ISource &source);
  static std::string extractString(ISource &source);
  static BNode decodeString(ISource &source);
  static BNode decodeInteger(ISource &source);
  static BNode decodeDictionary(ISource &source);
  static BNode decodeList(ISource &source);
  static BNode decodeBNodes(ISource &source);
  static void encodeBNodes(const BNode &bNode, IDestination &destination);
  // =================
  // PRIVATE VARIABLES
  // =================
  // Root of BNoding Tree
  BNode m_bNodeRoot;
};
} // namespace BencodeLib
