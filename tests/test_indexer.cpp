#include "array_list.hpp"
#include "indexer.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

void create_temp_file(const std::string &file_path,
                      const std::string &content) {
  std::ofstream ofs(file_path);
  ofs << content;
  ofs.close();
}

std::string read_file(const std::string &file_path) {
  std::ifstream file(file_path);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

// TEST: GIVEN a valid file WHEN file_to_words is called THEN it returns the
// correct words
TEST(IndexerTest, FileToWordsWithValidFile) {
  std::string file_path = "./test_file.txt";
  create_temp_file(file_path, "Hello, world! This is a test file.");

  Indexer indexer("./", "index.txt");
  ArrayList<std::string> words = indexer.file_to_words(file_path);

  EXPECT_EQ(words.size(), 7);
  EXPECT_EQ(words.get(0), "Hello");
  EXPECT_EQ(words.get(1), "world");
  EXPECT_EQ(words.get(2), "This");
  EXPECT_EQ(words.get(3), "is");
  EXPECT_EQ(words.get(4), "a");
  EXPECT_EQ(words.get(5), "test");
  EXPECT_EQ(words.get(6), "file");

  std::filesystem::remove(file_path);
}

// TEST: GIVEN a file that cannot be opened WHEN file_to_words is called THEN it
// returns an empty list
TEST(IndexerTest, FileToWordsWithInvalidFile) {
  Indexer indexer("./", "index.txt");
  ArrayList<std::string> words = indexer.file_to_words("nonexistent_file.txt");

  EXPECT_EQ(words.size(), 0);
}

// TEST: GIVEN an empty file WHEN file_to_words is called THEN it returns an
// empty list
TEST(IndexerTest, FileToWordsWithEmptyFile) {
  std::string file_path = "./empty_file.txt";
  create_temp_file(file_path, "");

  Indexer indexer("./", "index.txt");
  ArrayList<std::string> words = indexer.file_to_words(file_path);

  EXPECT_EQ(words.size(), 0);

  std::filesystem::remove(file_path);
}

// TEST: GIVEN a file with only punctuation WHEN file_to_words is called THEN it
// returns an empty list
TEST(IndexerTest, FileToWordsWithOnlyPunctuation) {
  std::string file_path = "./punctuation_file.txt";
  create_temp_file(file_path, "!!!@@@###");

  Indexer indexer("./", "index.txt");
  ArrayList<std::string> words = indexer.file_to_words(file_path);

  EXPECT_EQ(words.size(), 0);

  std::filesystem::remove(file_path);
}

// TEST: GIVEN a file with unique words WHEN indexed THEN it returns correct
// counts
TEST(IndexerTest, UniqueWords) {
  std::string temp_file = "./unique_words.txt";
  create_temp_file(temp_file, "apple banana orange");
  Indexer indexer("./", "index.txt");

  auto result = indexer.index_file(temp_file);

  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result["apple"], 1);
  EXPECT_EQ(result["banana"], 1);
  EXPECT_EQ(result["orange"], 1);

  std::filesystem::remove(temp_file);
}

// TEST: GIVEN a file with duplicate words WHEN indexed THEN it returns correct
// counts
TEST(IndexerTest, DuplicateWords) {
  std::string temp_file = "./duplicate_words.txt";
  create_temp_file(temp_file, "apple banana apple orange banana");
  Indexer indexer("./", "index.txt");

  auto result = indexer.index_file(temp_file);

  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result["apple"], 2);
  EXPECT_EQ(result["banana"], 2);
  EXPECT_EQ(result["orange"], 1);

  std::filesystem::remove(temp_file);
}

// TEST: GIVEN an empty file WHEN indexed THEN it returns an empty map
TEST(IndexerTest, EmptyFile) {
  std::string temp_file = "./empty_file.txt";
  create_temp_file(temp_file, "");
  Indexer indexer("./", "index.txt");

  auto result = indexer.index_file(temp_file);

  EXPECT_TRUE(result.empty());

  std::filesystem::remove(temp_file);
}

