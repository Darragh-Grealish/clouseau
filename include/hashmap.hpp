#pragma once
#include <functional>
#include <stdexcept>

template <typename K, typename V>
class HashMap {
private:
    struct Node {
        K key;
        V value;
        bool isOccupied;
        bool isDeleted;
        
        Node() : isOccupied(false), isDeleted(false) {}
        Node(const K& k, const V& v) : key(k), value(v), isOccupied(true), isDeleted(false) {}
    };

    Node* buckets;
    size_t capacity;
    size_t numElements;
    static constexpr float MAX_LOAD_FACTOR = 0.7f;
    static constexpr size_t INITIAL_CAPACITY = 64;

    size_t hashFunction(const K& key) const {
        return std::hash<K>{}(key);
    }

    size_t probe(size_t hash, size_t i) const {
        return (hash + (i + i * i) / 2) & (capacity - 1);
    }

    void growIfNeeded() {
        if (static_cast<float>(numElements) / capacity >= MAX_LOAD_FACTOR) {
            resize(capacity * 2);
        }
    }

    void resize(size_t newCapacity) {
        Node* oldBuckets = buckets;
        size_t oldCapacity = capacity;
        
        buckets = new Node[newCapacity];
        capacity = newCapacity;
        numElements = 0;

        for (size_t i = 0; i < oldCapacity; i++) {
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

    ~HashMap() {
        delete[] buckets;
    }

    void insert(const K& key, const V& value) {
        growIfNeeded();
        
        size_t hash = hashFunction(key);
        size_t i = 0;
        size_t index;
        size_t firstDeleted = capacity;

        do {
            index = probe(hash, i++);
            
            if (!buckets[index].isOccupied) {
                if (firstDeleted != capacity) {
                    index = firstDeleted;
                }
                buckets[index] = Node(key, value);
                numElements++;
                return;
            }
            else if (buckets[index].isDeleted && firstDeleted == capacity) {
                firstDeleted = index;
            }
            else if (!buckets[index].isDeleted && buckets[index].key == key) {
                buckets[index].value = value;
                return;
            }
        } while (i < capacity);

        throw std::runtime_error("HashMap is full");
    }

    V& operator[](const K& key) {
        growIfNeeded();
        
        size_t hash = hashFunction(key);
        size_t i = 0;
        size_t index;
        size_t firstDeleted = capacity;

        do {
            index = probe(hash, i++);
            
            if (!buckets[index].isOccupied) {
                if (firstDeleted != capacity) {
                    index = firstDeleted;
                }
                buckets[index] = Node(key, V());
                numElements++;
                return buckets[index].value;
            }
            else if (buckets[index].isDeleted && firstDeleted == capacity) {
                firstDeleted = index;
            }
            else if (!buckets[index].isDeleted && buckets[index].key == key) {
                return buckets[index].value;
            }
        } while (i < capacity);

        throw std::runtime_error("HashMap is full");
    }

    bool erase(const K& key) {
        size_t hash = hashFunction(key);
        size_t i = 0;
        size_t index;

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

    size_t size() const { return numElements; }
    bool empty() const { return numElements == 0; }

    void clear() {
        for (size_t i = 0; i < capacity; i++) {
            buckets[i].isOccupied = false;
            buckets[i].isDeleted = false;
        }
        numElements = 0;
    }

    // Iterator implementation
    class Iterator {
    private:
        Node* buckets;
        size_t capacity;
        size_t index;

        void findNext() {
            while (index < capacity && 
                   (!buckets[index].isOccupied || buckets[index].isDeleted)) {
                ++index;
            }
        }

    public:
        Iterator(Node* b, size_t c, size_t i) 
            : buckets(b), capacity(c), index(i) {
            findNext();
        }

        Iterator& operator++() {
            if (index < capacity) {
                ++index;
                findNext();
            }
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return index == other.index;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        Node& operator*() {
            return buckets[index];
        }
    };

    Iterator begin() {
        return Iterator(buckets, capacity, 0);
    }

    Iterator end() {
        return Iterator(buckets, capacity, capacity);
    }

    Iterator find(const K& key) {
        size_t hash = hashFunction(key);
        size_t i = 0;
        size_t index;

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
