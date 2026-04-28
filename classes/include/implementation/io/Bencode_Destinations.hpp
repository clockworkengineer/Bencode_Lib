// File: Bencode_Destinations.hpp
//
// Description: Aggregated header exporting concrete Bencode destination implementations and helpers.
//

#pragma once

#include "Bencode_BufferDestination.hpp"

#if BENCODE_ENABLE_FILE_IO
#include "Bencode_FileDestination.hpp"
#endif