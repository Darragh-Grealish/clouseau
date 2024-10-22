#pragma once

#include "array_list.hpp"
#include "hashmap.hpp"
#include "set.hpp"
#include "trie.hpp"

#include <atomic>
#include <mutex>
#include <string>

struct FileFrequency {
  std::string file;
  int count;
  double tf;
};

// NOTE: Associated with a word
struct Frequency {
  int total;
  double idf;
  ArrayList<FileFrequency> files;
};

class Indexer {
public:
  Indexer(const std::string &directory);

  void index_directory();
  void serialize_index();
  void deserialize_index();

  // INFO: Build Trie from index during deserialization
  void deserialize_index(Trie &trie);

  HashMap<std::string, Frequency> index;

private:
  std::string directory;
  std::string indexFile;
  ArrayList<std::string> files;
  std::mutex index_mutex;

  // INFO: Ignore counting these
  const Set<std::string> stopwords = {"the", "and", "is",   "in",   "it",  "of",
                                      "to",  "a",   "that", "with", "for", "on",
                                      "as",  "by",  "at",   "an",   "be", "or", "not"};

  // INFO: Walk through the directory and get all files
  ArrayList<std::string> get_directory_files();

  // INFO: [THREAD WORKER] Index chunk of files
  void index_selection(const ArrayList<std::string> &files,
                       std::atomic<int> &processed_files, int total_files);

  // INFO: Count words in a file
  HashMap<std::string, int> file_word_count(const std::string &file);
};
