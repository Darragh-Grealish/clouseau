#pragma once
#include <new>       
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

    int firstHash(K key) { return key % capacity; }

    int secondHash(K key) { return (7 - (key % 7)); }

    int size() { return mapsize; }

    bool isEmpty() { return mapsize == 0; }

    void rehash() {
        int oldCapacity = capacity;
        capacity = 2 * capacity;
        HashNode<K, V>** oldArr = arr;
        arr = new HashNode<K, V>*[capacity]; 

        for (int i = 0; i < capacity; i++) {
            arr[i] = NULL;
        }

        mapsize = 0; // Reset mapsize, will be re-incremented in insertNode

        // Insert old -> new array
        for (int i = 0; i < oldCapacity; i++) {
            if (oldArr[i] != NULL && oldArr[i]->key != -1) {
                insertNode(oldArr[i]->key, oldArr[i]->value);
            }
        }

        delete[] oldArr;
    }

    void insertNode(K key, V value) {
        if ((float)size / capacity >= loadFactor) { rehash(); }

        HashNode<K, V>* temp = new HashNode<K, V>(key, value);
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);

        while (arr[hash1] != NULL && arr[hash1]->key != key && arr[hash1]->key != -1) {
            hash1 = (hash1 + hash2) % capacity; 
        }

        if (arr[hash1] == NULL || arr[hash1]->key == -1) { size++; }
        arr[hash1] = temp; // insert pair
    }

    V deleteNode(int key) {
        // Apply hash function
        // to find index for given key
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);

        while (arr[hash1] != NULL) {
            if (arr[hash1]->key == key) {
                HashNode<K, V>* temp = arr[hash1];
                arr[hash1] = new HashNode<K, V>(key, -1);

                // If reducing the size every time we delete a node and flag -1
                // The true size of the map will be incorrect as nodes are flagged not removed
                // Or should we reduce size in our implementation?
                // size--;  
                return temp->value;
            }
            hash1 = (hash1 + hash2) % capacity;
        }
        return -1;
    }

    V get(int key) {
        int hash1 = firstHash(key);
        int hash2 = secondHash(key);
        int counter = 0;

        while (arr[hash1] != NULL) { 
            // stop infinite loop
            if (counter > capacity) { return -1; } // capacity can be replaced with size, if size-- not in deleteNode()
            counter++;

            if (arr[hash1]->key == key){
                return arr[hash1]->value;
            }
            hash1 = (hash1 + hash2) % capacity;
        }
        return -1;
    }

    void display() {
        for (int i = 0; i < capacity; i++) {
            if (arr[i] != NULL && arr[i]->key != -1) {
                cout << "key = " << arr[i]->key << "  value = " << arr[i]->value << endl;
            }
        }
    }
};
