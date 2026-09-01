// File: Bencode_FileService.hpp
//
// Description: Dedicated file I/O service class for reading and writing Bencode strings to disk.
//

#pragma once

#include <string>
#include <string_view>

namespace Bencode_Lib {

class Bencode_FileService {
public:
  static std::string readFromFile(const std::string_view &fileName);
  static void writeToFile(const std::string_view &fileName,
                         const std::string_view &bencodeString);
};

} // namespace Bencode_Lib
