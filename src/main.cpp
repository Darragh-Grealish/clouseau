#include "cli.hpp"

#include <iostream>

void search_handler(std::vector<std::string> args) {
  if (args.size() < 1) {
    std::cerr << "Usage: search <file name>" << std::endl;
  }

  std::cout << "Searching for " << args[0] << std::endl;
}

void index_handler(std::vector<std::string> args) {
  if (args.size() < 2) {
    std::cerr << "Usage: index <input directory> <index path>" << std::endl;
  }

  std::cout << "Indexing " << args[0] << " to " << args[1] << std::endl;
}

int main(int argc, char *argv[]) {
  CLI cli("clouseau", argc, argv);
  cli.add_cmd("search", Cmd{"Search for a file", search_handler});
  cli.add_cmd("index", Cmd{"Index a directory", index_handler});
  cli.run();

  return 0;
}
