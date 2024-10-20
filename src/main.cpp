#include "cli.h"
#include "indexer.h"
#include "trie.hpp"  
#include "array_list.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <unordered_set> 
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

    std::cout << "Searching index: " << args[1] << std::endl;

    Indexer indexer(args[1]);
    indexer.deserialize_index();
    std::unordered_map<std::string, Frequency> index = indexer.get_index();

    Trie trie;
    trie.load_index(indexer);

    while (true) {
        std::string query;
        std::cout << "==============================================" << std::endl;
        std::cout << "Enter a search query (or 'exit' to quit): ";
        std::getline(std::cin, query);

        std::transform(query.begin(), query.end(), query.begin(), ::tolower);
        query.erase(std::remove_if(query.begin(), query.end(),
            [](unsigned char c) { return !std::isalnum(c) && c != ' '; }), query.end());

        if (query.empty()) {
            std::cout << "No valid input provided." << std::endl;
            continue; 
        }

        if (query == "exit") {
            break;
        }

        std::istringstream iss(query);
        std::vector<std::string> tokens;
        std::string token;

        while (iss >> token) {
            tokens.push_back(token);
        }

        std::unordered_set<std::string> results;
        bool isFirstToken = true;
        std::string lastOperator;

        for (const auto& token : tokens) {
            if (token == "and" || token == "or" || token == "not") {
                lastOperator = token;
                continue; 
            }

            ArrayList<std::string> tokenResults = trie.search(token);

            if (lastOperator == "or" || isFirstToken) {
                results.insert(tokenResults.begin(), tokenResults.end());
            } else if (lastOperator == "and") {
                std::unordered_set<std::string> intersection;
                for (const auto& result : results) {
                    if (std::find(tokenResults.begin(), tokenResults.end(), result) != tokenResults.end()) {
                        intersection.insert(result);
                    }
                }
                results = intersection;
            } else if (lastOperator == "not") {
                for (const auto& result : tokenResults) {
                    results.erase(result);
                }
            }

            isFirstToken = false;
        }

        if (results.empty()) {
            std::cout << "No results found for query: " << query << std::endl;
            continue; 
        }

        std::unordered_map<std::string, double> relevance_scores;
        for (const auto& result : results) {
            if (index.find(result) != index.end()) {
                const Frequency& freq = index[result];
                double relevance = freq.total * freq.idf;
                relevance_scores[result] = relevance;
            }
        }

        std::vector<std::pair<std::string, double>> sortedResults(relevance_scores.begin(), relevance_scores.end());
        std::sort(sortedResults.begin(), sortedResults.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        const int resultsPerPage = 10;
        int totalResults = sortedResults.size();
        int currentPage = 0;

        while (true) {
            int start = currentPage * resultsPerPage;
            int end = std::min(start + resultsPerPage, totalResults);

            std::cout << "\nSearch Results for query '" << query << "':" << std::endl;
            std::cout << "----------------------------------------------" << std::endl;

            for (int i = start; i < end; ++i) {
                const auto& pair = sortedResults[i];
                const std::string& result = pair.first;
                double relevance = pair.second;

                const Frequency& freq = index[result];
                std::cout << result << std::endl;
                std::cout << "Relevance: " << relevance << std::endl;
                std::cout << "Total Occurrences: " << freq.total << std::endl;
                std::cout << "IDF: " << freq.idf << std::endl;

                for (const FileFrequency& file_freq : freq.files) {
                    std::cout << "File: " << file_freq.file 
                              << ", Count: " << file_freq.count 
                              << ", TF: " << file_freq.tf << std::endl;
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
            std::cin.ignore();

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
