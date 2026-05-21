// File: Bencode_Core.hpp
//
// Description: Core Bencode type aliases, configuration macros, and internal support utilities for the library.
//

#pragma once
// ============
// Bencode Core
// ============
#include "Bencode_Config.hpp"

static_assert(BENCODE_MAX_NODE_COUNT > 0,
              "BENCODE_MAX_NODE_COUNT must be positive.");
static_assert(BENCODE_MAX_CONTAINER_SIZE > 0,
              "BENCODE_MAX_CONTAINER_SIZE must be positive.");
static_assert(BENCODE_MAX_STRING_LENGTH > 0,
              "BENCODE_MAX_STRING_LENGTH must be positive.");

#if defined(BENCODE_EMBEDDED_MODE) && (BENCODE_EMBEDDED_MODE == 1)
static_assert(BENCODE_ENABLE_FILE_IO == 0,
              "Embedded mode requires BENCODE_ENABLE_FILE_IO=0.");
static_assert(BENCODE_ENABLE_DYNAMIC_ALLOCATION == 0,
              "Embedded mode requires BENCODE_ENABLE_DYNAMIC_ALLOCATION=0.");
static_assert(BENCODE_ENABLE_EXCEPTIONS == 0,
              "Embedded mode requires BENCODE_ENABLE_EXCEPTIONS=0.");
#endif

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

// Default parser/stringify and translator implementations are internal.
// Public users should include only the core API and optional modules explicitly.
