#pragma once

#include "array_list.hpp"
#include "hashmap.hpp"

#include <string>

class TrieNode {
public:
  char character;
  bool is_end_of_word;
  HashMap<char, TrieNode *> children;

  TrieNode(char character) : character(character), is_end_of_word(false) {}
};

class Trie {
public:
  TrieNode *root;

  Trie() : root(new TrieNode('\0')) {}

  void insert(const std::string &word);
  void collect_all_words(TrieNode *node, const std::string &prefix, ArrayList<std::string> &results);

  ArrayList<std::string> search(const std::string &prefix);
};
