#pragma once
#include <initializer_list>
#include <stdexcept>

template <typename T> class ArrayList {
public:
  ArrayList()
      : current_size(0), capacity(INITIAL_CAPACITY),
        data(new T[INITIAL_CAPACITY]) {}

  ArrayList(const ArrayList &other)
      : current_size(other.current_size), capacity(other.capacity),
        data(new T[other.capacity]) {
    for (int i = 0; i < current_size; ++i) {
      data[i] = other.data[i];
    }
  }

  ArrayList(std::initializer_list<T> init_list)
      : current_size(init_list.size()), capacity(init_list.size()),
        data(new T[init_list.size()]) {
    int i = 0;
    for (const T &element : init_list) {
      data[i++] = element;
    }
  }

  ~ArrayList() { delete[] data; }

  void remove(int index) {
    if (index < 0 || index >= current_size) {
      throw std::out_of_range("Index out of bounds");
    }
    for (int i = index; i < current_size - 1; ++i) {
      data[i] = data[i + 1];
    }
    current_size--;
  }

  bool contains(T element) const {
    for (int i = 0; i < current_size; ++i) {
      if (data[i] == element) {
        return true;
      }
    }
    return false;
  }

  void add(T element) {
    if (current_size == capacity) {
      resize();
    }
    data[current_size++] = element;
  }

  void add(T element, int index) {
    if (index < 0 || index > current_size) {
      throw std::out_of_range("Index out of bounds");
    }
    if (current_size == capacity) {
      resize();
    }
    for (int i = current_size; i > index; --i) {
      data[i] = data[i - 1];
    }
    data[index] = element;
    current_size++;
  }

  T get(int index) const {
    if (index < 0 || index >= current_size) {
      throw std::out_of_range("Index out of bounds");
    }
    return data[index];
  }

  int size() const { return current_size; }
  bool isEmpty() const { return current_size == 0; }

  // NOTE: Range-based for loop support
  class Iterator {
  public:
    Iterator(T *ptr) : ptr(ptr) {}

    Iterator &operator++() {
      ++ptr;
      return *this;
    }

    bool operator!=(const Iterator &other) const { return ptr != other.ptr; }

    T &operator*() const { return *ptr; }

  private:
    T *ptr;
  };

  Iterator begin() { return Iterator(data); }

  Iterator end() { return Iterator(data + current_size); }

private:
  // NOTE: Doubles capacity (complexity O(n) but amortized O(1))
  void resize() {
    capacity *= 2;
    T *new_data = new T[capacity];
    for (int i = 0; i < current_size; ++i) {
      new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
  }

  static const int INITIAL_CAPACITY = 10;
  int current_size;
  int capacity;
  T *data;
};
