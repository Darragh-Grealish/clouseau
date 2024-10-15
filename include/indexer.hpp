#pragma once
#include "trie.hpp"
#include "array_list.hpp"
#include <mutex>
#include <string>
#include <hashmap.hpp>

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
  Trie trie; 
  Indexer(const std::string &directory, const std::string &indexFile);

  // NOTE: Indexes all files in the directory and serializes the index
  void index_directory();

  // NOTE: Serializes the index to a file (clouseau.idx)
  void serialize_index();

  // NOTE: Returns a map of words to their frequency
  HashMap<std::string, int> file_word_count(const std::string &file);

  // NOTE: Returns the index
  HashMap<std::string, Frequency> get_index();

private:
  HashMap<std::string, Frequency> index;
  std::string directory;
  std::string indexFile;
  ArrayList<std::string> files;
  std::mutex index_mutex;
};
