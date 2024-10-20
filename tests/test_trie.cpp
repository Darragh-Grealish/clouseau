#include "trie.hpp"
#include "indexer.h"
#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

// TEST: GIVEN an empty Trie WHEN inserting a word THEN it should contain that word.
TEST(TrieTest, Insert_AddsWord) {
    Trie trie;
    trie.insert("test", "file1.txt");
    ArrayList<std::string> results = trie.search("test");
    
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0], "test");
}
// TEST: GIVEN a Trie with words WHEN searching for a prefix THEN it should return matching words.
TEST(TrieTest, Search_ReturnsWordsWithPrefix) {
    Trie trie;
    trie.insert("apple", "file1.txt");
    trie.insert("app", "file2.txt");
    trie.insert("banana", "file1.txt");
    ArrayList<std::string> results = trie.search("app");
    EXPECT_EQ(results.size(), 2);
    EXPECT_TRUE(std::find(results.begin(), results.end(), "app") != results.end());
    EXPECT_TRUE(std::find(results.begin(), results.end(), "apple") != results.end());
}
// TEST: GIVEN a Trie with no words WHEN searching for a prefix THEN it should return an empty list.
TEST(TrieTest, Search_NoWordsReturnsEmpty) {
    Trie trie;
    ArrayList<std::string> results = trie.search("nonexistent");
    EXPECT_TRUE(results.empty());
}
// TEST: GIVEN a Trie with multiple files WHEN inserting the same word with different files THEN it should store all files.
TEST(TrieTest, Insert_SameWordMultipleFiles) {
    Trie trie;
    trie.insert("example", "file1.txt");
    trie.insert("example", "file2.txt");
    
    ArrayList<std::string> results = trie.search("example");
    EXPECT_EQ(results.size(), 1); 
    EXPECT_TRUE(std::find(results.begin(), results.end(), "example") != results.end());
}
// TEST: GIVEN a Trie with words WHEN searching for a non-existing prefix THEN it should return an empty list.
TEST(TrieTest, Search_NonExistingPrefixReturnsEmpty) {
    Trie trie;
    trie.insert("cat", "file1.txt");
    trie.insert("dog", "file2.txt");
    ArrayList<std::string> results = trie.search("fish");
    EXPECT_TRUE(results.empty());
}
// Helper function to create a temporary directory and test files
void setup_test_files(const std::string& directory) {
    std::filesystem::create_directory(directory); 
    std::ofstream file1(directory + "/file1.txt");
    file1 << "apple banana apple cherry";
    file1.close();
    std::ofstream file2(directory + "/file2.txt");
    file2 << "banana apple orange";
    file2.close();
}

void cleanup_test_files(const std::string& directory) {
    std::filesystem::remove_all(directory); 
}
// TEST: GIVEN a Trie WHEN loading an index with words THEN the Trie should contain those words.
// TEST(TrieTest, LoadIndex_InsertsWordsFromIndexer) {
//     Indexer indexer("./archive/clouseau.csv"); 
//     indexer.deserialize_index(); 
//     Trie trie; 
//     trie.load_index(indexer); 
//     ArrayList<std::string> results_apple = trie.search("apple");
//     EXPECT_GT(results_apple.size(), 0); 
//     ArrayList<std::string> results_banana = trie.search("banana");
//     EXPECT_GT(results_banana.size(), 0); 
//     ArrayList<std::string> results_cherry = trie.search("cherry");
//     EXPECT_GT(results_cherry.size(), 0);
// }

// TEST: GIVEN a Trie with words WHEN finding words THEN it should return all complete words.
TEST(TrieTest, FindWords_ReturnsAllWords) {
    Trie trie;
    trie.insert("bat", "file1.txt");
    trie.insert("batman", "file2.txt");
    trie.insert("batter", "file1.txt");
    ArrayList<std::string> results;
    trie.find_words(trie.root, "", results); 
    EXPECT_EQ(results.size(), 3); 
    EXPECT_TRUE(std::find(results.begin(), results.end(), "bat") != results.end());
    EXPECT_TRUE(std::find(results.begin(), results.end(), "batman") != results.end());
    EXPECT_TRUE(std::find(results.begin(), results.end(), "batter") != results.end());
}