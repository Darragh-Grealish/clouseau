#pragma once
#include <new>      
#include <string>
#include <functional>
using namespace std;

template <typename K, typename V> class HashNode {
public:
    V value;
    K key;

    HashNode(K key, V value) {
        this->value = value;
        this->key = key;
    }
};


template <typename K, typename V> class HashMap {
public:
    HashNode<K, V>** arr;
    int capacity;
    int mapsize;
    float loadFactor;

    HashMap() {
        capacity = 40;
        mapsize = 0;
        loadFactor = 0.75;
        arr = new HashNode<K, V>*[capacity];

        for (int i = 0; i < capacity; i++)
            arr[i] = NULL;
    }

    // int firstHash(K key) { return key % capacity; }

    // int secondHash(K key) { return (7 - (key % 7)); }

    // Hash function for strings
    int firstHash(const K& key) {
        size_t hash = std::hash<K>()(key);  // Correct way to use std::hash
        return hash % capacity;
    }

    int secondHash(const K& key) {
        size_t hash = std::hash<K>()(key);  // Correct way to use std::hash
        return (7 - (hash % 7));
    }

    int size() { return mapsize; }

    bool empty() { return mapsize == 0; }

    void rehash() {
        int oldCapacity = capacity;
        capacity = 2 * capacity;
        HashNode<K, V>** oldArr = arr;
        arr = new HashNode<K, V>*[capacity]; 

        for (int i = 0; i < capacity; i++) {
            arr[i] = NULL;
        }

        mapsize = 0; // Reset mapsize, will be re-incremented in insert()

        // Insert old -> new array
        for (int i = 0; i < oldCapacity; i++) {
            if (oldArr[i] != NULL && !oldArr[i]->key.empty()) {
                insert(oldArr[i]->key, oldArr[i]->value);
            }
        }
        delete[] oldArr;
    }

    void insert(const K& key, const V& value) {
        if ((float)mapsize / capacity >= loadFactor) { rehash(); }

        HashNode<K, V>* temp = new HashNode<K, V>(key, value);
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);

        while (arr[hash1] != NULL && arr[hash1]->key != key && !arr[hash1]->key.empty()) {
            hash1 = (hash1 + hash2) % capacity; 
        }

        if (arr[hash1] == NULL || arr[hash1]->key.empty()) { mapsize++; }
        arr[hash1] = temp; // insert pair
    }

    V erase(const K& key) {
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);

        while (arr[hash1] != NULL) {
            if (arr[hash1]->key == key) {
                HashNode<K, V>* temp = arr[hash1];
                arr[hash1] = new HashNode<K, V>("", V());

                // If reducing the size every time we delete a node and flag -1
                // The true size of the map will be incorrect as nodes are flagged not removed
                // Or should we reduce size in our implementation?
                // size--;  
                return temp->value;
            }
            hash1 = (hash1 + hash2) % capacity;
        }
        return V();
    }

    V find(const K& key) {
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);
        int counter = 0;

        while (arr[hash1] != NULL) { 
            // stop infinite loop
            if (counter > capacity) { return V(); } // capacity can be replaced with size, if size-- not in erase()
            counter++;

            if (arr[hash1]->key == key){
                return arr[hash1]->value;
            }
            hash1 = (hash1 + hash2) % capacity;
        }
        return V();
    }

    V& operator[](const K& key) {
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);

        while (arr[hash1] != NULL && arr[hash1]->key != key) {
            hash1 = (hash1 + hash2) % capacity;
        }

        if (arr[hash1] == NULL) {
            arr[hash1] = new HashNode<K, V>(key, V()); // Create a new node if key doesn't exist
            mapsize++;
        }

        return arr[hash1]->value;
    }   

    class iterator {
    public:
        HashNode<K, V>** current;
        HashNode<K, V>** end;

        iterator(HashNode<K, V>** curr, HashNode<K, V>** end) : current(curr), end(end) {}

        bool operator!=(const iterator& other) const {
            return current != other.current;
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

    iterator begin() {
        for (int i = 0; i < capacity; i++) {
            if (arr[i] != nullptr) {
                return iterator(&arr[i], &arr[capacity]);
            }
        }
        return end();
    }

    iterator end() {
        return iterator(&arr[capacity], &arr[capacity]);
    }

};
