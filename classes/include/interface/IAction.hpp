#pragma once

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
  struct Error final : std::runtime_error {
    explicit Error(const std::string_view message)
        : std::runtime_error(std::string("IAction Error: ").append(message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IAction() = default;
  // ============================
  // BNode encountered so process
  // ============================
  virtual void onBNode([[maybe_unused]] BNode &bNode) { }
  virtual void onBNode([[maybe_unused]] const BNode &bNode) { }
  // =============================
  // String encountered so process
  // =============================
  virtual void onString([[maybe_unused]] BNode &bNode) { }
  virtual void onString([[maybe_unused]] const BNode &bNode) { }
  // =============================
  // Integer encountered so process
  // =============================
  virtual void onInteger([[maybe_unused]] BNode &bNode) { }
  virtual void onInteger([[maybe_unused]] const BNode &bNode) { }
  // ============================
  // Array encountered so process
  // ============================
  virtual void onList([[maybe_unused]] BNode &bNode) { }
  virtual void onList([[maybe_unused]] const BNode &bNode) { }
  // =============================
  // Object encountered so process
  // =============================
  virtual void onDictionary([[maybe_unused]] BNode &bNode) { }
  virtual void onDictionary([[maybe_unused]] const BNode &bNode) { }
};
}// namespace Bencode_Lib