#include "cli.hpp"
#include "indexer.hpp"
#include "array_list.hpp"
#include <iostream>
#include "trie.hpp"
#ifdef _WIN32
#include <direct.h>
#else 
#include <libgen.h>
#endif

void search_handler(ArrayList<std::string> args) {
    std::cout << "search_handler called with " << args.size() << " arguments." << std::endl;
    if (args.size() != 2) {
      std::cerr << "Usage: search <index name>" << std::endl;
      return;
    }

    std::string indexPath = args[1];
    std::string dirPath = indexPath.substr(0, indexPath.find_last_of('/'));

    std::cout << "Searching: " << args[1] << std::endl;

    Indexer indexer(dirPath.c_str(), args[1].c_str()); 
    Trie& trie = indexer.trie;

    while (true) {
      std::string prefix;
      std::cout << "==============================================" << std::endl;
      std::cout << "Enter a prefix to search: ";
      std::cin >> prefix;

      ArrayList<std::string> results = trie.search(prefix);

      if (results.size() == 0) {
        std::cout << "No results found." << std::endl;
        std::cout << "----------------------------------------------" << std::endl;
      } else {
        int result_count = results.size();
        int display_count = 0;

        while (result_count > 0) {
          std::cout << "==============================================" << std::endl;
          std::cout << "Search Results for '" << prefix << "':" << std::endl;
          std::cout << "----------------------------------------------" << std::endl;
          for (int i = 0; i < 10 && i < result_count; i++) {
            std::string file_path = dirPath + "/" + results[display_count];
            std::string matched_string = results[display_count];

            double relevance = 0.0;
            size_t comma_pos = matched_string.find(',');
            if (comma_pos != std::string::npos) {
              std::string tf_str = matched_string.substr(comma_pos + 1);
              std::string idf_str = matched_string.substr(0, comma_pos);
              try {
                double tf = std::stod(tf_str);
                double idf = std::stod(idf_str);
                relevance = tf * idf;
              } catch (const std::invalid_argument& e) {
                std::cout << "Error: unable to parse TF-IDF values from search string." << std::endl;
                relevance = 0.0;
              } catch (const std::out_of_range& e) {
                std::cout << "Error: TF-IDF values are out of range." << std::endl;
                relevance = 0.0;
              }
            }
              std::cout << "Title: " << matched_string << std::endl;
              std::cout << "File Path: " << file_path << std::endl;
              std::cout << "Relevance: " << relevance << std::endl;
              std::cout << std::endl;

              display_count++;
            }

            result_count -= 10;

            if (result_count > 0) {
              std::cout << "----------------------------------------------" << std::endl;
              std::cout << "Press [Enter] to display the next 10 results, or type 'quit' to exit: ";
              std::string response;
              std::cin.ignore(); 
              std::getline(std::cin, response); 

              if (response == "quit") {
                return; 
              } else if (response.empty()) {
                continue;
              } else {
                std::cout << "Invalid input. Please press [Enter] or type 'quit'." << std::endl;
                continue; 
              }
            } else {
              std::cout << "----------------------------------------------" << std::endl;
              std::cout << "No more entries found. ";
              std::cout << "Press [Enter] to search for a new word, or type 'quit' to exit: ";
              std::string response;
              std::cin.ignore(); 
              std::getline(std::cin, response); 

              if (response == "quit") {
                return; 
              } else if (response.empty()) {
                break;
              } else {
                std::cout << "Invalid input. Please press Enter or type 'quit'." << std::endl;
                continue; 
              }
            }
        }
      }
    }
}

void index_handler(ArrayList<std::string> args) {
  if (args.size() != 3) {
    std::cerr << "Usage: index <input directory> <index path>" << std::endl;
    return;
  }

  Indexer indexer(args[1], args[2]);
  indexer.index_directory();
  indexer.serialize_index();
}

int main(int argc, char *argv[]) {
  CLI cli("clouseau", argc, argv);
  cli.add_cmd("search", Cmd{"Search for a file", search_handler});
  cli.add_cmd("index", Cmd{"Index a directory", index_handler});
  cli.run();

  return 0;
}