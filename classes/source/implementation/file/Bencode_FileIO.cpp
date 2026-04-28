
//
// Class: Bencode_Impl_FileIO
//
// Description: Public wrapper implementation for Bencode file-based I/O.
//              Platform-specific file open behavior is selected by CMake.
//
// Dependencies: C++20 - Language standard features used.
//

#include "Bencode_Impl.hpp"
#include "Bencode_Error.hpp"
#include "Bencode_FileIO_Internal.hpp"

#include <cstdio>
#include <string>
#include <vector>

namespace Bencode_Lib {

Bencode_FileHandle::Bencode_FileHandle(const std::string_view &path, Mode mode)
    : file_(nullptr) {
  std::string pathString(path);
  if (mode == Mode::Read) {
    openBencodeFileForRead(pathString, file_);
  } else {
    openBencodeFileForWrite(pathString, file_);
  }
}

Bencode_FileHandle::~Bencode_FileHandle() {
  if (file_) {
    std::fclose(file_);
  }
}

Bencode_FileHandle::Bencode_FileHandle(Bencode_FileHandle &&other) noexcept
    : file_(other.file_) {
  other.file_ = nullptr;
}

Bencode_FileHandle &Bencode_FileHandle::operator=(
    Bencode_FileHandle &&other) noexcept {
  if (this != &other) {
    if (file_) {
      std::fclose(file_);
    }
    file_ = other.file_;
    other.file_ = nullptr;
  }
  return *this;
}

bool Bencode_FileHandle::isOpen() const noexcept { return file_ != nullptr; }
FILE *Bencode_FileHandle::get() const noexcept { return file_; }
Bencode_FileHandle::operator FILE *() const noexcept { return file_; }

static void ensureFileOpen(FILE *bencodeFile, const char *message) {
  if (!bencodeFile) {
    throw Error(message);
  }
}

static Bencode_FileHandle openFileOrThrow(const std::string_view &fileName,
                                          Bencode_FileHandle::Mode mode,
                                          const char *errorMessage) {
  Bencode_FileHandle file(fileName, mode);
  if (!file.isOpen()) {
    throw Error(errorMessage);
  }
  return file;
}

void writeBencodeString(FILE *bencodeFile,
                        const std::string_view bencodeString) {
  ensureFileOpen(bencodeFile, "File stream is not open for writing.");
  std::size_t written =
      std::fwrite(bencodeString.data(), 1, bencodeString.size(), bencodeFile);
  if (written != bencodeString.size()) {
    throw Error("Failed to write Bencode string to file.");
  }
}

std::string readBencodeString(FILE *bencodeFile) {
  ensureFileOpen(bencodeFile, "File stream is not open for reading.");

  std::vector<char> buffer;
  char chunk[4096];
  while (true) {
    std::size_t count = std::fread(chunk, 1, sizeof(chunk), bencodeFile);
    if (count > 0) {
      buffer.insert(buffer.end(), chunk, chunk + count);
    }
    if (count < sizeof(chunk)) {
      if (std::ferror(bencodeFile)) {
        throw Error("Failed to read Bencode file.");
      }
      break;
    }
  }
  return std::string(buffer.begin(), buffer.end());
}

std::string Bencode_Impl::fromFile(const std::string_view &fileName) {
  Bencode_FileHandle file = openFileOrThrow(
      fileName, Bencode_FileHandle::Mode::Read,
      "Bencode file input stream failed to open or does not exist.");

  return readBencodeString(file.get());
}

void Bencode_Impl::toFile(const std::string_view &fileName,
                          const std::string_view &bencodeString) {
  Bencode_FileHandle file = openFileOrThrow(
      fileName, Bencode_FileHandle::Mode::Write,
      "Bencode file output stream failed to open or could not be created.");

  writeBencodeString(file.get(), bencodeString);
}

} // namespace Bencode_Lib
