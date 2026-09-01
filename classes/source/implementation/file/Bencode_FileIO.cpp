#include "Bencode_Impl.hpp"
#include "Bencode_Error.hpp"
#include "Bencode_FileIO_Internal.hpp"
#include "implementation/io/Bencode_FileService.hpp"

#include <cstdio>
#include <string>
#include <vector>

namespace Bencode_Lib {

/// <summary>
/// Open a platform-specific file handle for Bencode I/O.
/// </summary>
Bencode_FileHandle::Bencode_FileHandle(const std::string_view &path, Mode mode) {
  std::string pathString(path);
  FILE *rawFile = nullptr;
  if (mode == Mode::Read) {
    openBencodeFileForRead(pathString, rawFile);
  } else {
    openBencodeFileForWrite(pathString, rawFile);
  }
  file_.reset(rawFile);
}

Bencode_FileHandle::~Bencode_FileHandle() = default;

/// <summary>
/// Move-construct a file handle.
/// </summary>
Bencode_FileHandle::Bencode_FileHandle(Bencode_FileHandle &&other) noexcept
    : file_(std::move(other.file_)) {}

/// <summary>
/// Move-assign a file handle.
/// </summary>
Bencode_FileHandle &Bencode_FileHandle::operator=(
    Bencode_FileHandle &&other) noexcept {
  if (this != &other) {
    file_ = std::move(other.file_);
  }
  return *this;
}

/// <summary>
/// Query whether the file handle is currently open.
/// </summary>
bool Bencode_FileHandle::isOpen() const noexcept { return file_ != nullptr; }

/// <summary>
/// Get the underlying FILE pointer for the open handle.
/// </summary>
FILE *Bencode_FileHandle::get() const noexcept { return file_.get(); }

/// <summary>
/// Convert the file handle to a raw FILE pointer.
/// </summary>
Bencode_FileHandle::operator FILE *() const noexcept { return file_.get(); }

/// <summary>
/// Verify that the underlying FILE pointer is open.
/// </summary>
static void ensureFileOpen(FILE *bencodeFile, const char *message) {
  if (!bencodeFile) {
    throw Error(message);
  }
}

/// <summary>
/// Open a file or throw a Bencode error if the file cannot be opened.
/// </summary>
static Bencode_FileHandle openFileOrThrow(const std::string_view &fileName,
                                          Bencode_FileHandle::Mode mode,
                                          const char *errorMessage) {
  Bencode_FileHandle file(fileName, mode);
  if (!file.isOpen()) {
    throw Error(errorMessage);
  }
  return file;
}

/// <summary>
/// Write a Bencode string into the open file stream.
/// </summary>
void writeBencodeString(FILE *bencodeFile,
                        const std::string_view bencodeString) {
  ensureFileOpen(bencodeFile, "File stream is not open for writing.");
  std::size_t written =
      std::fwrite(bencodeString.data(), 1, bencodeString.size(), bencodeFile);
  if (written != bencodeString.size()) {
    throw Error("Failed to write Bencode string to file.");
  }
}

/// <summary>
/// Read the entire contents of the opened Bencode file into a string.
/// </summary>
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

/// <summary>
/// Read a Bencode file and return its contents as a string via Bencode_FileService.
/// </summary>
std::string Bencode_FileService::readFromFile(const std::string_view &fileName) {
  Bencode_FileHandle file = openFileOrThrow(
      fileName, Bencode_FileHandle::Mode::Read,
      "Bencode file input stream failed to open or does not exist.");

  return readBencodeString(file.get());
}

/// <summary>
/// Write a Bencode string to a file via Bencode_FileService.
/// </summary>
void Bencode_FileService::writeToFile(const std::string_view &fileName,
                                      const std::string_view &bencodeString) {
  Bencode_FileHandle file = openFileOrThrow(
      fileName, Bencode_FileHandle::Mode::Write,
      "Bencode file output stream failed to open or could not be created.");

  writeBencodeString(file.get(), bencodeString);
}

/// <summary>
/// Read a Bencode file and return its contents as a string (forwarded to Bencode_FileService).
/// </summary>
std::string Bencode_Impl::fromFile(const std::string_view &fileName) {
  return Bencode_FileService::readFromFile(fileName);
}

/// <summary>
/// Write a Bencode string to a file (forwarded to Bencode_FileService).
/// </summary>
void Bencode_Impl::toFile(const std::string_view &fileName,
                          const std::string_view &bencodeString) {
  Bencode_FileService::writeToFile(fileName, bencodeString);
}

} // namespace Bencode_Lib