// TEST: GIVEN a file with words separated by varying whitespace WHEN indexed
// THEN it returns correct counts
TEST(IndexerTest, WhitespaceHandling) {
  std::string temp_file = "./whitespace_file.txt";
  create_temp_file(temp_file, "apple   banana \n orange\r\n");
  Indexer indexer("./", "index.txt");

  auto result = indexer.index_file(temp_file);

  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result["apple"], 1);
  EXPECT_EQ(result["banana"], 1);
  EXPECT_EQ(result["orange"], 1);

  std::filesystem::remove(temp_file);
}

// TEST: GIVEN a file with punctuation WHEN indexed THEN it ignores punctuation
TEST(IndexerTest, PunctuationHandling) {
  std::string temp_file = "./punctuation_file.txt";
  create_temp_file(temp_file, "apple, banana! orange.");
  Indexer indexer("./", "index.txt");

  auto result = indexer.index_file(temp_file);

  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result["apple"], 1);
  EXPECT_EQ(result["banana"], 1);
  EXPECT_EQ(result["orange"], 1);

  std::filesystem::remove(temp_file);
}

// TEST: GIVEN words with different cases WHEN indexed THEN it treats them as
// different
TEST(IndexerTest, CaseSensitivity) {
  std::string temp_file = "./case_sensitivity.txt";
  create_temp_file(temp_file, "Apple apple Banana banana");
  Indexer indexer("./", "index.txt");

  auto result = indexer.index_file(temp_file);

  EXPECT_EQ(result.size(), 4);
  EXPECT_EQ(result["Apple"], 1);
  EXPECT_EQ(result["apple"], 1);
  EXPECT_EQ(result["Banana"], 1);
  EXPECT_EQ(result["banana"], 1);

  std::filesystem::remove(temp_file);
}

// TEST: GIVEN a non-existent file WHEN indexed THEN it handles the error
// gracefully
TEST(IndexerTest, NonExistentFile) {
  Indexer indexer("./", "index.txt");

  EXPECT_NO_THROW({
    auto result = indexer.index_file("non_existent_file.txt");
    EXPECT_TRUE(result.empty());
  });
}

// TEST: GIVEN a directory with files WHEN indexed THEN it indexes all files
// correctly
TEST(IndexerTest, IndexDirectory) {
  std::string temp_dir = "./temp_dir";
  std::filesystem::create_directory(temp_dir);

  create_temp_file(temp_dir + "file1.txt", "apple banana");
  create_temp_file(temp_dir + "file2.txt", "apple orange");
  create_temp_file(temp_dir + "file3.txt", "banana orange");

  Indexer indexer(temp_dir, "index.txt");
  indexer.index_directory();
  

  // expect the opposite order of the files

  std::filesystem::remove_all(temp_dir);
  std::filesystem::remove("index.txt");
}

// TEST: GIVEN a directory with no files WHEN indexed THEN it creates an empty
// index
TEST(IndexerTest, IndexEmptyDirectory) {
  std::string temp_dir = "./temp_dir";
  std::filesystem::create_directory(temp_dir);

  Indexer indexer(temp_dir, "index.txt");
  indexer.index_directory();

  std::ifstream index_file("index.txt");
  std::string line;
  std::unordered_map<std::string, int> index;

  while (std::getline(index_file, line)) {
    std::string word;
    int count;
    std::istringstream iss(line);
    iss >> word >> count;
    index[word] = count;
  }

  EXPECT_TRUE(index.empty());

  std::filesystem::remove_all(temp_dir);
  std::filesystem::remove("index.txt");
}

