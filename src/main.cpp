#include "cli.hpp"
#include "indexer.hpp"
#include "array_list.hpp"
#include <iostream>
#include "trie.hpp"
#include <libgen.h>

void search_handler(ArrayList<std::string> args) {
    std::cout << "search_handler called with " << args.size() << " arguments." << std::endl;
    if (args.size() != 2) {
        std::cerr << "Usage: search <index path>" << std::endl;
        return;
    }

    std::string indexPath = args[1];
    std::string dirPath = indexPath.substr(0, indexPath.find_last_of('/'));

    std::cout << "Searching for " << args[1] << std::endl;

    Indexer indexer(dirPath.c_str(), args[1].c_str()); 
    Trie& trie = indexer.trie;

    std::string prefix;
    std::cout << "Enter a prefix to search: ";
    std::cin >> prefix;

    ArrayList<std::string> results = trie.search(prefix);
    if (results.size() == 0) {
        std::cout << "No results found." << std::endl;
    } else {
        std::cout << "Results:" << std::endl;
        for (const std::string& file : results) {
            std::cout << file << std::endl;
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
