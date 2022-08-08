//
// Class: BNode
//
// Description: BNode struct is used to hold the parsed Bencoding and model its
// structure by arranging the BNodes in a tree; this can then be traversed to
// reform the Bencoding during encode.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "Bencode.hpp"
#include "Bencode_Types.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
// =========
// NAMESPACE
// =========
namespace BencodeLib {
// ===========================
// PRIVATE TYPES AND CONSTANTS
// ===========================
// ==========================
// PUBLIC TYPES AND CONSTANTS
// ==========================
// ========================
// PRIVATE STATIC VARIABLES
// ========================
// =======================
// PUBLIC STATIC VARIABLES
// =======================
// ===============
// PRIVATE METHODS
// ===============
// ==============
// PUBLIC METHODS
// ==============
// =====================
// BNode index overloads
// =====================
// ====
// List
// ====
BNode &BNode::operator[](const std::string &key) {
  return (BRef<Dictionary>(*this)[key]);
}
BNode &BNode::operator[](int index) { return (BRef<List>(*this)[index]); }
// ==========
// Dictionary
// ==========
const BNode &BNode::operator[](const std::string &key) const // Dictionary
{
  return (BRef<const Dictionary>(*this)[key]);
}
const BNode &BNode::operator[](int index) const // List
{
  return (BRef<const List>(*this)[index]);
}
// ==============
// Get BNode type
// ==============
BNode::Type BNode::getNodeType() const {
  return (m_bNodeVariant->getNodeType());
}
// ==============================
// Get reference to BNode variant
// ==============================
std::unique_ptr<Variant> &BNode::getVariant() { return (m_bNodeVariant); }
const std::unique_ptr<Variant> &BNode::getVariant() const {
  return (m_bNodeVariant);
};
} // namespace BencodeLib