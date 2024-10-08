#include "cli.hpp"
#include "trie.hpp"
#include "indexer.hpp"
#include "array_list.hpp"
#include <iostream>
#include <filesystem>

// void search_handler(ArrayList<std::string> args) {

//   std::cout << "search_handler called with " << args.size() << " arguments." << std::endl;
//   if (args.size() != 2) {
//     std::cerr << "Usage: search <index path>" << std::endl;
//     return;
//   }

//   std::cout << "Searching for " << args.get(1) << std::endl;

//   return;
// }

void search_handler(ArrayList<std::string> args) {
    if (args.size() != 2) {
        std::cerr << "Usage: search <word>" << std::endl;
        return;
    }

  std::cout << "Searching for " << args[1] << std::endl;

  return;
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
