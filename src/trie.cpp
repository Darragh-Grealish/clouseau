#include "trie.hpp"

void Trie::insert(const std::string &word) {
  TrieNode *current = root;
  for (char c : word) {
    if (current->children.find(c) == current->children.end()) { // character not found
      current->children[c] = new TrieNode(c);
    }
    current = current->children[c]; // move to the next node
  }
  current->is_end_of_word = true; // mark the end of the word
}

ArrayList<std::string> Trie::search(const std::string &prefix) {
  TrieNode *current = root;
  for (char c : prefix) {
    if (current->children.find(c) == current->children.end()) {
      // std::cout << "keywords not found" << std::endl;
      return ArrayList<std::string>(); // character not found
    }
    current = current->children[c]; // move to the next node
  }

  ArrayList<std::string> results;
  collect_all_words(current, prefix, results); 
  return results;
}

void Trie::collect_all_words(TrieNode *node, const std::string &prefix, ArrayList<std::string> &results) {
  if (node->is_end_of_word) {
    results.push_back(prefix); // add the word to the results
  }

  for (auto const &pair : node->children) { // iterate over all children
    collect_all_words(pair.value, prefix + pair.key, results);
  }
}
