#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <stdexcept>
#include <memory>
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// ====================
// FORWARD DECLARATIONS
// ====================
#include "Bencode_Forward.hpp"
// ================
// CLASS DEFINITION
// ================
class Bencode {
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  Bencode();
  Bencode(const Bencode &other) = delete;
  Bencode &operator=(const Bencode &other) = delete;
  Bencode(Bencode &&other) = delete;
  Bencode &operator=(Bencode &&other) = delete;
  ~Bencode();
  // ==============
  // PUBLIC METHODS
  // ==============
  void decode(ISource &source) const;
  void decode(ISource &&source) const;
  void encode(IDestination &destination) const;
  void encode(IDestination &&destination) const;
  [[nodiscard]] std::string version() const;
  [[nodiscard]] BNode &root();
  [[nodiscard]] const BNode &root() const;
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
  // =================
  // PRIVATE VARIABLES
  // =================
  // Bencode implementation
  const std::unique_ptr<Bencode_Impl> m_implementation;
};
} // namespace Bencode_Lib
