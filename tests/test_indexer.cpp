#include "indexer.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

// NOTE: Helper function to create a temporary file
void create_temp_file(const std::string &directory,
                      const std::string &file_name,
                      const std::string &content) {
  std::ofstream file(directory + "/" + file_name);
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

// TEST: GIVEN a file with words WHEN file_word_count is called THEN it should
// return the correct word frequency.
TEST(IndexerTest, FileWordCount_CountsWords) {
  std::string temp_dir = "./test_data";
  std::filesystem::create_directory(temp_dir);
  create_temp_file(temp_dir, "file1.txt", "word1 word2 word3 word1 word2");

  Indexer indexer(temp_dir);
  HashMap<std::string, int> word_count = indexer.file_word_count("file1.txt");

  EXPECT_EQ(word_count["word1"], 2);
  EXPECT_EQ(word_count["word2"], 2);
  EXPECT_EQ(word_count["word3"], 1);

  // Clean up
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

  // Check if the index file was created
  EXPECT_TRUE(std::filesystem::exists(temp_dir + "/clouseau.csv"));

  // Read the file and check contents
  std::ifstream index_file(temp_dir + "/clouseau.csv");
  std::string line;
  std::getline(index_file, line); // Skip header

  HashMap<std::string, int> expected_totals;
  expected_totals.insert("word1", 1);
  expected_totals.insert("word2", 2);
  expected_totals.insert("word3", 1);

  while (std::getline(index_file, line)) {
    std::istringstream iss(line);
    std::string word;
    int total;
    iss >> word >> total;
    EXPECT_EQ(total, expected_totals[word]);
  }

  // Clean up
  index_file.close();
  std::filesystem::remove(temp_dir + "/clouseau.csv");
  std::filesystem::remove_all(temp_dir);
}

// TEST: GIVEN a directory with an index file WHEN deserialize_index is called
// THEN the index should be deserialized correctly.
TEST(IndexerTest, DeserializeIndex_ReadsFromFile) {
  std::string temp_dir = "./test_data";
  std::filesystem::create_directory(temp_dir);
  create_temp_file(temp_dir, "file1.txt", "word1 word2");
  create_temp_file(temp_dir, "file2.txt", "word2 word3");

  Indexer indexer(temp_dir);
  indexer.index_directory();
  indexer.serialize_index();
  auto idx1 = indexer.get_index();

  Indexer new_indexer(temp_dir);
  new_indexer.deserialize_index();
  // HashMap<std::string, Frequency> idx2 = new_indexer.get_index();
  unordered_map<std::string, Frequency> idx2 = new_indexer.get_index();

  EXPECT_EQ(idx1.size(), idx2.size());
  EXPECT_EQ(idx1["word1"].total, idx2["word1"].total);
  EXPECT_EQ(idx1["word2"].total, idx2["word2"].total);
  EXPECT_EQ(idx1["word3"].total, idx2["word3"].total);
  EXPECT_EQ(idx1["word1"].idf, idx2["word1"].idf);
  EXPECT_EQ(idx1["word2"].idf, idx2["word2"].idf);
  EXPECT_EQ(idx1["word3"].idf, idx2["word3"].idf);
}

