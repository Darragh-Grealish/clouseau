#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

template <typename K, typename V> class HashMap {
private:
  struct Node {
    K key;
    V value;
    bool isOccupied;
    bool isDeleted;

    Node() : isOccupied(false), isDeleted(false) {}
    Node(const K &k, const V &v)
        : key(k), value(v), isOccupied(true), isDeleted(false) {}
  };

  Node *buckets;
  int capacity;
  int numElements;
  static constexpr float MAX_LOAD_FACTOR = 0.7f;
  static constexpr int INITIAL_CAPACITY = 64;

  int hashFunction(const K &key) const {
    std::stringstream ss;
    ss << key;
    std::string str = ss.str();

    // INFO: DJB2 algorithm
    unsigned long hash = 5381;
    for (char c : str) {
      hash = ((hash << 5) + hash) + c;
    }

    return hash;
  }

  int probe(int hash, int i) const {
    return (hash + (i + i * i) / 2) & (capacity - 1);
  }

  void growIfNeeded() {
    if (static_cast<float>(numElements) / capacity >= MAX_LOAD_FACTOR) {
      resize(capacity * 2);
    }
  }

  void resize(int newCapacity) {
    Node *oldBuckets = buckets;
    int oldCapacity = capacity;

    buckets = new Node[newCapacity];
    capacity = newCapacity;
    numElements = 0;

    for (int i = 0; i < oldCapacity; i++) {
      if (oldBuckets[i].isOccupied && !oldBuckets[i].isDeleted) {
        insert(oldBuckets[i].key, oldBuckets[i].value);
      }
    }

    delete[] oldBuckets;
  }

public:
  HashMap() : capacity(INITIAL_CAPACITY), numElements(0) {
    buckets = new Node[capacity];
  }

  ~HashMap() { delete[] buckets; }

  void insert(const K &key, const V &value) {
    growIfNeeded();

    int hash = hashFunction(key);
    int i = 0;
    int index;
    int firstDeleted = capacity;

    do {
      index = probe(hash, i++);

      if (!buckets[index].isOccupied) {
        if (firstDeleted != capacity) {
          index = firstDeleted;
        }
        buckets[index] = Node(key, value);
        numElements++;
        return;
      } else if (buckets[index].isDeleted && firstDeleted == capacity) {
        firstDeleted = index;
      } else if (!buckets[index].isDeleted && buckets[index].key == key) {
        buckets[index].value = value;
        return;
      }
    } while (i < capacity);

    throw std::runtime_error("HashMap is full");
  }

  V &operator[](const K &key) {
    growIfNeeded();

    int hash = hashFunction(key);
    int i = 0;
    int index;
    int firstDeleted = capacity;

    do {
      index = probe(hash, i++);

      if (!buckets[index].isOccupied) {
        if (firstDeleted != capacity) {
          index = firstDeleted;
        }
        buckets[index] = Node(key, V());
        numElements++;
        return buckets[index].value;
      } else if (buckets[index].isDeleted && firstDeleted == capacity) {
        firstDeleted = index;
      } else if (!buckets[index].isDeleted && buckets[index].key == key) {
        return buckets[index].value;
      }
    } while (i < capacity);

    throw std::runtime_error("HashMap is full");
  }

  bool erase(const K &key) {
    int hash = hashFunction(key);
    int i = 0;
    int index;

    do {
      index = probe(hash, i++);

      if (!buckets[index].isOccupied) {
        return false;
      }

      if (!buckets[index].isDeleted && buckets[index].key == key) {
        buckets[index].isDeleted = true;
        numElements--;
        return true;
      }
    } while (i < capacity);

    return false;
  }

  int size() const { return numElements; }
  bool empty() const { return numElements == 0; }

  void clear() {
    for (int i = 0; i < capacity; i++) {
      buckets[i].isOccupied = false;
      buckets[i].isDeleted = false;
    }
    numElements = 0;
  }

  // Iterator implementation
  class Iterator {
  private:
    Node *buckets;
    int capacity;
    int index;

    void findNext() {
      while (index < capacity &&
             (!buckets[index].isOccupied || buckets[index].isDeleted)) {
        ++index;
      }
    }

  public:
    Iterator(Node *b, int c, int i) : buckets(b), capacity(c), index(i) {
      findNext();
    }

    Iterator &operator++() {
      if (index < capacity) {
        ++index;
        findNext();
      }
      return *this;
    }

    bool operator==(const Iterator &other) const {
      return index == other.index;
    }

    bool operator!=(const Iterator &other) const { return !(*this == other); }

    Node &operator*() { return buckets[index]; }
  };

  Iterator begin() { return Iterator(buckets, capacity, 0); }

  Iterator end() { return Iterator(buckets, capacity, capacity); }

  Iterator find(const K &key) {
    int hash = hashFunction(key);
    int i = 0;
    int index;

    do {
      index = probe(hash, i++);

      if (!buckets[index].isOccupied) {
        return end();
      }

      if (!buckets[index].isDeleted && buckets[index].key == key) {
        return Iterator(buckets, capacity, index);
      }
    } while (i < capacity);

    return end();
  }
};
