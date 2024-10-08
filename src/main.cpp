#include "cli.hpp"
#include "trie.hpp"
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

    std::string word = args.get(1);

    if (global_trie.search(word)) {
        std::cout << "Word '" << word << "' found in the index!" << std::endl;
    } else {
        std::cout << "Word '" << word << "' not found." << std::endl;
    }
}


// void index_handler(ArrayList<std::string> args) {
//   if (args.size() != 2) {
//     std::cerr << "Usage: index <input directory> <index path>" << std::endl;
//     return;
//   }

//   std::cout << "Indexing " << args.get(1) << " to " << args.get(2) << std::endl;
// }

void index_handler(ArrayList<std::string> args) {
    if (args.size() != 2) {
        std::cerr << "Usage: index <input directory>" << std::endl;
        return;
    }

    std::string directory = args.get(1);
    
    Trie trie;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        std::string filepath = entry.path().string();
        std::cout << "Indexing file: " << filepath << std::endl;
        trie.insertFromFile(filepath);  // Insert all words from the file into the trie
    }


    global_trie = trie;

    std::cout << "Indexing complete!" << std::endl;
}



int main(int argc, char *argv[]) {
  CLI cli("clouseau", argc, argv);
  cli.add_cmd("search", Cmd{"Search for a file", search_handler});
  cli.add_cmd("index", Cmd{"Index a directory", index_handler});
  cli.run();

  return 0;
}
