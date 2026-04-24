#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace Bencode_Lib {

template <typename T, std::size_t Capacity> class FixedVector {
public:
  using value_type = T;
  using size_type = std::size_t;
  using iterator = T *;
  using const_iterator = const T *;
  using reference = T &;
  using const_reference = const T &;

  static_assert(Capacity > 0, "FixedVector capacity must be positive.");

  FixedVector() = default;
  FixedVector(const FixedVector &other) = default;
  FixedVector &operator=(const FixedVector &other) = default;
  FixedVector(FixedVector &&other) = default;
  FixedVector &operator=(FixedVector &&other) = default;
  ~FixedVector() = default;

  iterator begin() noexcept { return data_.data(); }
  const_iterator begin() const noexcept { return data_.data(); }
  iterator end() noexcept { return data_.data() + size_; }
  const_iterator end() const noexcept { return data_.data() + size_; }

  [[nodiscard]] size_type size() const noexcept { return size_; }
  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
  [[nodiscard]] size_type capacity() const noexcept { return Capacity; }

  void clear() noexcept { size_ = 0; }

  reference operator[](size_type index) { return data_[index]; }
  const_reference operator[](size_type index) const { return data_[index]; }

  reference back() { return data_[size_ - 1]; }
  const_reference back() const { return data_[size_ - 1]; }

  void push_back(const T &value) { insert(end(), value); }
  void push_back(T &&value) { insert(end(), std::move(value)); }

  template <typename... Args> void emplace_back(Args &&...args) {
    if (size_ >= Capacity) {
      throw std::runtime_error("FixedVector capacity exceeded.");
    }
    data_[size_] = T(std::forward<Args>(args)...);
    ++size_;
  }

  iterator insert(const_iterator position, const T &value) {
    return insertImpl(position, value);
  }

  iterator insert(const_iterator position, T &&value) {
    return insertImpl(position, std::move(value));
  }

  void resize(size_type newSize) {
    if (newSize > Capacity) {
      throw std::runtime_error("FixedVector resize capacity exceeded.");
    }
    if (newSize > size_) {
      for (size_type index = size_; index < newSize; ++index) {
        data_[index] = T();
      }
    }
    size_ = newSize;
  }

private:
  template <typename U>
  iterator insertImpl(const_iterator position, U &&value) {
    const auto index = static_cast<size_type>(position - begin());
    if (index > size_) {
      throw std::out_of_range("FixedVector insert position out of range.");
    }
    if (size_ >= Capacity) {
      throw std::runtime_error("FixedVector capacity exceeded.");
    }
    for (size_type current = size_; current > index; --current) {
      data_[current] = std::move(data_[current - 1]);
    }
    data_[index] = std::forward<U>(value);
    ++size_;
    return begin() + index;
  }

  std::array<T, Capacity> data_{};
  size_type size_{};
};

} // namespace Bencode_Lib
