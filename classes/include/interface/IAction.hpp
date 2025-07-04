#pragma once

namespace Bencode_Lib {

// ====================
// Forward declarations
// ====================
struct Node;

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
    explicit Error(const std::string_view & message)
        : std::runtime_error(std::string("IAction Error: ").append(message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IAction() = default;
  // ============================
  // Node encountered so process
  // ============================
  virtual void onNode([[maybe_unused]] Node &bNode) { }
  virtual void onNode([[maybe_unused]] const Node &bNode) { }
  // =============================
  // String encountered so process
  // =============================
  virtual void onString([[maybe_unused]] Node &bNode) { }
  virtual void onString([[maybe_unused]] const Node &bNode) { }
  // =============================
  // Integer encountered so process
  // =============================
  virtual void onInteger([[maybe_unused]] Node &bNode) { }
  virtual void onInteger([[maybe_unused]] const Node &bNode) { }
  // ============================
  // Array encountered so process
  // ============================
  virtual void onList([[maybe_unused]] Node &bNode) { }
  virtual void onList([[maybe_unused]] const Node &bNode) { }
  // =============================
  // Object encountered so process
  // =============================
  virtual void onDictionary([[maybe_unused]] Node &bNode) { }
  virtual void onDictionary([[maybe_unused]] const Node &bNode) { }
};
}// namespace Bencode_Lib