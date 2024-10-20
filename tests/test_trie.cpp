// #include "trie.hpp"
// #include "indexer.h"
// #include <gtest/gtest.h>
// #include <filesystem>
// #include <fstream>

// // Helper function to create a temporary file (reuse from indexer test)
// void create_temp_file(const std::string &directory,
//                       const std::string &file_name,
//                       const std::string &content) {
//   std::ofstream file(directory + "/" + file_name);
//   file << content;
//   file.close();
// }

// // TEST: GIVEN a Trie WHEN insert is called THEN words should be inserted correctly.
// TEST(TrieTest, InsertWords) {
//   Trie trie;

//   trie.insert("apple", "file1.txt");
//   trie.insert("app", "file2.txt");

//   // Check if "apple" and "app" are inserted correctly
//   ArrayList<std::string> result1 = trie.search("apple");
//   ArrayList<std::string> result2 = trie.search("app");

//   ASSERT_EQ(result1.size(), 1);
//   EXPECT_EQ(result1[0], "file1.txt");

//   ASSERT_EQ(result2.size(), 1);
//   EXPECT_EQ(result2[0], "file2.txt");
// }

// // TEST: GIVEN a Trie with inserted words WHEN search is called THEN it should return the correct files.
// TEST(TrieTest, SearchWords) {
//   Trie trie;

//   trie.insert("apple", "file1.txt");
//   trie.insert("app", "file2.txt");
//   trie.insert("banana", "file3.txt");

//   // Check search results for different prefixes
//   ArrayList<std::string> result1 = trie.search("app");
//   ArrayList<std::string> result2 = trie.search("banana");
//   ArrayList<std::string> result3 = trie.search("orange");

//   ASSERT_EQ(result1.size(), 1);
//   EXPECT_EQ(result1[0], "file2.txt");

//   ASSERT_EQ(result2.size(), 1);
//   EXPECT_EQ(result2[0], "file3.txt");

//   // Search for a word that doesn't exist
//   EXPECT_EQ(result3.size(), 0);
// }

// // TEST: GIVEN an index loaded into the Trie WHEN search is called THEN it should return the correct files.
// // TEST(TrieTest, LoadIndex) {
// //     // Create a temporary directory and files for testing
// //     std::string temp_dir = "./test_data2";
// //     std::filesystem::create_directory(temp_dir);
// //     create_temp_file(temp_dir, "file1.txt", "apple banana");
// //     create_temp_file(temp_dir, "file2.txt", "apple orange");

// //     // Create and index the files using Indexer
// //     Indexer indexer(temp_dir);
// //     indexer.index_directory();
// //     std::unordered_map<std::string, Frequency> index = indexer.get_index();

// //     // Debug: Print the index content
// //     for (const auto &pair : index) {
// //         std::cout << pair.first << ": ";
// //         for (const FileFrequency &fileFreq : pair.second.files) {
// //             std::cout << fileFreq.file << " ";
// //         }
// //         std::cout << std::endl;
// //     }

// //     // Create Trie and load the index
// //     Trie trie;
// //     trie.load_index(index);

// //     // Check if words are correctly loaded into the Trie
// //     ArrayList<std::string> result1 = trie.search("apple");
// //     ArrayList<std::string> result2 = trie.search("banana");
// //     ArrayList<std::string> result3 = trie.search("orange");

// //     // Verify "apple" is associated with two files
// //     ASSERT_EQ(result1.size(), 2);
// //     EXPECT_TRUE(std::find(result1.begin(), result1.end(), "file1.txt") != result1.end());
// //     EXPECT_TRUE(std::find(result1.begin(), result1.end(), "file2.txt") != result1.end());

// //     // Verify "banana" is associated with one file
// //     ASSERT_EQ(result2.size(), 1);
// //     EXPECT_EQ(result2[0], "file1.txt");

// //     // Verify "orange" is associated with one file
// //     ASSERT_EQ(result3.size(), 1);
// //     EXPECT_EQ(result3[0], "file2.txt");

// //     // Clean up
// //     std::filesystem::remove_all(temp_dir);
// // }


// // TEST: GIVEN a Trie with no matching prefix WHEN search is called THEN it should return an empty result.
// TEST(TrieTest, SearchNoMatch) {
//   Trie trie;

//   trie.insert("apple", "file1.txt");
//   trie.insert("banana", "file2.txt");

//   // Search for a prefix that doesn't exist
//   ArrayList<std::string> result = trie.search("orange");

//   EXPECT_EQ(result.size(), 0);
// }

// // TEST: GIVEN an empty Trie WHEN search is called THEN it should return an empty result.
// TEST(TrieTest, SearchEmptyTrie) {
//   Trie trie;

//   // Search on an empty Trie
//   ArrayList<std::string> result = trie.search("apple");

//   EXPECT_EQ(result.size(), 0);
// }