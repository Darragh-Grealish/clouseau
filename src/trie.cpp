#include "trie.hpp"
#include <iostream>

void Trie::insert(const std::string &word) {
  TrieNode *current = root;
  for (char c : word) {
    if (current->children.find(c) == current->children.end()) {
      current->children[c] = new TrieNode(c);
    }
    current = current->children[c];
  }
  current->is_end_of_word = true;
}

ArrayList<std::string> Trie::search(const std::string &prefix) {
  TrieNode *current = root;
  for (char c : prefix) {
    if (current->children.find(c) == current->children.end()) {
      std::cout << "No keywords found for the given prefix." << std::endl;
      return ArrayList<std::string>();
    }
    current = current->children[c];
  }

  ArrayList<std::string> results;
  collect_all_words(current, prefix, results);
  return results;
}

void Trie::collect_all_words(TrieNode *node, const std::string &prefix, ArrayList<std::string> &results) {
  if (node->is_end_of_word) {
    results.push_back(prefix);
  }

  for (auto const &pair : node->children) {
    collect_all_words(pair.value, prefix + pair.key, results);
  }
}