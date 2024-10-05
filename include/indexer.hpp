#pragma once

#include "array_list.hpp"
#include <mutex>
#include <string>
#include <unordered_map>

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
  Indexer(std::string directory, std::string indexFile);

  // NOTE: Reads a file, returns the words within
  ArrayList<std::string> file_to_words(std::string file_path);

  // NOTE: Returns a map of words to their frequency
  std::unordered_map<std::string, int> index_file(std::string file);

  // NOTE: Indexes all files in the directory and serializes the index
  void index_directory();

  // TODO: Autocomplete feature: returns a list of possible word completions
  // ArrayList<std::string> autocomplete(std::string &prefix);

  // TODO: Search function: returns a list of files where the word occurs
  // ArrayList<std::string> search(std::string &word);

  // NOTE: Serializes the index to a file (clouseau.idx)
  void serialize_index();

  // NOTE: Set index value
  void set_row(std::string word, Frequency freq);

private:
  // NOTE: Word -> Frequency pairs
  std::unordered_map<std::string, Frequency> index;
  std::string directory;
  std::string indexFile;
  ArrayList<std::string> files;

  std::mutex index_mutex;
  // TODO: Implement a trie for autocomplete
  // Trie trie;
};
