#include "cli.h"
#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>

// NOTE: Dummy cmd 
std::string testCommand(std::vector<std::string> args) {
  if (args.empty()) {
    return "No arguments provided.";
  }
  return "Received arguments: " + args[0];
}

// NOTE: Redirect cout to a buffer to capture output 
std::string captureOutput(std::function<void()> func) {
  std::ostringstream buffer;
  std::streambuf *oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

  func(); // Call the function to capture output

  std::cout.rdbuf(oldCoutBuffer); // Restore original buffer
  return buffer.str();            // Return the captured output
}

// TEST: Given a command when executed then it should return the expected output 
void testCommandExecution() {
  const char *argv[] = {"my_program", "test", "arg1"};
  CLI cli("my_program", 3, const_cast<char **>(argv));

  Cmd cmd = {"A test command", testCommand};
  cli.add_cmd("test", cmd);

  std::string result = captureOutput([&cli]() { cli.run(); });
  assert(result == "Received arguments: arg1\n");
}

// TEST: Given a command when executed then it should print the help message (available commands/args)
void testHelpPrinting() {
  const char *argv[] = {"my_program"};
  CLI cli("my_program", 1, const_cast<char **>(argv));

  Cmd cmd = {"A test command", testCommand};
  cli.add_cmd("test", cmd);

  std::string result = captureOutput([&cli]() { cli.run(); });
  std::string expectedOutput =
      "Usage: my_program <command> [args...]\nCommands:\n  test - A test "
      "command\n";
  assert(result == expectedOutput);
}

// TEST: Given an unknown command when executed then it should print the help message (available commands/args)
void testCommandNotFound() {
  const char *argv[] = {"my_program", "unknown"};
  CLI cli("my_program", 2, const_cast<char **>(argv));

  Cmd cmd = {"A test command", testCommand};
  cli.add_cmd("test", cmd);

  std::string result = captureOutput([&cli]() { cli.run(); });
  std::string expectedOutput =
      "Command not found: unknown\nUsage: my_program <command> "
      "[args...]\nCommands:\n  test - A test command\n";
  assert(result == expectedOutput);
}

int main() {
  testCommandExecution();
  testHelpPrinting();
  testCommandNotFound();

  std::cout << "All tests passed!" << std::endl;
  return 0;
}
