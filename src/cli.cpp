#include "cli.hpp"
#include <iostream>

CLI::CLI(std::string name, int argc, char *argv[]) {
  this->name = name;

  for (int i = 1; i < argc; i++) { // NOTE: skip first arg as its the program name
    args.push_back(argv[i]); // NOTE: FIFO
  }
}

void CLI::add_cmd(std::string name, Cmd cmd) {
  cmds[name] = cmd;
}

void CLI::print_help() {
  std::cout << "Usage: " << name << " <command> [args...]" << std::endl;
  std::cout << "Commands:" << std::endl;
  for (auto &cmd : cmds) {
    std::cout << "  " << cmd.first << " - " << cmd.second.description
              << std::endl;
  }
}

void CLI::run() {
  if (args.size() == 0) {
    print_help();
    return;
  }

  auto cmd = cmds.find(args[0]);
  if (cmd == cmds.end()) {
    std::cout << "Command not found: " << args[0] << std::endl;
    print_help();
    return;
  }

  std::vector<std::string> cmd_args(args.begin() + 1, args.end());
  cmd->second.fn(cmd_args);
}

