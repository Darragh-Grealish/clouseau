#include "indexer.h"
#include "array_list.hpp"
#include "hashmap.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/unistd.h>
#include <unistd.h>
#endif

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

  if (this->files.empty()) { // Empty dir?
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
  std::ifstream input(directory + "/" + file,
                      std::ios::binary); // Binary mode for faster reading
  if (!input.is_open()) {
    throw std::runtime_error("Could not open file");
  }

  // Pre-allocate buffers
  const size_t BUFFER_SIZE = 16384; // 16KB buffer
  std::string buffer;
  buffer.reserve(BUFFER_SIZE);
  std::string word;
  word.reserve(100); // Reserve space for reasonably sized words

  int total_words = 0;
  char c;
  bool in_word = false;

  // Read file in chunks
  while (input.get(c)) {
    if (c >= 'A' && c <= 'Z') {
      c += 32; // Fast lowercase conversion for ASCII
    }

    // Check if character is valid for a word
    bool is_valid = (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
                    (c == '\'' || c == '-');

    if (is_valid) {
      if (!in_word) {
        word.clear();
        in_word = true;
      }
      word += c;
    }

    // Word boundary found
    else if (in_word) {
      if (word.length() > 0) {
        // Handle contractions and hyphenated words
        bool valid_word = true;
        if (word.front() == '\'' || word.front() == '-' ||
            word.back() == '\'' || word.back() == '-') {
          valid_word = false;
        }

        // Check for multiple hyphens/apostrophes
        bool prev_special = false;
        for (size_t i = 0; valid_word && i < word.length(); ++i) {
          if (word[i] == '\'' || word[i] == '-') {
            if (prev_special) {
              valid_word = false;
              break;
            }
            prev_special = true;
          } else {
            prev_special = false;
          }
        }

        if (valid_word && !stopwords.contains(word)) {
          word_count[word]++;
          total_words++;
        }
      }
      in_word = false;
    }
  }

  // Process last word if file ends with word character
  if (in_word && !stopwords.contains(word)) {
    word_count[word]++;
    total_words++;
  }

  word_count["__total_words__"] = total_words;
  return word_count;
}

void Indexer::index_selection(const ArrayList<std::string> &files,
                              std::atomic<int> &processed_files,
                              int total_files) {
  HashMap<std::string, Frequency> local_index;

  for (const std::string &file : files) {
    HashMap<std::string, int> word_count = file_word_count(file);
    int total_words = word_count["__total_words__"];
    word_count.erase("__total_words__");

    for (auto const &pair : word_count) {
      FileFrequency file_freq{file, pair.value,
                              pair.value / (double)total_words};

      if (local_index.find(pair.key) == local_index.end()) {
        Frequency freq;
        freq.total = pair.value;
        freq.files.push_back(file_freq);
        local_index[pair.key] = freq;
      } else {
        local_index[pair.key].total += pair.value;
        local_index[pair.key].files.push_back(file_freq);
      }
    }

    // Update progress
    int current = ++processed_files;
    int percentage = (current * 100) / total_files;
    std::cout << "\r" << percentage << "% (" << current << "/" << total_files
              << " files)" << std::flush;
  }

  {
    std::lock_guard<std::mutex> lock(index_mutex);
    for (auto const &pair : local_index) {
      if (index.find(pair.key) == index.end()) {
        index[pair.key] = pair.value;
      } else {
        index[pair.key].total += pair.value.total;
        for (const auto &file_freq : pair.value.files) {
          index[pair.key].files.push_back(file_freq);
        }
      }
    }
  }
}

void Indexer::index_directory() {
  int num_threads = std::thread::hardware_concurrency();
  if (num_threads == 0)
    num_threads = 1;

  ArrayList<std::thread> threads;
  int files_per_thread =
      static_cast<int>(std::ceil(files.size() / (double)num_threads));

  // Atomic counter for progress tracking
  std::atomic<int> processed_files(0);
  int total_files = static_cast<int>(files.size());

  for (int i = 0; i < num_threads; i++) {
    ArrayList<std::string> thread_files;
    int start = i * files_per_thread;
    int end = std::min(start + files_per_thread, (int)files.size());

    if (start >= files.size())
      break;

    for (int j = start; j < end; j++) {
      thread_files.push_back(files[j]);
    }

    threads.push_back(std::thread(&Indexer::index_selection, this, thread_files,
                                  std::ref(processed_files), total_files));
  }

  for (std::thread &thread : threads) {
    thread.join();
  }

  for (auto &pair : index) {
    pair.value.idf = std::log(files.size() / (double)pair.value.files.size());
    if (pair.value.idf == -INFINITY) {
      pair.value.idf = 0;
    }
  }
}

// NOTE: serialize index to file (binary)
void Indexer::serialize_index() {
  std::ofstream index_file(directory + "/" + indexFile, std::ios::binary);
  if (!index_file.is_open()) {
    throw std::runtime_error("Unable to open index file for writing");
  }

  int num_words = static_cast<int>(index.size());
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
    int files_size = static_cast<int>(freq.files.size());
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

  std::cout << std::endl
            << "Index saved to " << directory + "/" + indexFile << std::endl;
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
