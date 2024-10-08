#pragma once
#include <initializer_list>
#include <stdexcept>
#include <utility>

template <typename T> class ArrayList {
private:
  T *data;
  int current_size;
  int capacity;

  // NOTE: Resize inner fixed array to new capacity and moving elements
  void resize(int new_capacity) {
    T *new_data = new T[new_capacity];

    for (int i = 0; i < current_size; ++i) {
      new_data[i] = std::move(data[i]); // WARNING: Have to use std::move as not
                                        // all types are copyable
    }

    delete[] data;
    data = new_data;
    capacity = new_capacity;
  }

public:
  ArrayList() : data(nullptr), current_size(0), capacity(0) {}
  ~ArrayList() { delete[] data; }

  // NOTE: Copy constructor (lvalue reference)
  ArrayList(const ArrayList &other)
      : data(new T[other.capacity]), current_size(other.current_size),
        capacity(other.capacity) {
    for (int i = 0; i < current_size; ++i) {
      data[i] = other.data[i];
    }
  }

  // NOTE: Copy assignment (lvalue reference)
  ArrayList &operator=(const ArrayList &other) {
    if (this != &other) {
      delete[] data;
      current_size = other.current_size;
      capacity = other.capacity;
      data = new T[capacity];
      for (int i = 0; i < current_size; ++i) {
        data[i] = other.data[i];
      }
    }
    return *this;
  }

  // NOTE: Move constructor (rvalue reference)
  ArrayList(ArrayList &&other) noexcept
      : data(other.data), current_size(other.current_size),
        capacity(other.capacity) {
    other.data = nullptr;
    other.current_size = 0;
    other.capacity = 0;
  }

  // NOTE: Move assignment (rvalue reference)
  ArrayList &operator=(ArrayList &&other) noexcept {
    if (this != &other) {
      delete[] data;
      data = other.data;
      current_size = other.current_size;
      capacity = other.capacity;
      other.data = nullptr;
      other.current_size = 0;
      other.capacity = 0;
    }
    return *this;
  }

  // NOTE: Constructor with initializer list (e.g., ArrayList<int> list = {1, 2,
  // 3})
  ArrayList(std::initializer_list<T> init_list) : ArrayList() {
    for (const T &item : init_list) {
      add(item);
    }
  }

  // NOTE: Add element to end of list (lvalue reference overload)
  void add(const T &value) {
    if (current_size == capacity) {
      resize(capacity == 0 ? 1 : capacity * 2);
    }
    data[current_size++] = value;
  }

  // NOTE: Add element to end of list (rvalue reference overload for move-only
  // types)
  void add(T &&value) {
    if (current_size == capacity) {
      resize(capacity == 0 ? 1 : capacity * 2);
    }
    data[current_size++] = std::move(value); // Move the value into the array
  }

  // NOTE: Add element at index
  void add(const T &value, int index) {
    if (index < 0 || index > current_size) {
      throw std::out_of_range("Index out of bounds");
    }
    if (current_size == capacity) {
      resize(capacity == 0 ? 1 : capacity * 2);
    }
    for (int i = current_size; i > index; --i) {
      data[i] = std::move(data[i - 1]); // Move elements to make space
    }
    data[index] = value;
    ++current_size;
  }

  // NOTE: Add element at index (rvalue overload)
  void add(T &&value, int index) {
    if (index < 0 || index > current_size) {
      throw std::out_of_range("Index out of bounds");
    }
    if (current_size == capacity) {
      resize(capacity == 0 ? 1 : capacity * 2);
    }
    for (int i = current_size; i > index; --i) {
      data[i] = std::move(data[i - 1]); // Move elements to make space
    }
    data[index] = std::move(value); // Move the value into the array
    ++current_size;
  }

  // NOTE: Get element at index
  T &get(int index) const {
    if (index < 0 || index >= current_size) {
      throw std::out_of_range("Index out of bounds");
    }
    return data[index];
  }

  // NOTE: Remove element at index
  void remove(int index) {
    if (index < 0 || index >= current_size) {
      throw std::out_of_range("Index out of bounds");
    }
    for (int i = index; i < current_size - 1; ++i) {
      data[i] = std::move(data[i + 1]); // Move elements down
    }
    --current_size;
  }

  // NOTE: Number of elements in list
  int size() const { return current_size; }

  // NOTE: Check if list is empty
  bool isEmpty() const { return current_size == 0; }

  // NOTE: Check if list contains item
  bool contains(const T &value) const {
    for (int i = 0; i < current_size; ++i) {
      if (data[i] == value) {
        return true;
      }
    }
    return false;
  }

  // NOTE: Iterator support
  class Iterator {
  private:
    T *ptr;

  public:
    Iterator(T *ptr) : ptr(ptr) {}

    T &operator*() const { return *ptr; }

    Iterator &operator++() {
      ++ptr;
      return *this;
    }

    bool operator!=(const Iterator &other) const { return ptr != other.ptr; }
  };

  Iterator begin() { return Iterator(data); }
  Iterator end() { return Iterator(data + current_size); }
};
