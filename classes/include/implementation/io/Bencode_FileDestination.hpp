#pragma once

namespace Bencode_Lib {

class FileDestination final : public IDestination {

public:
  // Constructors/Destructors
  explicit FileDestination(const std::string &filename) : filename(filename) {
    destination.open(filename.c_str(), std::ios_base::binary);
    if (!destination.is_open()) {
      throw Error("Bencode file output stream failed to open or could not be "
                  "created.");
    }
  }
  FileDestination() = delete;
  FileDestination(const FileDestination &other) = delete;
  FileDestination &operator=(const FileDestination &other) = delete;
  FileDestination(FileDestination &&other) = delete;
  FileDestination &operator=(FileDestination &&other) = delete;
  ~FileDestination() override = default;

  void add(const std::string &bytes) override {
    destination.write(bytes.c_str(), bytes.length());
    destination.flush();
    length += bytes.length();
        lastChar = bytes.back();
  }
  void add( std::string_view bytes) override {
    destination.write(bytes.data(), bytes.length());
    destination.flush();
    length += bytes.length();
    lastChar = bytes.back();
  }
  void add(const char *bytes) override {
    int length = strlen(bytes);
    destination.write(bytes, length);
    destination.flush();
    length += length;
    lastChar = bytes[length - 1];
  }
  void add(const char ch) override {
    destination.put(ch);
    destination.flush();
    length++;
        lastChar = ch;
  }

  void clear() override {
    if (destination.is_open()) {
      destination.close();
    }
    destination.open(filename.c_str(),
                     std::ios_base::binary | std::ios_base::trunc);
    if (!destination.is_open()) {
      throw Error("File output stream failed to open or could not be created.");
    }
    length = 0;
        lastChar = 0;
  }

  std::size_t size() const { return length; }
    [[nodiscard]] char last() override { return lastChar; }

private:
  std::ofstream destination;
  std::string filename;
  std::size_t length{};
  char lastChar{};
};

} // namespace Bencode_Lib
