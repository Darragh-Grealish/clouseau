#pragma once

#include "array_list.hpp"
#include "hashmap.hpp"

#include <mutex>
#include <string>

struct FileFrequency {
  std::string file;
  int count;
  double tf;
};

// NOTE: index[word] = below
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
  HashMap<std::string, int> file_word_count(const std::string &file);

  HashMap<std::string, Frequency> index;

private:
  std::string directory;
  std::string indexFile;
  ArrayList<std::string> files;
  std::mutex index_mutex;
};
