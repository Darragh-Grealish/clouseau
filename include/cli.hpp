#ifndef CLI_H
#define CLI_H

#include <string>
#include <unordered_map>
#include <vector>

struct Cmd {
  std::string description;
  void (*fn)(std::vector<std::string> args);
};

class CLI {
public:
  CLI(std::string name, int argc, char *argv[]);
  void add_cmd(std::string name, Cmd cmd);
  void print_help();
  void run();

private:
  std::string name;
  std::unordered_map<std::string, Cmd> cmds;
  std::vector<std::string> args;
};

#endif // CLI_H

