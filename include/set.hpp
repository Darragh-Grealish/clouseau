#pragma once

#include <initializer_list>
#include <mutex>
#include <stdexcept>
#include <utility>
#include <algorithm> 

template <typename T>
class Set {
public:
    class Iterator {
    public:
        Iterator(T* ptr) : ptr_(ptr) {}
        Iterator operator++() {
            ++ptr_;
            return *this;
        } 
        bool operator!=(const Iterator& other) const {
            return ptr_ != other.ptr_;
        } 
        T& operator*() const { return *ptr_; } 

    private:
        T* ptr_;
    };

    Set();
    explicit Set(size_t capacity);
    Set(std::initializer_list<T> list);
    ~Set();

    Set(const Set& other);
    Set& operator=(const Set& other);
    Set(Set&& other) noexcept;
    Set& operator=(Set&& other) noexcept;

    bool contains(const T& value) const;
    void insert(const T& value);  
    template <typename InputIt>
    void insert(InputIt first, InputIt last); 
    void erase(const T& value);
    Set<T> intersect(const Set<T>& other) const; 
    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    void clear();
    Iterator begin() { return Iterator(data_); }
    Iterator end() { return Iterator(data_ + size_); }

private:
    T* data_;
    size_t size_;
    size_t capacity_;
    std::mutex mtx_;

    void resize_if_needed();
    void copy_from(const Set& other);
    void move_from(Set&& other);
};


template <typename T>
Set<T>::Set() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
Set<T>::Set(size_t capacity) : size_(0), capacity_(capacity) {
    data_ = new T[capacity];
}

template <typename T>
Set<T>::Set(std::initializer_list<T> list) : Set(list.size()) {
    size_ = 0;
    for (const T& element : list) {
        insert(element); 
    }
}

template <typename T>
Set<T>::~Set() { delete[] data_; }

template <typename T>
Set<T>::Set(const Set& other) { copy_from(other); }

template <typename T>
Set<T>& Set<T>::operator=(const Set& other) {
    if (this != &other) {
        delete[] data_;
        copy_from(other);
    }
    return *this;
}

template <typename T>
Set<T>::Set(Set&& other) noexcept { move_from(std::move(other)); }

template <typename T>
Set<T>& Set<T>::operator=(Set&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        move_from(std::move(other));
    }
    return *this;
}

template <typename T>
bool Set<T>::contains(const T& value) const {
    for (size_t i = 0; i < size_; ++i) {
        if (data_[i] == value) {
            return true;
        }
    }
    return false;
}

template <typename T>
void Set<T>::insert(const T& value) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (!contains(value)) {
        resize_if_needed();
        data_[size_++] = value;
    }
}

template <typename T>
template <typename InputIt>
void Set<T>::insert(InputIt first, InputIt last) {
    for (InputIt it = first; it != last; ++it) {
        insert(*it); 
    }
}

template <typename T>
void Set<T>::erase(const T& value) {
    std::lock_guard<std::mutex> lock(mtx_);
    for (size_t i = 0; i < size_; ++i) {
        if (data_[i] == value) {
            data_[i] = data_[--size_];  
            break;
        }
    }
}

template <typename T>
Set<T> Set<T>::intersect(const Set<T>& other) const {
    Set<T> result;
    for (size_t i = 0; i < size_; ++i) {
        if (other.contains(data_[i])) {
            result.insert(data_[i]);
        }
    }
    return result;
}

template <typename T>
size_t Set<T>::size() const { return size_; }

template <typename T>
size_t Set<T>::capacity() const { return capacity_; }

template <typename T>
bool Set<T>::empty() const { return size_ == 0; }

template <typename T>
void Set<T>::clear() {
    std::lock_guard<std::mutex> lock(mtx_);
    size_ = 0;
}

template <typename T>
void Set<T>::resize_if_needed() {
    if (size_ == capacity_) {
        size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        T* new_data = new T[new_capacity];

        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
}

template <typename T>
void Set<T>::copy_from(const Set& other) {
    data_ = new T[other.capacity_];
    size_ = other.size_;
    capacity_ = other.capacity_;
    for (size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

template <typename T>
void Set<T>::move_from(Set&& other) {
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}
