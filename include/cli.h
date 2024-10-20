#pragma once

#include "array_list.hpp"
#include "hashmap.hpp"
#include <functional>
#include <string>

struct Cmd {
  std::string description;
  // NOTE: Callback that takes the args as input
  std::function<void(ArrayList<std::string>)> fn;
};

class CLI {
public:
  CLI(std::string name, int argc, char *argv[]);

  // NOTE: Add a command to the CLI
  void add_cmd(std::string name, Cmd cmd);

  // NOTE: Print the help message
  void print_help();

  // NOTE: Run the CLI
  void run();

private:
  std::string name;
  ArrayList<std::string> args;
  HashMap<std::string, Cmd> cmds;
};
