#pragma once

#include "array_list.hpp"
#include "hashmap.hpp"
#include "trie.hpp"

#include <mutex>
#include <string>

struct FileFrequency {
  std::string file;
  int count;
  double tf;
};

 struct Frequency {
  int total;
  double idf;
  ArrayList<FileFrequency> files;
};

class Indexer {
public:
  Indexer(const std::string &directory);

  // NOTE: Indexes all files in the directory and serializes the index
  void index_directory();

  // NOTE: Serializes the index to a file (clouseau.idx)
  void serialize_index();

  // NOTE: Deserializes the index from a file (clouseau.csv)
  void deserialize_index();

  // NOTE: Deserializes the index from a file (clouseau.csv) and populates the trie
  void deserialize_index_trie( Trie &trie);

  // NOTE: Returns a map of words to their frequency
  HashMap<std::string, int> file_word_count(const std::string &file);

  // NOTE: Returns the index
  HashMap<std::string, Frequency> get_index();

  HashMap<std::string, Frequency> index;

private:
  std::string directory;
  std::string indexFile;
  ArrayList<std::string> files;
  std::mutex index_mutex;
};
