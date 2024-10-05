#include "indexer.hpp"
#include "array_list.hpp"
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

Indexer::Indexer(std::string directory, std::string indexFile) {
  this->directory = directory;
  this->indexFile = indexFile;

  for (const auto &entry : std::filesystem::directory_iterator(directory)) {
    if (entry.is_regular_file()) {
      files.add(entry.path().filename().string());
    }
  }
}

ArrayList<std::string> Indexer::file_to_words(std::string file_path) {
  std::ifstream file(directory + "/" + file_path);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << file_path << std::endl;
    return ArrayList<std::string>();
  }

  ArrayList<std::string> words;
  std::string line;

  while (std::getline(file, line)) {
    std::string word;
    for (char c : line) {
      if (std::isalnum(c)) {
        word += c;
      } else {
        if (!word.empty()) {
          words.add(word);
          word.clear();
        }
      }
    }
    if (!word.empty()) {
      words.add(word);
    }
  }

  return words;
}

std::unordered_map<std::string, int> Indexer::index_file(std::string file) {
  std::unordered_map<std::string, int> word_count;
  ArrayList<std::string> words = file_to_words(file);

  for (std::string word : words) {
    if (word_count.find(word) == word_count.end()) {
      word_count[word] = 1;
    } else {
      word_count[word]++;
    }
  }

  return word_count;
}

void Indexer::index_directory() {
  for (std::string file : files) {
    auto word_count = index_file(file);
    for (auto const &pair : word_count) {
      if (index.find(pair.first) == index.end()) {
        index[pair.first] = Frequency{
            pair.second, ArrayList<FileFrequency>{{file, pair.second}}};
      } else {
        index[pair.first].total += pair.second;
        index[pair.first].files.add(FileFrequency{file, pair.second});
      }
    }
  }
}

void Indexer::serialize_index() {
  std::ofstream index_file(indexFile);
  index_file << "word total file1 count1 file2 count2 ..." << std::endl;
  for (auto const &pair : index) {
    index_file << pair.first << " " << pair.second.total;
    for (int i = 0; i < pair.second.files.size(); i++) {
      index_file << " " << pair.second.files.get(i).file << " "
                 << pair.second.files.get(i).count;
    }
    index_file << std::endl;
  }
  index_file.close();
}

void Indexer::set_row(std::string word, Frequency freq) { index[word] = freq; }
