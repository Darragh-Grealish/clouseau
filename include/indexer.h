#pragma once

#include "array_list.hpp"
#include "hashmap.hpp"
#include "trie.hpp"
#include "set.hpp"

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
  void deserialize_index( Trie &trie);
  HashMap<std::string, int> file_word_count(const std::string &file);

  HashMap<std::string, Frequency> index;

private:
  std::string directory;
  std::string indexFile;
  ArrayList<std::string> files;
  std::mutex index_mutex;
  const Set<std::string> stopwords = {"the", "and", "is",   "in",   "it",  "of",
                                      "to",  "a",   "that", "with", "for", "on",
                                      "as",  "by",  "at",   "an",   "be"};

  ArrayList<std::string> get_directory_files();
};
