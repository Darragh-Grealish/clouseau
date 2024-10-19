#include <iostream>
#include "cli.h"

CLI::CLI(std::string name, int argc, char *argv[]) {
  this->name = name;

  if (argc == 1) {
    return;
  }

  for (int i = 1; i < argc; i++) {  // NOTE: skip first arg as it's the program name
    args.push_back(argv[i]);
  }
}

void CLI::add_cmd(std::string name, Cmd cmd) {
  cmds[name] = cmd;
}

void CLI::print_help() {
  std::cout << "Usage: " << name << " <command> [args...]" << std::endl;
  std::cout << "Commands:" << std::endl;
  for (auto cmd : cmds) {
    std::cout << "  " << cmd.first << " - " << cmd.second.description
              << std::endl;
  }
}

void CLI::run() {
  if (args.size() == 0) {
    std::cout << "No arguments provided HERE " << std::endl;
    print_help();
    return;
  }

  auto iter = cmds.find(args[0]);
  if (iter == cmds.end()) {
    std::cout << "Command not found: " << args[0] << std::endl;
    print_help();
    return;
  }

  (*iter).second.fn(args);
}
