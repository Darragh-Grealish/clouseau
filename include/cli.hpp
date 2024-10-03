#ifndef CLI_H
#define CLI_H

#include "array_list.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

struct Cmd {
  std::string description;
  std::function<void(ArrayList<std::string>)> fn;
};

class CLI {
public:
  CLI(std::string name, int argc, char *argv[]) {
    this->name = name;

    if (argc == 1) {
      return;
    }

    for (int i = 1; i < argc;
         i++) {                // NOTE: skip first arg as its the program name
      args.add(argv[i]);
    }
  };

  void add_cmd(std::string name, Cmd cmd) { cmds[name] = cmd; }

  void print_help() {
    std::cout << "Usage: " << name << " <command> [args...]" << std::endl;
    std::cout << "Commands:" << std::endl;
    for (auto &cmd : cmds) {
      std::cout << "  " << cmd.first << " - " << cmd.second.description
                << std::endl;
    }
  };

  void run() {
    if (args.size() == 0) {
      print_help();
      return;
    }

    auto cmd = cmds.find(args.get(0));
    if (cmd == cmds.end()) {
      std::cout << "Command not found: " << args.get(0) << std::endl;
      print_help();
      return;
    }

    cmd->second.fn(args);
  };

private:
  std::string name;
  ArrayList<std::string> args;
  std::unordered_map<std::string, Cmd> cmds;
};

#endif // CLI_H
