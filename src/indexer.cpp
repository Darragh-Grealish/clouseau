#include "indexer.h"
#include "array_list.hpp"
#include "hashmap.hpp"

#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

Indexer::Indexer(const std::string &directory) {
  this->directory = directory;
  this->indexFile = "clouseau.csv";

  for (const auto &entry : std::filesystem::directory_iterator(directory)) {
    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
      files.push_back(entry.path().filename().string());
    }
  }
}

HashMap<std::string, int> Indexer::file_word_count(const std::string &file) {
  std::cout << "Opening file: " << directory + "/" + file << std::endl;
  HashMap<std::string, int> word_count;
  std::ifstream input(directory + "/" + file);
  if (!input.is_open()) {
    throw std::runtime_error("Could not open file");
  }
  std::string line;
  int total_words = 0;
  while (std::getline(input, line)) {
    std::string clean_word;

    for (char &c : line) {
      if (std::isalnum(c, std::locale()) || c == '\'' || c == '-') {
        clean_word += std::tolower(c, std::locale());
      } else {
        if (!clean_word.empty()) {
          std::cout << "Found word: " << clean_word << std::endl;
          word_count[clean_word]++;
          total_words++;
          clean_word.clear();
        }
      }
    }

    clean_word.erase(std::remove(clean_word.begin(), clean_word.end(), ','),
                     clean_word.end());

    if (!clean_word.empty()) {
      std::cout << "Found word: " << clean_word << std::endl;
      word_count[clean_word]++;
      total_words++;
    }
  }
  word_count["__total_words__"] = total_words;
  std::cout << "Total words: " << total_words << std::endl;
  return word_count;
}

void Indexer::index_directory() {
  ArrayList<std::thread> threads;

  auto worker = [this](ArrayList<std::string> files) {
    for (const std::string &file : files) {
      HashMap<std::string, int> word_count = file_word_count(file);
      int total_words = word_count["__total_words__"];
      word_count.erase("__total_words__");

      std::lock_guard<std::mutex> lock(index_mutex);
      for (auto const &pair : word_count) {
        if (index.find(pair.key) == index.end()) {
          Frequency freq;
          freq.total = pair.value;
          FileFrequency file_freq;
          file_freq.file = file;
          file_freq.count = pair.value;
          file_freq.tf = pair.value / (double)total_words;
          freq.files.push_back(file_freq);
          index[pair.key] = freq;
        } else {
          index[pair.key].total += pair.value;
          FileFrequency file_freq;
          file_freq.file = file;
          file_freq.count = pair.value;
          file_freq.tf = pair.value / (double)total_words;
          index[pair.key].files.push_back(file_freq);
        }
      }

      for (auto &pair : index) {
        pair.value.idf = std::log(files.size() / pair.value.files.size());
      }
    }
  };

  int num_threads = std::thread::hardware_concurrency();
  if (num_threads == 0) {
    num_threads = 1;
  }
  std::cout << "Indexing " << files.size() << " files with " << num_threads
            << " threads" << std::endl;

  int files_per_thread = files.size() / num_threads;
  for (int i = 0; i < num_threads; i++) {
    ArrayList<std::string> thread_files;
    for (int j = i * files_per_thread;
         j < (i + 1) * files_per_thread && j < files.size(); j++) {
      thread_files.push_back(files[j]);
    }
    threads.push_back(std::thread(worker, thread_files));
  }

  for (std::thread &thread : threads) {
    thread.join();
  }
}

void Indexer::serialize_index() {
  std::ofstream index_file(directory + "/" + indexFile);

  // NOTE: Valid header
  index_file << "word,idf,total,file1,tf1";
  for (int i = 2; i <= files.size(); i++) {
    index_file << ",file" << i << ",tf" << i;
  }
  index_file << std::endl;

  for (auto const &pair : index) {
    index_file << pair.key << "," << pair.value.idf << "," << pair.value.total;
    for (int i = 0; i < pair.value.files.size(); i++) {
      index_file << "," << pair.value.files[i].file << ","
                 << pair.value.files[i].tf;
    }
    index_file << std::endl;
  }
  index_file.close();

  std::cout << "Index written to " << indexFile << std::endl;
}

HashMap<std::string, Frequency> Indexer::get_index() { return index; }

void Indexer::deserialize_index() {
  std::ifstream index_file(directory + "/" + indexFile);
  if (!index_file.is_open()) {
    throw std::runtime_error("Unable to open index file for reading");
  }

  index.clear(); // Clear existing index

  std::string line;
  std::getline(index_file, line); // Skip header

  while (std::getline(index_file, line)) {
    std::istringstream iss(line);
    std::string word;
    double idf;
    int total;

    // Parse word, idf, and total count
    if (!std::getline(iss, word, ',') || !(iss >> idf) || iss.get() != ',' ||
        !(iss >> total)) {
      throw std::runtime_error("Error parsing index file: word, idf, or total");
    }

    Frequency freq;
    freq.idf = idf;
    freq.total = total;

    // Parse file and tf pairs
    std::string file;
    double tf;
    char comma;
    while (iss >> comma && comma == ',' && std::getline(iss, file, ',') &&
           iss >> tf) {
      FileFrequency file_freq;
      file_freq.file = file;
      file_freq.tf = tf;
      // Calculate count from tf and total words in file
      // Since tf = count/total_words, count = tf * total_words
      int total_words = static_cast<int>(tf > 0 ? (1.0 / tf) : 0);
      file_freq.count = static_cast<int>(tf * total_words);
      freq.files.push_back(file_freq);
    }

    index[word] = freq;
  }

  index_file.close();
  std::cout << "Index loaded from " << indexFile << std::endl;
}
