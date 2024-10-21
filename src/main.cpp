#include "array_list.hpp"
#include "cli.h"
#include "hashmap.hpp"
#include "indexer.h"

#include <iostream>

#ifdef _WIN32
#include <direct.h>
#else
#include <libgen.h>
#endif

void search_handler(ArrayList<std::string> args) {
  if (args.size() != 2) {
    std::cerr << "Usage: search <index name>" << std::endl;
    return;
  }

  std::cout << "Searching: " << args[1] << std::endl;

  Indexer indexer(args[1]);
  indexer.deserialize_index();
 
  std::cout << indexer.index.size() << " words indexed" << std::endl;

}

void index_handler(ArrayList<std::string> args) {
  if (args.size() != 2) {
    std::cerr << "Usage: index <input directory>" << std::endl;
    return;
  }

  Indexer indexer(args[1]);
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
