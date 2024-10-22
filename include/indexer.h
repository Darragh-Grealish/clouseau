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

// NOTE: index[word] = below
struct Frequency {
  int total;
  double idf;
  ArrayList<FileFrequency> files;
};

class Indexer {
public:
  Indexer(
      const std::string &directory); // contains checks for dir compatibility

  void index_directory();   // Index all files in the directory
  void serialize_index();   // Serialize index to file (binary)
  void deserialize_index(); // Deserialize index to HashMap structure for search
  void deserialize_index(
      Trie &trie); // Deserialize index to Trie structure for autocomplete
  HashMap<std::string, int>
  file_word_count(const std::string &file); // Count words in a file

  HashMap<std::string, Frequency> index;

private:
  std::string directory;
  std::string indexFile;
  ArrayList<std::string> files;
  std::mutex index_mutex;
  const Set<std::string> stopwords = {"the", "and", "is",   "in",   "it",  "of",
                                      "to",  "a",   "that", "with", "for", "on",
                                      "as",  "by",  "at",   "an",   "be"};

  ArrayList<std::string>
  get_directory_files(); // Walk through directory and get all files
  // Thread worker function to index a selection of files
  void index_selection(const ArrayList<std::string>& files, std::atomic<int>& processed_files, int total_files); 

};
