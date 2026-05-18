// File: Bencode_Core.hpp
//
// Description: Core Bencode type aliases, configuration macros, and internal support utilities for the library.
//

#pragma once
// ============
// Bencode Core
// ============
#include "Bencode_Config.hpp"
#include "interface/Bencode_Interfaces.hpp"
#include "implementation/variants/Bencode_Variant.hpp"
#include "implementation/node/Bencode_Node.hpp"
#include "implementation/variants/Bencode_Hole.hpp"
#include "implementation/variants/Bencode_Integer.hpp"
#include "implementation/variants/Bencode_String.hpp"
#include "implementation/variants/Bencode_Dictionary.hpp"
#include "implementation/variants/Bencode_List.hpp"
#include "implementation/node/Bencode_Node_Reference.hpp"
#include "implementation/node/Bencode_Node_Creation.hpp"
#include "implementation/node/Bencode_Node_Index.hpp"
#include "implementation/io/Bencode_Sources.hpp"
#include "implementation/io/Bencode_Destinations.hpp"
#include "implementation/common/Bencode_Error.hpp"
#include "implementation/common/Bencode_Status.hpp"

// Default parser/stringify and translator implementations are internal.
// Public users should include only the core API and optional modules explicitly.
