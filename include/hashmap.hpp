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

    int hashCode(K key) { return key % capacity; }

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
        if ((float)mapsize / capacity >= loadFactor) { rehash(); }

        HashNode<K, V>* temp = new HashNode<K, V>(key, value);
        int hashIndex = hashCode(key);

        // Linear Probing
        while (arr[hashIndex] != NULL && arr[hashIndex]->key != key && arr[hashIndex]->key != -1) {
            hashIndex++;
            hashIndex %= capacity; // wrap around
        }

        if (arr[hashIndex] == NULL || arr[hashIndex]->key == -1) { mapsize++; }
        arr[hashIndex] = temp; // insert pair
    }

    V deleteNode(int key) {
        // Apply hash function
        // to find index for given key
        int hashIndex = hashCode(key);

        while (arr[hashIndex] != NULL) {
            if (arr[hashIndex]->key == key) {
                HashNode<K, V>* temp = arr[hashIndex];
                arr[hashIndex] = new HashNode<K, V>(key, -1);

                // If reducing the mapsize every time we delete a node and flag -1
                // The true mapsize of the map will be incorrect as nodes are flagged not removed
                // Or should we reduce mapsize in our implementation?
                // mapsize--;  
                return temp->value;
            }
            hashIndex++;
            hashIndex %= capacity;
        }

        return -1;
    }

    V get(int key) {
        int hashIndex = hashCode(key);
        int counter = 0;

        while (arr[hashIndex] != NULL) { 
            // stop infinite loop
            if (counter > capacity) { return -1; } // capacity can be replaced with mapsize, if mapsize-- not in deleteNode()
            counter++;

            if (arr[hashIndex]->key == key){
                return arr[hashIndex]->value;
            }
            hashIndex++;
            hashIndex %= capacity;
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
