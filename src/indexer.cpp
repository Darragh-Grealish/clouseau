#include "indexer.hpp"
#include "array_list.hpp"
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <thread>
#include <unordered_map>

Indexer::Indexer(const std::string &directory, const std::string &indexFile) {
  this->directory = directory;
  this->indexFile = indexFile;

  for (const auto &entry : std::filesystem::directory_iterator(directory)) {
    if (entry.is_regular_file()) {
      files.push_back(entry.path().filename().string());
    }
  }
}

std::unordered_map<std::string, int>
Indexer::file_word_count(const std::string &file) {
  std::unordered_map<std::string, int> word_count;
  std::ifstream input(directory + "/" + file);
  if (!input.is_open()) {
    throw std::runtime_error("Could not open file");
  }
  std::string line;
  while (std::getline(input, line)) {
    std::string clean_word;

    for (char &c : line) {
      // NOTE: Keep letters, numbers, and allowed symbols (apostrophes and
      // hyphens inside words)
      if (std::isalnum(c, std::locale()) || c == '\'' || c == '-') {
        clean_word += std::tolower(c, std::locale());
      } else {

        if (!clean_word.empty()) {
          word_count[clean_word]++;
          clean_word.clear();
        }
      }
    }
    if (!clean_word.empty()) {
      word_count[clean_word]++;
    }
  }
  return word_count;
}

void Indexer::index_directory() {
  ArrayList<std::thread> threads;

  // NOTE: Anon func for each thread
  auto worker = [this](ArrayList<std::string> files) {
    for (const std::string &file : files) {
      std::unordered_map<std::string, int> word_count = file_word_count(file);

      std::lock_guard<std::mutex> lock(index_mutex);
      for (auto const &pair : word_count) {
        if (index.find(pair.first) == index.end()) {
          Frequency freq;
          freq.total = pair.second;
          FileFrequency file_freq;
          file_freq.file = file;
          file_freq.count = pair.second;
          file_freq.tf = pair.second / (double)word_count.size(); // NOTE: Term frequency
          freq.files.push_back(file_freq);
          index[pair.first] = freq;
        } else {
          index[pair.first].total += pair.second;
          FileFrequency file_freq;
          file_freq.file = file;
          file_freq.count = pair.second;
          index[pair.first].files.push_back(file_freq);
        }
      }

      for (auto &pair : index) {
        pair.second.idf = std::log(files.size() / pair.second.files.size());
      }
    }
  };

  int num_threads = std::thread::hardware_concurrency();
  if (num_threads == 0) {
    num_threads = 1; // WARNING: hardware_concurrency returns 0 when the machine is single-core
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
  std::ofstream index_file(indexFile);
  index_file << "word idf total file1 tf1 count1 file2 tf2 count2 ..." << std::endl; // NOTE: Header
  //
  for (auto const &pair : index) {
    index_file << pair.first << " " << pair.second.idf << " " << pair.second.total;
    for (int i = 0; i < pair.second.files.size(); i++) {
      index_file << " " << pair.second.files[i].file << " " << pair.second.files[i].tf
                 << pair.second.files[i].count;
    }
    index_file << std::endl;
  }
  index_file.close();

  std::cout << "Index written to " << indexFile << std::endl;
}



