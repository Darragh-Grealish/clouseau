#include "indexer.hpp"
#include "array_list.hpp"
#include "chunk.hpp"
#include "hashmap.hpp"
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

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

HashMap<std::string, int> Indexer::index_file(std::string file) {
  HashMap<std::string, int> word_count;
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
  unsigned int num_threads = std::thread::hardware_concurrency();
  if (num_threads == 0)
    num_threads = 2; // WARNING: Default to 2 threads when core info n/a

  ArrayList<Chunk> file_chunks;
  int chunk_size = files.size() / num_threads;
  for (int i = 0; i < num_threads; i++) {
    Chunk chunk;
    chunk.start = i * chunk_size;
    chunk.end = (i == num_threads - 1) ? files.size() : (i + 1) * chunk_size;
    file_chunks.add(chunk);
  }

  auto index_chunk = [&](Chunk chunk) {
    for (int i = chunk.start; i < chunk.end; i++) {
      HashMap<std::string, int> word_count =
          index_file(files.get(i));
      for (auto const &pair : word_count) {
        std::lock_guard<std::mutex> lock(index_mutex);
        if (index.find(pair.first) == index.end()) {
          Frequency freq;
          freq.total = pair.second;
          FileFrequency file_freq;
          file_freq.file = files.get(i);
          file_freq.count = pair.second;
          freq.files.add(file_freq);
          index[pair.first] = freq;
        } else {
          index[pair.first].total += pair.second;
          FileFrequency file_freq;
          file_freq.file = files.get(i);
          file_freq.count = pair.second;
          index[pair.first].files.add(file_freq);
        }
      }
    }
  }; // WARNING: Mutex auto released here

  typedef std::unique_ptr<std::thread> thread_ptr;
  ArrayList<thread_ptr> threads;
  for (int i = 0; i < num_threads; i++) {
    threads.add(thread_ptr(new std::thread(index_chunk, file_chunks.get(i))));
  }

  for (int i = 0; i < num_threads; i++) {
    threads.get(i)->join();
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
