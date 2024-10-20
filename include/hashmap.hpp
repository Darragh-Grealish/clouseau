#pragma once
#include <functional>
#include <vector>
#include <mutex>
using namespace std;


template <typename K, typename V, typename Hash = std::hash<K>>
class HashMap {
private:
    struct Node {
        K first;
        V second;
        Node* next;
        Node(const K& k, const V& v) : first(k), second(v), next(nullptr) {}
    };

    std::vector<std::vector<Node*>> buckets;
    std::vector<std::mutex>* mutexes;
    Hash hashFunction;

    // mutex for key
    std::mutex& getMutex(const K& first) {
        std::size_t hashV = hashFunction(first);
        return (*mutexes)[hashV % mutexes->size()]; // dereference pointer
    }

public:
    HashMap(std::size_t num_buckets = 16) : buckets(num_buckets), mutexes(new std::vector<std::mutex>(num_buckets)) {}

    int size() {
        int size = 0;
        for (auto& bucket : buckets) {
            std::lock_guard lock((*mutexes)[size]);
            Node* currentNode;
            if (bucket.empty()) {currentNode = nullptr;} else {currentNode = bucket[0];}
            while (currentNode != nullptr) {
                size++;
                currentNode = currentNode->next;
            }
        }
        return size;
    }

    bool empty() {
        for (auto& bucket : buckets) {
            std::lock_guard lock((*mutexes)[0]);
            if (!bucket.empty()) {
                return false;
            }
        }
        return true;
    }

    void clear() {
        for (auto& bucket : buckets) {
            std::lock_guard lock((*mutexes)[0]);
            Node* currentNode;
            if (bucket.empty()) {
                currentNode = nullptr;
            } else {
                currentNode = bucket[0];
            }
            while (currentNode != nullptr) {
                Node* temp = currentNode;
                currentNode = currentNode->next;
                delete temp;
            }
            bucket.clear();
        }
    }

    void insert(const K& first, const V& second) {
        std::unique_lock lock(getMutex(first));
        std::size_t hashV = hashFunction(first);
        std::size_t index = hashV % buckets.size();

        Node* newNode = new Node(first, second);
        if (buckets[index].empty()) {
            newNode->next = nullptr;
        } else {
            newNode->next = buckets[index][0];
        }
        buckets[index] = {newNode};
    }

    void erase(const K& first) {
        std::unique_lock lock(getMutex(first));
        std::size_t hashV = hashFunction(first);
        std::size_t index = hashV % buckets.size();

        Node* currentNode;
        if (buckets[index].empty()) {
            currentNode = nullptr;
        } else {
            currentNode = buckets[index][0];
        }
        Node* prev = nullptr;

        while (currentNode != nullptr) {
            if (currentNode->first == first) {
                if (prev == nullptr) {
                    buckets[index][0] = currentNode->next;
                } else {
                    prev->next = currentNode->next;
                }
                delete currentNode;
                return;
            }
            prev = currentNode;
            currentNode = currentNode->next;  // No need to cast here
        }
    }

    // check if the key exists in the hashmap
    V& operator[](const K& first) {
        std::unique_lock lock(getMutex(first));
        std::size_t hashV = hashFunction(first);
        std::size_t index = hashV % buckets.size();

        Node* currentNode;
        if (buckets[index].empty()) {
            currentNode = nullptr;
        } else {
            currentNode = buckets[index][0];
        }
        while (currentNode != nullptr) {
            if (currentNode->first == first) {
                return currentNode->second;
            }
            currentNode = currentNode->next;
        }

        // key is not found, insert new node with default key
        V defaultV = V();
        Node* newNode = new Node(first, defaultV);
        if (buckets[index].empty()) {
            newNode->next = nullptr;
        } else {
            newNode->next = buckets[index][0];
        }
        buckets[index] = {newNode};
        return newNode->second;  // reference to the new value
    }   

    ~HashMap() {
        for (auto& bucket : buckets) {
            Node* currentNode;
            if (bucket.empty()) {
                currentNode = nullptr;
            } else {
                currentNode = bucket[0];
            }
            while (currentNode != nullptr) {
                Node* temp = currentNode;
                currentNode = currentNode->next;
                delete temp;
            }
        }
        delete mutexes;
    }

class HashIterator {
    private:
        HashMap& hashMapptr;
        typename std::vector<std::vector<Node*>>::iterator bucketIt;
        Node* nodeIt;

    public:
        HashIterator(HashMap& hashMapptr, typename std::vector<std::vector<Node*>>::iterator bucketIt, Node* node)
        : hashMapptr(hashMapptr), bucketIt(bucketIt), nodeIt(node) {}

        bool operator!=(const HashIterator& other) const {
            return bucketIt != other.bucketIt || nodeIt != other.nodeIt;
        }

        bool operator==(const HashIterator& other) const {
            return bucketIt == other.bucketIt && nodeIt == other.nodeIt;
        }

        void operator++() {
            if (nodeIt != nullptr) {
                nodeIt = nodeIt->next;
            }
            while (nodeIt == nullptr && bucketIt != hashMapptr.buckets.end()) {
                ++bucketIt;
                if (bucketIt != hashMapptr.buckets.end()) {
                    if (bucketIt->empty()) {
                        nodeIt = nullptr;
                    } else {
                        nodeIt = (*bucketIt)[0];
                    }
                }
            }
        }

        std::pair<K, V>& operator*() {
            return *reinterpret_cast<std::pair<K, V>*>(&nodeIt->first);
        }   
    };

    HashIterator begin() {
        auto bucketIt = buckets.begin();
        Node* node = nullptr;
        // find first non-empty bucket
        while (bucketIt != buckets.end() && (bucketIt->empty() || (*bucketIt)[0] == nullptr)) {
            ++bucketIt;
        }
        if (bucketIt != buckets.end() && !bucketIt->empty()) {
            node = (*bucketIt)[0]; // set first node
        }
        return HashIterator(*this, bucketIt, node);
    }

    HashIterator end() {
        return HashIterator(*this, buckets.end(), nullptr);
    }

    HashIterator find(const K& key) {
        size_t index = hashFunction(key) % buckets.size();
        Node* currentNode;
        if (buckets[index].empty()) {
            currentNode = nullptr;
        } else {
            currentNode = buckets[index][0];
        }
        while (currentNode != nullptr) {
            if (currentNode->first == key) {
                return HashIterator(*this, buckets.begin() + index, currentNode);
            }
            currentNode = currentNode->next;
        }
        return HashIterator(*this, buckets.begin() + index, nullptr);
    }
};

