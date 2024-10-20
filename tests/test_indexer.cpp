#include "indexer.h"
#include "hashmap.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

// NOTE: Helper function to create a temporary file
void create_temp_file(const std::string &dir, const std::string &filename, const std::string &content) {
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


// TEST: GIVEN a directory with multiple files WHEN serialize_index is called
// THEN the index should be serialized correctly to a file.
TEST(IndexerTest, SerializeIndex_WritesToFile) {
  std::string temp_dir = "./test_data";
  std::filesystem::create_directory(temp_dir);
  create_temp_file(temp_dir, "file1.txt", "word1 word2");
  create_temp_file(temp_dir, "file2.txt", "word2 word3");

  Indexer indexer(temp_dir);
  indexer.index_directory();
  indexer.serialize_index();

  EXPECT_TRUE(std::filesystem::exists(temp_dir + "/clouseau.csv"));

  std::ifstream index_file(temp_dir + "/clouseau.csv");
  std::string line;
  std::getline(index_file, line); // Skip header

  HashMap<std::string, int> expected_totals; 
  expected_totals["word1"] = 1;
  expected_totals["word2"] = 2;
  expected_totals["word3"] = 1;

  while (std::getline(index_file, line)) {
    std::istringstream iss(line);
    std::string word;
    int total;
    iss >> word >> total;
    EXPECT_EQ(total, expected_totals[word]);
  }

  index_file.close();
  std::filesystem::remove(temp_dir + "/clouseau.csv");
  std::filesystem::remove_all(temp_dir);
}

