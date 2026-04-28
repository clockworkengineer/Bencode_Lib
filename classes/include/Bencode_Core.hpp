// File: Bencode_Core.hpp
//
// Description: Core Bencode type aliases, configuration macros, and internal support utilities for the library.
//

#pragma once
// ============
// Bencode Core
// ============
#include "Bencode_Config.hpp"
#include "Bencode_Interfaces.hpp"
#include "Bencode_Variant.hpp"
#include "Bencode_Node.hpp"
#include "Bencode_Hole.hpp"
#include "Bencode_Integer.hpp"
#include "Bencode_String.hpp"
#include "Bencode_Dictionary.hpp"
#include "Bencode_List.hpp"
#include "Bencode_Node_Reference.hpp"
#include "Bencode_Node_Creation.hpp"
#include "Bencode_Node_Index.hpp"
#include "Bencode_Sources.hpp"
#include "Bencode_Destinations.hpp"
#include "Bencode_Error.hpp"
#include "Bencode_Status.hpp"
#include "Default_Translator.hpp"
#include "Default_Parser.hpp"
#include "Default_Stringify.hpp"
#if BENCODE_ENABLE_JSON_STRINGIFY
#include "JSON_Stringify.hpp"
#endif
#if BENCODE_ENABLE_XML_STRINGIFY
#include "XML_Stringify.hpp"
#endif
#if BENCODE_ENABLE_YAML_STRINGIFY
#include "YAML_Stringify.hpp"
#endif