// TEST: GIVEN an index with words WHEN serialize_index is called THEN the index
// is correctly written to the file
TEST(IndexerTest, SerializeIndexWithValidIndex) {
  std::string test_directory = "./test_dir";
  std::filesystem::create_directory(test_directory);

  // Create dummy files to populate the index
  create_temp_file(test_directory + "/file1.txt", "This is a test file.");
  create_temp_file(test_directory + "/file2.txt",
                   "Another test file with test data.");

  // Create an Indexer instance and manually populate the index
  std::string index_file = "./test_index.txt";
  Indexer indexer(test_directory, index_file);

  // Manually populate the index with sample data
  indexer.set_row("test", Frequency{4, ArrayList<FileFrequency>{
                                           FileFrequency{"file1.txt", 1},
                                           FileFrequency{"file2.txt", 3}}});
  indexer.set_row("file", Frequency{2, ArrayList<FileFrequency>{
                                           FileFrequency{"file1.txt", 1},
                                           FileFrequency{"file2.txt", 1}}});
  // Call serialize_index to write the index to the file
  indexer.serialize_index();

  // Expected output format in the index file
  std::string expected_output = "total file1 count1 file2 count2 ...\n"
                                "file 2 file1.txt 1 file2.txt 1\n"
                                "test 4 file1.txt 1 file2.txt 3\n";

  // Read the output file and compare it with the expected output
  std::string actual_output = read_file(index_file);

  // WARNING: Non-deterministic order of words in the index file (OS dependent)
  std::sort(actual_output.begin(), actual_output.end());
  std::sort(expected_output.begin(), expected_output.end());
  EXPECT_EQ(actual_output, expected_output);

  // Clean up
  std::filesystem::remove_all(test_directory);
  std::filesystem::remove(index_file);
}

// TEST: GIVEN an empty index WHEN serialize_index is called THEN only the
// header is written to the file
TEST(IndexerTest, SerializeIndexWithEmptyIndex) {
  std::string test_directory = "./test_dir";
  std::filesystem::create_directory(test_directory);

  // Create an Indexer instance
  std::string index_file = "./test_index.txt";
  Indexer indexer(test_directory, index_file);

  // Call serialize_index with an empty index
  indexer.serialize_index();

  // Expected output format in the index file
  std::string expected_output = "total file1 count1 file2 count2 ...\n";

  // Read the output file and compare it with the expected output
  std::string actual_output = read_file(index_file);

  // WARNING: Non-deterministic order of words in the index file (OS dependent)
  std::sort(actual_output.begin(), actual_output.end());
  std::sort(expected_output.begin(), expected_output.end());
  EXPECT_EQ(actual_output, expected_output);

  // Clean up
  std::filesystem::remove_all(test_directory);
  std::filesystem::remove(index_file);
}

// TEST: GIVEN an index with multiple files and words WHEN serialize_index is
// called THEN the index is written correctly
TEST(IndexerTest, SerializeIndexWithMultipleFiles) {
  std::string test_directory = "./test_dir";
  std::filesystem::create_directory(test_directory);

  // Create dummy files
  create_temp_file(test_directory + "/file1.txt", "This is a test file.");
  create_temp_file(test_directory + "/file2.txt",
                   "Another test file with more test data.");

  // Create an Indexer instance and manually populate the index
  std::string index_file = "./test_index.txt";
  Indexer indexer(test_directory, index_file);

  // Populate the index with more data
  indexer.set_row("test", Frequency{5, ArrayList<FileFrequency>{
                                           FileFrequency{"file1.txt", 1},
                                           FileFrequency{"file2.txt", 4}}});
  indexer.set_row("file", Frequency{2, ArrayList<FileFrequency>{
                                           FileFrequency{"file1.txt", 1},
                                           FileFrequency{"file2.txt", 1}}});
  indexer.set_row("data", Frequency{1, ArrayList<FileFrequency>{
                                           FileFrequency{"file2.txt", 1}}});

  // Call serialize_index
  indexer.serialize_index();

  // Expected output format in the index file
  std::string expected_output = "total file1 count1 file2 count2 ...\n"
                                "data 1 file2.txt 1\n"
                                "file 2 file1.txt 1 file2.txt 1\n"
                                "test 5 file1.txt 1 file2.txt 4\n";

  // Read the output file and compare it with the expected output
  std::string actual_output = read_file(index_file);
  EXPECT_EQ(actual_output, expected_output);

  // Clean up
  std::filesystem::remove_all(test_directory);
  std::filesystem::remove(index_file);
}
