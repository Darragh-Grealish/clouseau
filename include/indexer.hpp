    #ifndef INDEXER_H
    #define INDEXER_H

    #include <string>
    #include <unordered_map>
    #include <vector>

    class Indexer {
    public:
        Indexer(std::string inputDir, std::string indexPath);
        void index();

    private:
        std::string inputDir;
        std::string indexPath;
        std::vector<std::string> files;

        std::unordered_map<std::string, int> tokenize_file(const std::string &file_contents);
    };

    #endif

