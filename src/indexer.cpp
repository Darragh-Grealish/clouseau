#include "indexer.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

Indexer::Indexer(std::string inputDir, std::string indexPath) {
  this->inputDir = inputDir;
  this->indexPath = indexPath;

  if (inputDir.empty()) {
    throw std::invalid_argument("Input directory is empty");
  }

  for (auto &p : std::filesystem::directory_iterator(inputDir)) {
    this->files.push_back(p.path().string());
  }
}

void Indexer::index() {
  // NOTE: map of token to list of files and count
  std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>
      index;
  for (const auto &file : this->files) {
    std::cout << "Indexing " << file << std::endl;
    std::ifstream fileStream(file);
    std::string fileContents((std::istreambuf_iterator<char>(fileStream)),
                             std::istreambuf_iterator<char>());
    auto tokens = tokenize_file(fileContents);

    for (const auto &token : tokens) {
      index[token.first].push_back({file, token.second});
    }
  }

  // TODO: Rank files by token count
  // TODO: Save index to csv
  std::cout << "Indexing complete" << std::endl;
}

std::unordered_map<std::string, int>
Indexer::tokenize_file(const std::string &file_contents) {
  std::unordered_map<std::string, int> tokens;

  if (file_contents.empty()) {
    return tokens;
  }

  std::istringstream stream(file_contents);
  std::string word;
  while (stream >> word) {
    tokens[word]++;
  }

  return tokens;
}
