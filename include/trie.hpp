#ifndef TRIE_HPP
#define TRIE_HPP

#include <string>

class TrieNode {
public:
    bool endofWord;
    TrieNode* children[26];

    
    
    TrieNode();
};

class Trie {
private:
    TrieNode* root;

public:
 
    Trie();

   
    void insert(const std::string& word);


    bool search(const std::string& word) const;


    bool startsWith(const std::string& prefix) const;


    void deleteWord(const std::string& word);


    void print() const;

    void insertFromFile(const std::string& filepath);

    ~Trie();
};

#endif
