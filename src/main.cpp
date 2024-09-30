#include "cli.h"

std::string search_handler(std::vector<std::string> args) {
  if (args.size() == 0) {
    return "Usage: search <query>";
  }

  return "Searching for: " + args[0];
}

std::string index_handler(std::vector<std::string> args) {
  if (args.size() == 0) {
    return "Usage: index <path to archive directory>";
  }

  return "Indexing dir: " + args[0];
}

int main(int argc, char *argv[]) {
  CLI cli("clouseau", argc, argv);

  cli.add_cmd("search", Cmd{"Search for a file", search_handler});
  cli.add_cmd("index", Cmd{"Index a directory", index_handler});

  cli.run();

  return 0;
}
