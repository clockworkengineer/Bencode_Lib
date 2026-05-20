// File: Bencode_Optional_Stringify.hpp
//
// Description: Optional stringify module headers for JSON, XML, and YAML.
//
// Include this header only when the corresponding optional stringify modules
// are enabled in CMake. If no optional stringifiers are enabled, this header
// compiles as a no-op.

#pragma once

#include "Bencode_Config.hpp"

#if BENCODE_ENABLE_JSON_STRINGIFY
#include "implementation/stringify/JSON_Stringify.hpp"
#endif

#if BENCODE_ENABLE_XML_STRINGIFY
#include "implementation/stringify/XML_Stringify.hpp"
#endif

#if BENCODE_ENABLE_YAML_STRINGIFY
#include "implementation/stringify/YAML_Stringify.hpp"
#endif
