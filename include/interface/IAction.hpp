#pragma once

#include <stdexcept>
#include <string>

namespace Bencode_Lib {

// ====================
// Forward declarations
// ====================
struct BNode;

// =============================================================
// Interface for the action events during Bencode tree traversal
// =============================================================
class IAction
{
public:
  // =============
  // IAction Error
  // =============
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("IAction Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IAction() = default;
  // ============================
  // BNode encountered so process
  // ============================
  virtual void onBNode([[maybe_unused]] BNode &bNode) = 0;
  virtual void onBNode([[maybe_unused]] const BNode &bNode) = 0;
  // =============================
  // String encountered so process
  // =============================
  virtual void onString([[maybe_unused]] BNode &bNode) = 0;
  virtual void onString([[maybe_unused]] const BNode &bNode) = 0;
  // =============================
  // Integer encountered so process
  // =============================
  virtual void onInteger([[maybe_unused]] BNode &bNode) = 0;
  virtual void onInteger([[maybe_unused]] const BNode &bNode) = 0;
  // ============================
  // Array encountered so process
  // ============================
  virtual void onList([[maybe_unused]] BNode &bNode) = 0;
  virtual void onList([[maybe_unused]] const BNode &bNode) = 0;
  // =============================
  // Object encountered so process
  // =============================
  virtual void onDictionary([[maybe_unused]] BNode &bNode) = 0;
  virtual void onDictionary([[maybe_unused]] const BNode &bNode) = 0;
};
}// namespace Bencode_Lib