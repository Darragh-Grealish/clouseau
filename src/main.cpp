#include "cli.h"
#include "indexer.h"
#include "array_list.hpp"
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
    std::unordered_map<std::string, Frequency> index = indexer.get_index();
        
//     while (true) {
//         std::string prefix;
//         std::cout << "Enter a prefix to search: ";
//         std::cin >> prefix;
//
//         ArrayList<std::string> results = trie.search(prefix);
//
//         if (results.size() == 0) {
//             std::cout << "No results found." << std::endl;
//         } else {
//             int result_count = results.size();
//             int display_count = 0;
//
//             while (result_count > 0) {
//                 std::cout << "\nSearch Results:" << std::endl;
//                 for (int i = 0; i < 10 && i < result_count; i++) {
//                     std::string file_path = dirPath + "/" + results[display_count];
//                     std::string title = results[display_count];
//
//                     std::cout << "Title: " << title << std::endl;
//                     std::cout << "File Path: " << file_path << std::endl;
//                     std::cout << std::endl;
//
//                     display_count++;
//                 }
//
//                 result_count -= 10;
//
//                 if (result_count > 0) {
//                     std::cout << "Press [Enter] to display the next 10 results, or type 'quit' to exit: ";
//                     std::string response;
//                     std::cin.ignore(); 
//                     std::getline(std::cin, response);
//
//                     if (response == "quit") {
//                         return; 
//                     } else if (response.empty()) {
//                         continue;
//                     } else {
//                         std::cout << "Invalid input. Please press [Enter] or type 'quit'." << std::endl;
//                         continue;
//                     }
//                 } else {
//                     std::cout << "No more entries found. Please search for a new word." << std::endl;
//                     break;
//                 }
//             }
//         }
//     }
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
