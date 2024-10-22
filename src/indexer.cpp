#include "indexer.h"
#include "array_list.hpp"
#include "hashmap.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <regex>
#include <stdexcept>
#include <string>
#include <sys/unistd.h>
#include <thread>
#include <unistd.h>

Indexer::Indexer(const std::string &directory) {
  this->directory = directory;
  this->indexFile = "clouseau.idx";

  if (!std::filesystem::exists(directory)) { // Exists?
    throw std::runtime_error("Directory does not exist");
  }

  if (!std::filesystem::is_directory(directory)) { // A dir?
    throw std::runtime_error("Path is not a directory");
  }

  if (access(directory.c_str(), W_OK) == -1) { // Writeable?
    throw std::runtime_error("Directory is not writable");
  }

  this->files = get_directory_files();

  if(this->files.empty()) { // Empty dir?
    throw std::runtime_error("No .txt files found in directory");
  }
}

// NOTE: Find all txts in the directory
ArrayList<std::string> Indexer::get_directory_files() {
  ArrayList<std::string> files;
  for (const auto &entry : std::filesystem::directory_iterator(directory)) {
    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
      files.push_back(entry.path().filename().string());
    }
  }
  return files;
}

// NOTE: Do word count for one file
HashMap<std::string, int> Indexer::file_word_count(const std::string &file) {
  HashMap<std::string, int> word_count;
  std::ifstream input(directory + "/" + file);
  if (!input.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  std::string line;
  int total_words = 0;

  std::locale loc;
  std::regex word_regex(
      R"([a-zA-Z0-9]+(?:['-][a-zA-Z0-9]+)*)"); // capture words including
                                               // apostrophes/hyphens

  while (std::getline(input, line)) {
    std::transform(line.begin(), line.end(), line.begin(),
                   [&](char c) { return std::tolower(c, loc); });

    auto words_begin =
        std::sregex_iterator(line.begin(), line.end(), word_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
      std::string word = (*i).str();

      // Ignore stopwords
      if (!stopwords.contains(word)) {
        word_count[word]++;
        total_words++;
      }
    }
  }

  word_count["__total_words__"] = total_words;
  return word_count;
}

// INFO: indexes a list of files (thread worker)
void Indexer::index_selection(const ArrayList<std::string> &files) {
  for (const std::string &file : files) {
    HashMap<std::string, int> word_count = file_word_count(file);
    int total_words = word_count["__total_words__"];
    word_count.erase("__total_words__");

    std::lock_guard<std::mutex> lock(index_mutex);
    for (auto const &pair : word_count) {
      if (index.find(pair.key) == index.end()) {
        Frequency freq;
        freq.total = pair.value;
        FileFrequency file_freq{file, pair.value,
                                pair.value / (double)total_words};
        freq.files.push_back(file_freq);
        index[pair.key] = freq;
      } else {
        index[pair.key].total += pair.value;
        FileFrequency file_freq{file, pair.value,
                                pair.value / (double)total_words};
        index[pair.key].files.push_back(file_freq);
      }
    }

    for (auto &pair : index) {
      pair.value.idf = std::log(files.size() / pair.value.files.size());
      if (pair.value.idf == -INFINITY) {
        pair.value.idf = 0;
      }
    }
  }
}

// INFO: indexes all files in the directory
void Indexer::index_directory() {
  ArrayList<std::thread> threads;
  int num_threads = std::thread::hardware_concurrency();
  if (num_threads == 0)
    num_threads = 1;

  int files_per_thread = files.size() / num_threads;
  std::cout << "Indexing " << files_per_thread << " files per thread ("
            << num_threads << " threads)" << std::endl;

  for (int i = 0; i < num_threads; i++) {
    ArrayList<std::string> thread_files;
    for (int j = i * files_per_thread;
         j < (i + 1) * files_per_thread && j < files.size(); j++) {
      thread_files.push_back(files[j]);
    }
    threads.push_back(
        std::thread(&Indexer::index_selection, this, thread_files));
  }

  for (std::thread &thread : threads) {
    thread.join();
  }
}

// NOTE: serialize index to file (binary)
void Indexer::serialize_index() {
  std::ofstream index_file(directory + "/" + indexFile, std::ios::binary);
  if (!index_file.is_open()) {
    throw std::runtime_error("Unable to open index file for writing");
  }

  int num_words = index.size();
  index_file.write(reinterpret_cast<char *>(&num_words), sizeof(int));

  for (auto const &pair : index) {
    // Write the word
    int word_length = pair.key.length();
    index_file.write(reinterpret_cast<char *>(&word_length), sizeof(int));
    index_file.write(pair.key.c_str(), word_length);

    // Write the frequency
    Frequency const &freq = pair.value;
    index_file.write(reinterpret_cast<const char *>(&freq.idf), sizeof(double));
    index_file.write(reinterpret_cast<const char *>(&freq.total), sizeof(int));

    // Write the files array
    int files_size = freq.files.size();
    index_file.write(reinterpret_cast<char *>(&files_size), sizeof(int));
    for (auto const &file_freq : freq.files) {
      // Write the file frequency
      int file_length = file_freq.file.length();
      index_file.write(reinterpret_cast<char *>(&file_length), sizeof(int));
      index_file.write(file_freq.file.c_str(), file_length);
      index_file.write(reinterpret_cast<const char *>(&file_freq.tf),
                       sizeof(double));
      index_file.write(reinterpret_cast<const char *>(&file_freq.count),
                       sizeof(int));
    }
  }

  std::cout << "Index saved to " << directory + "/" + indexFile << std::endl;
}

void Indexer::deserialize_index() {
  std::ifstream index_file(directory + "/" + indexFile, std::ios::binary);
  if (!index_file.is_open()) {
    throw std::runtime_error("Unable to open index file for reading");
  }

  index.clear(); // Clear existing index

  int num_words;
  index_file.read(reinterpret_cast<char *>(&num_words), sizeof(int));

  for (int i = 0; i < num_words; i++) {
    // Read the word
    int word_length;
    index_file.read(reinterpret_cast<char *>(&word_length), sizeof(int));
    std::string word;
    word.resize(word_length);
    index_file.read(&word[0], word_length);

    // Read the frequency
    Frequency freq;
    index_file.read(reinterpret_cast<char *>(&freq.idf), sizeof(double));
    index_file.read(reinterpret_cast<char *>(&freq.total), sizeof(int));

    // Read the files array
    int files_size;
    index_file.read(reinterpret_cast<char *>(&files_size), sizeof(int));
    for (int j = 0; j < files_size; j++) {
      FileFrequency file_freq;
      // Read the file frequency
      int file_length;
      index_file.read(reinterpret_cast<char *>(&file_length), sizeof(int));
      file_freq.file.resize(file_length);
      index_file.read(&file_freq.file[0], file_length);
      index_file.read(reinterpret_cast<char *>(&file_freq.tf), sizeof(double));
      index_file.read(reinterpret_cast<char *>(&file_freq.count), sizeof(int));
      freq.files.push_back(file_freq);
    }

    index[word] = freq;
  }

  index_file.close();
}

// INFO: deserialize index to Trie structure for autocomplete
void Indexer::deserialize_index(Trie &trie) {
  std::ifstream index_file(directory + "/" + indexFile, std::ios::binary);
  if (!index_file.is_open()) {
    throw std::runtime_error("Unable to open index file for reading");
  }

  index.clear(); // Clear existing index
  int num_words;
  index_file.read(reinterpret_cast<char *>(&num_words), sizeof(int));

  for (int i = 0; i < num_words; i++) {
    // Read the word
    int word_length;
    index_file.read(reinterpret_cast<char *>(&word_length), sizeof(int));
    std::string word;
    word.resize(word_length);
    index_file.read(&word[0], word_length);

    // Read the frequency
    Frequency freq;
    index_file.read(reinterpret_cast<char *>(&freq.idf), sizeof(double));
    index_file.read(reinterpret_cast<char *>(&freq.total), sizeof(int));

    // Read the files array
    int files_size;
    index_file.read(reinterpret_cast<char *>(&files_size), sizeof(int));
    for (int j = 0; j < files_size; j++) {
      FileFrequency file_freq;
      // Read the file frequency
      int file_length;
      index_file.read(reinterpret_cast<char *>(&file_length), sizeof(int));
      file_freq.file.resize(file_length);
      index_file.read(&file_freq.file[0], file_length);
      index_file.read(reinterpret_cast<char *>(&file_freq.tf), sizeof(double));
      index_file.read(reinterpret_cast<char *>(&file_freq.count), sizeof(int));
      freq.files.push_back(file_freq);
    }

    index[word] = freq;
    trie.insert(word); // Insert word into trie
  }

  index_file.close();
}
