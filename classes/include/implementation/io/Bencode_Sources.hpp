// File: Bencode_Sources.hpp
//
// Description: Aggregated header exporting concrete Bencode source implementations and helpers.
//

#pragma once

#include "Bencode_BufferSource.hpp"

#if BENCODE_ENABLE_FILE_IO
#include "Bencode_FileSource.hpp"
#endif