#include "hashmap.hpp"
#include "indexer.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

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

// TEST: GIVEN a directory with files WHEN get_directory_files is called THEN it
// indexes correctly.
TEST(IndexerTest, GetDirectoryFiles) {
  std::string temp_dir = "./test_data";
  std::filesystem::create_directory(temp_dir);
  create_temp_file(temp_dir, "file1.txt", "i am a test file");
  create_temp_file(temp_dir, "file2.txt", "i am another test file");

  Indexer indexer(temp_dir);
  indexer.index_directory();
  EXPECT_EQ(indexer.index["i"].total, 2);
  EXPECT_EQ(indexer.index["am"].total, 2);
  EXPECT_EQ(indexer.index["test"].total, 2);
  EXPECT_EQ(indexer.index["file"].total, 2);

  std::filesystem::remove_all(temp_dir);
}

// TEST: GIVEN a directory with indexed files WHEN serialize_index is called
// THEN it should serialize the index.
TEST(IndexerTest, SerializeIndex) {
  std::string temp_dir = "./test_data";
  std::filesystem::create_directory(temp_dir);
  create_temp_file(temp_dir, "file1.txt", "i am a test file");
  create_temp_file(temp_dir, "file2.txt", "i am another test file");

  Indexer indexer(temp_dir);
  indexer.index_directory();
  indexer.serialize_index();

  EXPECT_TRUE(std::filesystem::exists(temp_dir + "/clouseau.idx"));

  std::filesystem::remove_all(temp_dir);
}

// TEST: GIVEN a directory with indexed files WHEN deserialize_index is called
// THEN it should deserialize the index.
TEST(IndexerTest, DeserializeIndex) {
  std::string temp_dir = "./test_data";
  std::filesystem::create_directory(temp_dir);
  create_temp_file(temp_dir, "file1.txt", "i am a test file");
  create_temp_file(temp_dir, "file2.txt", "i am another test file");

  Indexer indexer(temp_dir);
  indexer.index_directory();
  indexer.serialize_index();

  Indexer new_indexer(temp_dir);
  new_indexer.deserialize_index();

  EXPECT_EQ(indexer.index.size(), new_indexer.index.size());

  std::filesystem::remove_all(temp_dir);
}
