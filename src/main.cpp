#include "array_list.hpp"
#include "cli.h"
#include "hashmap.hpp"
#include "indexer.h"

#include <iostream>
#include <set>
#include <algorithm>
#include <sstream>

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

    // Start user input loop
    while (true) {
        std::string query;
        std::cout << "==============================================" << std::endl;
        std::cout << "Enter a search query (or 'exit' to quit): ";
        std::getline(std::cin, query);

        // Exit the loop if the user types 'exit'
        if (query == "exit") {
            break;
        }

        // Transform the query to lowercase and clean the input
        std::transform(query.begin(), query.end(), query.begin(), ::tolower);
        query.erase(std::remove_if(query.begin(), query.end(),
            [](unsigned char c) { return !std::isalnum(c) && c != ' ' && c != '&' && c != '|' && c != '!'; }), query.end());

        // Handle an empty query
        if (query.empty()) {
            std::cout << "No valid input provided." << std::endl;
            continue;
        }

        // Parse the query, check if it's a single word or an expression
        std::istringstream iss(query);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        // Declare result_set to store the final result
        std::set<std::string> result_set;
        bool and_op = false, or_op = false, not_op = false;

        for (const std::string& term : tokens) {
            if (term == "and") {
                and_op = true;
                continue;
            } else if (term == "or") {
                or_op = true;
                continue;
            } else if (term == "not") {
                not_op = true;
                continue;
            }

            // Handle actual word search logic
            if (indexer.index.find(term) != indexer.index.end()) {
                Frequency freq = indexer.index[term];
                std::set<std::string> term_set;

                // Collect files for this term
                for (const auto& file_freq : freq.files) {
                    term_set.insert(file_freq.file);
                }

                if (not_op) {
                    // Remove terms from result_set for "NOT"
                    for (const auto& file : term_set) {
                        result_set.erase(file);
                    }
                    not_op = false;
                } else if (and_op) {
                    // Intersection of result_set and term_set for "AND"
                    std::set<std::string> intersection;
                    std::set_intersection(result_set.begin(), result_set.end(),
                                          term_set.begin(), term_set.end(),
                                          std::inserter(intersection, intersection.begin()));
                    result_set = intersection;
                    and_op = false;
                } else if (or_op || result_set.empty()) {
                    // Union of result_set and term_set for "OR"
                    result_set.insert(term_set.begin(), term_set.end());
                    or_op = false;
                }
            } else {
                std::cout << "Word '" << term << "' not found in the index." << std::endl;
            }
        }

        // Display the final set of results
        if (!result_set.empty()) {
            std::cout << "Files matching the query: " << std::endl;
            for (const std::string& file : result_set) {
                std::cout << file << std::endl;
            }
        } else {
            std::cout << "No results matching the query." << std::endl;
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
