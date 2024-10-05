#pragma once

#include "array_list.hpp"
#include <string>
#include "hashmap.hpp"
#include <functional>

struct Cmd {
  std::string description;
  std::function<void(ArrayList<std::string>)> fn;
};

class CLI {
public:
  CLI(std::string name, int argc, char *argv[]);

  void add_cmd(std::string name, Cmd cmd);
  void print_help();
  void run();

private:
  std::string name;
  ArrayList<std::string> args;
  HashMap<std::string, Cmd> cmds;
};
