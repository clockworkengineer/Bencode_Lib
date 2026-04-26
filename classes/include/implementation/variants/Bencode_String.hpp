#pragma once

#include <array>
#include <cstring>
#include <new>
#include <string>
#include <string_view>

namespace Bencode_Lib {

struct String : Variant {
#if defined(BENCODE_MAX_STRING_LENGTH)
  constexpr static Bencode::IntegerType kMaxLength = BENCODE_MAX_STRING_LENGTH;
#else
  constexpr static Bencode::IntegerType kMaxLength = 16 * 1024;
#endif
  constexpr static std::size_t kInlineCapacity = 22;

  // Constructors/Destructors
  String() : Variant(Type::string), mSize(0), mHeapActive(false) {
    inlineData()[0] = '\0';
  }

  explicit String(const std::string_view string)
      : Variant(Type::string), mSize(string.size()),
        mHeapActive(string.size() > kInlineCapacity) {
    if (mHeapActive) {
      new (&mStorage.heapString) std::string(string);
    } else {
      std::memcpy(inlineData(), string.data(), mSize);
      inlineData()[mSize] = '\0';
    }
  }

  explicit String(const char *string)
      : Variant(Type::string), mSize(std::strlen(string)),
        mHeapActive(mSize > kInlineCapacity) {
    if (mHeapActive) {
      new (&mStorage.heapString) std::string(string);
    } else {
      std::memcpy(inlineData(), string, mSize);
      inlineData()[mSize] = '\0';
    }
  }

  explicit String(std::string &&string)
      : Variant(Type::string), mSize(string.size()),
        mHeapActive(string.size() > kInlineCapacity) {
    if (mHeapActive) {
      new (&mStorage.heapString) std::string(std::move(string));
    } else {
      std::memcpy(inlineData(), string.data(), mSize);
      inlineData()[mSize] = '\0';
    }
  }

  explicit String(std::size_t length)
      : Variant(Type::string), mSize(length),
        mHeapActive(length > kInlineCapacity) {
    if (mHeapActive) {
      new (&mStorage.heapString) std::string(length, '\0');
    } else {
      inlineData()[length] = '\0';
    }
  }

  String(const String &other)
      : Variant(Type::string), mSize(other.mSize),
        mHeapActive(other.mHeapActive) {
    if (mHeapActive) {
      new (&mStorage.heapString) std::string(other.heapData());
    } else {
      std::memcpy(inlineData(), other.inlineData(), mSize + 1);
    }
  }

  String(String &&other) noexcept
      : Variant(Type::string), mSize(other.mSize),
        mHeapActive(other.mHeapActive) {
    if (mHeapActive) {
      new (&mStorage.heapString) std::string(std::move(other.heapData()));
      other.heapData().~basic_string();
    } else {
      std::memcpy(inlineData(), other.inlineData(), mSize + 1);
      other.inlineData()[0] = '\0';
    }
    other.mSize = 0;
    other.mHeapActive = false;
  }

  String &operator=(const String &other) {
    if (this != &other) {
      destroy();
      mSize = other.mSize;
      mHeapActive = other.mHeapActive;
      if (mHeapActive) {
        new (&mStorage.heapString) std::string(other.heapData());
      } else {
        std::memcpy(inlineData(), other.inlineData(), mSize + 1);
      }
    }
    return *this;
  }

  String &operator=(String &&other) noexcept {
    if (this != &other) {
      destroy();
      mSize = other.mSize;
      mHeapActive = other.mHeapActive;
      if (mHeapActive) {
        new (&mStorage.heapString) std::string(std::move(other.heapData()));
        other.heapData().~basic_string();
      } else {
        std::memcpy(inlineData(), other.inlineData(), mSize + 1);
        other.inlineData()[0] = '\0';
      }
      other.mSize = 0;
      other.mHeapActive = false;
    }
    return *this;
  }

  ~String() { destroy(); }

  // Get Node value
  [[nodiscard]] std::string_view value() const noexcept {
    return std::string_view(data(), mSize);
  }

  [[nodiscard]] char *data() noexcept {
    return mHeapActive ? heapData().data() : inlineData();
  }

  [[nodiscard]] const char *data() const noexcept {
    return mHeapActive ? heapData().data() : inlineData();
  }

  void resize(std::size_t newSize) {
    if (newSize > kInlineCapacity) {
      if (!mHeapActive) {
        std::string copy(inlineData(), mSize);
        new (&mStorage.heapString) std::string(std::move(copy));
        mHeapActive = true;
      }
      heapData().resize(newSize);
    } else if (mHeapActive) {
      std::string copy(heapData().data(), newSize);
      heapData().~basic_string();
      mHeapActive = false;
      std::memcpy(inlineData(), copy.data(), newSize);
      inlineData()[newSize] = '\0';
    } else {
      inlineData()[newSize] = '\0';
    }
    mSize = newSize;
  }

  void assign(const std::string_view string) {
    if (string.size() > kInlineCapacity) {
      if (mHeapActive) {
        heapData().assign(string);
      } else {
        new (&mStorage.heapString) std::string(string);
        mHeapActive = true;
      }
    } else {
      if (mHeapActive) {
        heapData().~basic_string();
        mHeapActive = false;
      }
      std::memcpy(inlineData(), string.data(), string.size());
      inlineData()[string.size()] = '\0';
    }
    mSize = string.size();
  }

  // Set/get maximum string length
  static void setMaxStringLength(const uint64_t length) {
    maxStringLength = length;
  }
  static uint64_t getMaxStringLength() { return maxStringLength; }

private:
  void destroy() noexcept {
    if (mHeapActive) {
      heapData().~basic_string();
    }
  }

  [[nodiscard]] char *inlineData() noexcept {
    return mStorage.inlineStorage.data();
  }

  [[nodiscard]] const char *inlineData() const noexcept {
    return mStorage.inlineStorage.data();
  }

  [[nodiscard]] std::string &heapData() noexcept { return mStorage.heapString; }

  [[nodiscard]] const std::string &heapData() const noexcept {
    return mStorage.heapString;
  }

  union Storage {
    std::array<char, kInlineCapacity + 1> inlineStorage;
    std::string heapString;
    Storage() noexcept {}
    ~Storage() noexcept {}
  };

  Storage mStorage;
  std::size_t mSize;
  bool mHeapActive;
  inline static uint64_t maxStringLength{kMaxLength};
};
} // namespace Bencode_Lib