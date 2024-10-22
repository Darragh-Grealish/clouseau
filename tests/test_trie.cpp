#include "trie.hpp"
#include <gtest/gtest.h>


// TEST: GIVEN a Trie of 3 words 
//       WHEN when searching prefix 'wo' 
//       THEN 3 words should be returned & size is 3
TEST(TrieTest, SearchPrefixResultsSize) {
    Trie trie;
    trie.insert("word");
    trie.insert("world");
    trie.insert("work");

    ArrayList<std::string> results = trie.search("wo");
    EXPECT_EQ(results.size(), 3);
}

// TEST: GIVEN a Trie of 3 words
//       WHEN when searching prefix 'wo'
//       THEN words should be returned in lexicographical order
TEST(TrieTest, SearchPrefixResultsOrder) {
    Trie trie;
    trie.insert("word");
    trie.insert("world");
    trie.insert("work");
    trie.insert("workplace");

    ArrayList<std::string> results = trie.search("wo");
    EXPECT_EQ(results[0], "word");
    EXPECT_EQ(results[1], "work");
    EXPECT_EQ(results[2], "workplace");
    EXPECT_EQ(results[3], "world");
}

// TEST: GIVEN a Trie of 3 words 
//       WHEN when searching prefix 'wa' 
//       THEN 0 words should be returned as no word has that prefix
TEST(TrieTest, SearchPrefix_NoResults) {
    Trie trie;
    trie.insert("word");
    trie.insert("world");
    trie.insert("work");

    ArrayList<std::string> results = trie.search("wa");
    EXPECT_EQ(results.size(), 0);
}

// TEST: GIVEN an empty Trie
//       WHEN when searching an any prefix
//       THEN results should be 0
TEST(TrieTest, SearchPrefix_EmptyTrie) {
    Trie trie;

    ArrayList<std::string> results = trie.search("abc");
    EXPECT_EQ(results.size(), 0);
}

// TEST: GIVEN a Trie of 3 words
//       WHEN when searching an empty prefix
//       THEN results should be 3
TEST(TrieTest, SearchPrefix_EmptyPrefix) {
    Trie trie;
    trie.insert("word");
    trie.insert("world");
    trie.insert("work");

    ArrayList<std::string> results = trie.search("");
    EXPECT_EQ(results.size(), 3);
}

// TEST: GIVEN a Trie of 3 words
//       WHEN when searching a prefix that is a word
//       THEN results should be 1
TEST(TrieTest, SearchPrefix_Word) {
    Trie trie;
    trie.insert("word");
    trie.insert("world");
    trie.insert("work");

    ArrayList<std::string> results = trie.search("word");
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0], "word");
}
