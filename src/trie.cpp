#include "trie.hpp"

void Trie::insert(const std::string& word, const std::string& file) {
    TrieNode* current = root;
    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode(c);
        }
        current = current->children[c];
    }
    current->is_end_of_word = true;
    current->files.push_back(file);
}

ArrayList<std::string> Trie::search(const std::string& prefix) {
    TrieNode* current = root;
    for (char c : prefix) {
        if (current->children.find(c) == current->children.end()) {
            return ArrayList<std::string>();
        }
        current = current->children[c];
    }
    return current->files;
}
