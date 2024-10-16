#pragma once

#include "array_list.hpp"
#include <string>
#include <unordered_map>

class TrieNode {
public:
    char character;
    bool is_end_of_word;
    std::unordered_map<char, TrieNode*> children;
    ArrayList<std::string> files;

    TrieNode(char character) : character(character), is_end_of_word(false) {}
};

class Trie {
public:
    TrieNode* root;

    Trie() : root(new TrieNode('\0')) {}

    void insert(const std::string& word, const std::string& file);

    ArrayList<std::string> search(const std::string& prefix);
};