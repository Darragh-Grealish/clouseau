#pragma once

#include "hashmap.hpp"
#include "array_list.hpp"
#include <mutex>
#include <string>

struct FileFrequency {
  std::string file;
  int count;
};

struct Frequency {
  int total;
  ArrayList<FileFrequency> files;
};

class Indexer {
public:
  Indexer(const std::string &directory, const std::string &indexFile);

  // NOTE: Indexes all files in the directory and serializes the index
  void index_directory();

  // NOTE: Serializes the index to a file (clouseau.idx)
  void serialize_index();

  // NOTE: Returns a map of words to their frequency
  std::unordered_map<std::string, int> file_word_count(const std::string &file);

  // NOTE: Returns the index
  std::unordered_map<std::string, Frequency> get_index();

private:
  std::unordered_map<std::string, Frequency> index;
  std::string directory;
  std::string indexFile;
  ArrayList<std::string> files;
  std::mutex index_mutex;
};
