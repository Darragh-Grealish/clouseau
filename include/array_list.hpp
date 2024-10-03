#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdexcept>

const int INITIAL_CAPACITY = 10;
const int GROWTH_FACTOR = 2;

template <typename T>
class ArrayList {
public:
    ArrayList() : current_size(0), capacity(INITIAL_CAPACITY), data(new T[INITIAL_CAPACITY]) {}

    ~ArrayList() {
        delete[] data;
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

    void set(int index, T element) {
        if (index < 0 || index >= current_size) {
            throw std::out_of_range("Index out of bounds");
        }
        data[index] = element;
    }

    int size() const {
        return current_size;
    }

    void remove(int index) {
        if (index < 0 || index >= current_size) {
            throw std::out_of_range("Index out of bounds");
        }
        for (int i = index; i < current_size - 1; i++) {
            data[i] = data[i + 1];
        }
        current_size--;
    }

    int indexOf(T element) const {
        for (int i = 0; i < current_size; i++) {
            if (data[i] == element) {
                return i;
            }
        }
        return -1;
    }

    bool isEmpty() const {
        return current_size == 0;
    }

    T* toArray() const {
        T* arr = new T[current_size];
        for (int i = 0; i < current_size; i++) {
            arr[i] = data[i];
        }
        return arr;
    }

    bool contains(T element) const {
        for (int i = 0; i < current_size; i++) {
            if (data[i] == element) {
                return true;
            }
        }
        return false;
    }

private:
    void resize() {
        capacity *= GROWTH_FACTOR;
        T* new_data = new T[capacity];
        for (int i = 0; i < current_size; i++) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
    }

    T* data;
    int current_size;
    int capacity;
};

#endif // ARRAY_LIST_H

