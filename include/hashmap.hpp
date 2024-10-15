#pragma once
#include <functional>
using namespace std;

template <typename K, typename V> class HashNode {
public:
    K key;
    V value;

    HashNode(const K& key, const V& value) : key(key), value(value) {}
};

template <typename K, typename V> class HashMap {
public:
    HashNode<K, V>** arr;
    int capacity;
    int mapsize;
    float loadFactor;
    std::mutex* mtx = nullptr;

    HashMap() {
        capacity = 40;
        mapsize = 0;
        loadFactor = 0.75;
        arr = new HashNode<K, V>*[capacity];
        mtx = new std::mutex();

        for (int i = 0; i < capacity; i++)
            arr[i] = nullptr;
    }

    ~HashMap() {
        delete mtx;
    }

    int firstHash(const K& key) {
        size_t hash = std::hash<K>()(key); 
        return hash % capacity;
    }

    int secondHash(const K& key) {
        size_t hash = std::hash<K>()(key); 
        return (7 - (hash % 7));
    }

    int size() { return mapsize; }

    bool empty() { 
        std::lock_guard<std::mutex> lock(*mtx);
        return mapsize == 0; 
        }

    void rehash() {
        int oldCapacity = capacity;
        capacity = 2 * capacity;
        HashNode<K, V>** oldArr = arr;
        arr = new HashNode<K, V>*[capacity]; 

        for (int i = 0; i < capacity; i++) {
            arr[i] = nullptr;
        }

        mapsize = 0; // Reset mapsize, will be re-incremented in insert()

        // Insert old -> new array
        for (int i = 0; i < oldCapacity; i++) {
            if (oldArr[i] != nullptr) {
                insertWithoutLock(oldArr[i]->key, oldArr[i]->value);
            }
        }
        delete[] oldArr;
    }

    void insert(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(*mtx);
        if ((float)mapsize / capacity >= loadFactor) { rehash(); }
        
        insertWithoutLock(key, value);
    }

    void insertWithoutLock(const K& key, const V& value) {
        HashNode<K, V>* temp = new HashNode<K, V>(key, value);
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);

        while (arr[hash1] != nullptr && arr[hash1]->key != key) {
            hash1 = (hash1 + hash2) % capacity; 
        }

        if (arr[hash1] == nullptr) { 
            mapsize++; 
        }
        
        arr[hash1] = temp;  // Insert pair
    }

    V erase(const K& key) {
        std::lock_guard<std::mutex> lock(*mtx);
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);

        while (arr[hash1] != nullptr) {
            if (arr[hash1]->key == key) {
                HashNode<K, V>* temp = arr[hash1];
                arr[hash1] = new HashNode<K, V>("", V());
                return temp->value;
            }
            hash1 = (hash1 + hash2) % capacity;
        }
        return V();
    }

    V& operator[](const K& key) {
        std::lock_guard<std::mutex> lock(*mtx);
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);

        while (arr[hash1] != nullptr && arr[hash1]->key != key) {
            hash1 = (hash1 + hash2) % capacity;
        }

        if (arr[hash1] == nullptr) {
            arr[hash1] = new HashNode<K, V>(key, V()); 
            mapsize++;
        }

        return arr[hash1]->value;
    }   

    class HashIterator {
    public:
        HashNode<K, V>** current;
        HashNode<K, V>** end;

        HashIterator(HashNode<K, V>** curr, HashNode<K, V>** end) : current(curr), end(end) {}

        bool operator!=(const HashIterator& other) const {
            return current != other.current;
        }

        bool operator==(const HashIterator& other) const {
            return current == other.current;  
        }   

        void operator++() {
            do {
                current++;
            } while (current != end && (*current) == nullptr);
        }

        HashNode<K, V>& operator*() {
            return **current;
        }
    };

    HashIterator begin() {
        for (int i = 0; i < capacity; i++) {
            if (arr[i] != nullptr) {
                return HashIterator(&arr[i], &arr[capacity]);
            }
        }
        return end();
    }

    HashIterator end() {
        return HashIterator(&arr[capacity], &arr[capacity]);
    }

    HashIterator find(const K& key) {
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);

        while (arr[hash1] != nullptr) {
            if (arr[hash1]->key == key) {
                return HashIterator(&arr[hash1], &arr[capacity]);
            }
            hash1 = (hash1 + hash2) % capacity;
        }
        return end();
    }
};

