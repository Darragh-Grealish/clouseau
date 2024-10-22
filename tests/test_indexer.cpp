
#include "indexer.h"
#include "trie.hpp"
#include "gtest/gtest.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

// NOTE: Helper function to create a temporary file
void create_temp_file(const std::string &dir, const std::string &filename,
                      const std::string &content) {
  std::ofstream file(dir + "/" + filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not create temporary file");
  }
  file << content;
  file.close();
}

// TEST: GIVEN a directory with files WHEN create_temp_file is called THEN it
// should create the files.
TEST(IndexerTest, CreateTempFile) {
  std::string temp_dir = "./test_data";
  std::filesystem::create_directory(temp_dir);
  create_temp_file(temp_dir, "file1.txt", "word1 word2 word3");
  create_temp_file(temp_dir, "file2.txt", "word2 word3 word4");

  EXPECT_TRUE(std::filesystem::exists(temp_dir + "/file1.txt"));
  EXPECT_TRUE(std::filesystem::exists(temp_dir + "/file2.txt"));

  std::filesystem::remove_all(temp_dir);
}

// TEST: GIVEN a directory with files WHEN file_word_count is called THEN we get
// the correct word count
TEST(IndexerTest, FileWordCount) {
  std::string temp_dir = "./test_data";
  std::filesystem::create_directory(temp_dir);
  create_temp_file(temp_dir, "file1.txt", "hello world hello");

  Indexer indexer(temp_dir);
  HashMap<std::string, int> word_count = indexer.file_word_count(std::filesystem::path("file1.txt").filename().string());

  EXPECT_EQ(word_count["hello"], 2);
  EXPECT_EQ(word_count["world"], 1);
}
