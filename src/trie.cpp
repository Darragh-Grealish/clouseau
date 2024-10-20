#include "trie.hpp"
#include <iostream>

void Trie::insert(const std::string& word, const std::string& file) {
    TrieNode* current_node = root;
    for (char ch : word) {
        if (current_node->children.find(ch) == current_node->children.end()) {
            current_node->children[ch] = new TrieNode(ch);
        }
        current_node = current_node->children[ch];
    }
    current_node->is_end_of_word = true;
    current_node->files.push_back(file); 
}

ArrayList<std::string> Trie::search(const std::string& prefix) {
    ArrayList<std::string> results;
    TrieNode* current_node = root;

    for (char ch : prefix) {
        if (current_node->children.find(ch) == current_node->children.end()) {
            return results; 
        }
        current_node = current_node->children[ch];
    }

    find_words(current_node, prefix, results);
    return results;
}

void Trie::find_words(TrieNode* node, const std::string& prefix, ArrayList<std::string>& results) {
    if (node->is_end_of_word) {
        results.push_back(prefix); 
    }

    for (const auto& pair : node->children) {
        find_words(pair.second, prefix + pair.first, results); 
    }
}

void Trie::load_index(const Indexer& indexer) { 
    std::unordered_map<std::string, Frequency> index = indexer.get_index(); 

    for (const auto& pair : index) {
        const std::string& word = pair.first;
        const Frequency& freq = pair.second;

        for (const FileFrequency& file_freq : freq.files) {
            insert(word, file_freq.file);
        }
    }

   // std::cout << "Trie loaded with index data." << std::endl;
}