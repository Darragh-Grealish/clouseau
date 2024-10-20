#include "cli.h"
#include "indexer.h"
#include "trie.hpp"  
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

    std::cout << "Searching index: " << args[1] << std::endl;

    Indexer indexer(args[1]);
    indexer.deserialize_index();
    std::unordered_map<std::string, Frequency> index = indexer.get_index();


    Trie trie;
    trie.load_index(indexer);

    while (true) {
        std::string prefix;
        std::cout << "==============================================" << std::endl;
        std::cout << "Enter a prefix to search (or 'exit' to quit): ";
        std::cin >> prefix;

        if (prefix == "exit") {
            break;
        }

        ArrayList<std::string> results = trie.search(prefix);

        if (results.size() == 0) {
            std::cout << "No results found for prefix: " << prefix << std::endl;
            continue; 
        } 
        
        const int resultsPerPage = 10;
        int totalResults = results.size();
        int currentPage = 0;

        while (true) {
            int start = currentPage * resultsPerPage;
            int end = std::min(start + resultsPerPage, totalResults);
            
            std::cout << "\nSearch Results for prefix '" << prefix << "':" << std::endl;
            std::cout << "----------------------------------------------" << std::endl;

            for (int i = start; i < end; ++i) {
                const std::string& result = results[i];
                std::cout << result << std::endl;

                if (index.find(result) != index.end()) {
                    const Frequency& freq = index[result]; 
                    std::cout << "Total Occurrences: " << freq.total << std::endl;
                    std::cout << "IDF: " << freq.idf << std::endl;

                    for (const FileFrequency& file_freq : freq.files) {
                        std::cout << "File: " << file_freq.file 
                                  << ", Count: " << file_freq.count 
                                  << ", TF: " << file_freq.tf << std::endl;
                    }
                } else {
                    std::cout << "No file information found for word: " << prefix << std::endl;
                }
                std::cout << std::endl;
            }

            if (end >= totalResults) {
                std::cout << "No more results to show." << std::endl;
                break; 
            }

            std::string choice;
            std::cout << "Do you want to see more results? (y/n): ";
            std::cin >> choice;

            if (choice == "y" || choice == "Y") {
                currentPage++;
            } else {
                break;
            }
        }
    }
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
