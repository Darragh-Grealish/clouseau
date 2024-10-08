#include "trie.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

TrieNode::TrieNode() {
    endofWord = false;
    for (int i = 0; i < 26; i++) {
        children[i] = nullptr;
    }
}

Trie::Trie() {
    root = new TrieNode();
}

void Trie::insert(const std::string& word) {
    TrieNode* node = root;
    for (char c : word) {
        if (isalpha(c)) {
            c = tolower(c);  
            int index = c - 'a';
            if (!node->children[index]) {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
    }
    node->endofWord = true;
}

bool Trie::search(const std::string& word) const {
    TrieNode* node = root;
    for (char c : word) {
        if (isalpha(c)) {
            c = tolower(c);
            int index = c - 'a';
            if (!node->children[index]) {
                return false;
            }
            node = node->children[index];
        }
    }
    return node->endofWord;
}

bool Trie::startsWith(const std::string& prefix) const {
    TrieNode* node = root;
    for (char c : prefix) {
        if (isalpha(c)) {
            c = tolower(c);
            int index = c - 'a';
            if (!node->children[index]) {
                return false;
            }
            node = node->children[index];
        }
    }
    return true;
}

void Trie::deleteWord(const std::string& word) {
    TrieNode* node = root;
    for (char c : word) {
        if (isalpha(c)) {
            c = tolower(c);
            int index = c - 'a';
            if (!node->children[index]) {
                return;
            }
            node = node->children[index];
        }
    }
    if (node->endofWord) {
        node->endofWord = false;
    }
}

void printHelper(TrieNode* node, std::string prefix) {
    if (node->endofWord) {
        std::cout << prefix << std::endl;
    }
    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            printHelper(node->children[i], prefix + char('a' + i));
        }
    }
}

void Trie::print() const {
    printHelper(root, "");
}

void Trie::insertFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    std::string word;
    while (file >> word) {
        std::string cleanWord;
        for (char c : word) {
            if (isalpha(c)) {
                cleanWord += tolower(c);
            }
        }
        insert(cleanWord);
    }

    file.close();
}

void clearHelper(TrieNode* node) {
    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            clearHelper(node->children[i]);
        }
    }
    delete node;
}

Trie::~Trie() {
    clearHelper(root);
}