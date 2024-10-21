#pragma once

#include <initializer_list>
#include <mutex>
#include <stdexcept>
#include <utility>

template <typename T> class ArrayList {
public:
  class Iterator {
  public:
    Iterator(T *ptr) : ptr_(ptr) {}
    Iterator operator++() {
      ++ptr_;
      return *this;
    } // NOTE: Prefix increment
    bool operator!=(const Iterator &other) const {
      return ptr_ != other.ptr_;
    } // NOTE: Inequality
    T &operator*() const { return *ptr_; } // NOTE: Dereference

  private:
    T *ptr_;
  };

  ArrayList();
  explicit ArrayList(size_t capacity);
  ArrayList(std::initializer_list<T> list);
  ~ArrayList();

  // NOTE: Deep copy - copy and assignment init
  ArrayList(const ArrayList &other);
  ArrayList &operator=(const ArrayList &other);

  // NOTE: Move constructor and assignment (transfew ownership)
  ArrayList(ArrayList &&other) noexcept;
  ArrayList &operator=(ArrayList &&other) noexcept;

  T &operator[](size_t index); // NOTE: Access without bounds checking
  const T &operator[](size_t index) const;
  T &at(size_t index); // NOTE: Access with bounds checking
  const T &at(size_t index) const;

  void push_back(const T &value);
  void push_back(T &&value);
  void pop_back();
  size_t size() const;
  size_t capacity() const;
  bool empty() const;
  void clear();
  Iterator begin() const { return Iterator(data_); }
  Iterator end() const { return Iterator(data_ + size_); }

private:
  T *data_;
  size_t size_;
  size_t capacity_;
  std::mutex mtx_;

  void resize_if_needed();
  void copy_from(const ArrayList &other);
  void move_from(ArrayList &&other);
};

// NOTE: Implementation

template <typename T>
ArrayList<T>::ArrayList() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
ArrayList<T>::ArrayList(size_t capacity) : size_(0), capacity_(capacity) {
  data_ = new T[capacity];
}

template <typename T>
ArrayList<T>::ArrayList(std::initializer_list<T> list)
    : ArrayList(list.size()) {
  size_ = 0;
  for (const T &element : list) {
    data_[size_++] = element;
  }
}

template <typename T> ArrayList<T>::~ArrayList() { delete[] data_; }

template <typename T> ArrayList<T>::ArrayList(const ArrayList &other) {
  copy_from(other);
}

template <typename T>
ArrayList<T> &ArrayList<T>::operator=(const ArrayList &other) {
  if (this != &other) {
    delete[] data_;
    copy_from(other);
  }
  return *this;
}

template <typename T> ArrayList<T>::ArrayList(ArrayList &&other) noexcept {
  move_from(std::move(other));
}

template <typename T>
ArrayList<T> &ArrayList<T>::operator=(ArrayList &&other) noexcept {
  if (this != &other) {
    delete[] data_;
    move_from(std::move(other));
  }
  return *this;
}

template <typename T> T &ArrayList<T>::operator[](size_t index) {
  return data_[index];
}

template <typename T> const T &ArrayList<T>::operator[](size_t index) const {
  return data_[index];
}
// Access with bounds checking
template <typename T> T &ArrayList<T>::at(size_t index) {
  if (index >= size_) {
    throw std::out_of_range("Index out of range");
  }
  return data_[index];
}

template <typename T> const T &ArrayList<T>::at(size_t index) const {
  if (index >= size_) {
    throw std::out_of_range("Index out of range");
  }
  return data_[index];
}

template <typename T> void ArrayList<T>::push_back(const T &value) {
  std::lock_guard<std::mutex> lock(mtx_);
  resize_if_needed();
  data_[size_++] = value;
}

template <typename T> void ArrayList<T>::push_back(T &&value) {
  std::lock_guard<std::mutex> lock(mtx_);
  resize_if_needed();
  data_[size_++] = std::move(value);
}

template <typename T> void ArrayList<T>::pop_back() {
  std::lock_guard<std::mutex> lock(mtx_);
  if (size_ > 0) {
    --size_;
  }
}

template <typename T> size_t ArrayList<T>::size() const { return size_; }

template <typename T> size_t ArrayList<T>::capacity() const {
  return capacity_;
}

template <typename T> bool ArrayList<T>::empty() const { return size_ == 0; }

template <typename T> void ArrayList<T>::clear() {
  std::lock_guard<std::mutex> lock(mtx_);
  size_ = 0;
}

template <typename T> void ArrayList<T>::resize_if_needed() {
  if (size_ == capacity_) {
    size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
    T *new_data = new T[new_capacity];

    for (size_t i = 0; i < size_; ++i) {
      new_data[i] = std::move(data_[i]);
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }
}

template <typename T> void ArrayList<T>::copy_from(const ArrayList &other) {
  data_ = new T[other.capacity_];
  size_ = other.size_;
  capacity_ = other.capacity_;
  for (size_t i = 0; i < size_; ++i) {
    data_[i] = other.data_[i];
  }
}

template <typename T> void ArrayList<T>::move_from(ArrayList &&other) {
  data_ = other.data_;
  size_ = other.size_;
  capacity_ = other.capacity_;

  other.data_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}
