#pragma once
#include <stdexcept>

template <typename T> class ArrayList {
public:
  ArrayList()
      : current_size(0), capacity(INITIAL_CAPACITY),
        data(new T[INITIAL_CAPACITY]) {}

  ~ArrayList() {}

  void remove(int index) {
    if (index < 0 || index >= current_size) {
      throw std::out_of_range("Index out of bounds");
    }
    for (int i = index; i < current_size - 1; ++i) {
      data[i] = data[i + 1];
    }
    current_size--;
  }

  void add(T element) {
    if (current_size == capacity) {
      resize();
    }
    data[current_size++] = element;
  }

  T get(int index) const {
    if (index < 0 || index >= current_size) {
      throw std::out_of_range("Index out of bounds");
    }
    return data[index];
  }

  int size() const { return current_size; }
  bool isEmpty() const { return current_size == 0; }

private:
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
