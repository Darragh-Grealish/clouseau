#include "cli.hpp"
#include <gtest/gtest.h>

// TEST: GIVEN a command with no arguments WHEN the command is executed THEN it
// should print "No arguments provided"
void testCommand(ArrayList<std::string> args) {
  if (args.empty()) {
    std::cout << "No arguments provided" << std::endl;
    return;
  }
  std::cout << "Received arguments: " << args[1] << std::endl;
}

// TEST: GIVEN a CLI with a command WHEN the command is run with arguments THEN
// it should print the correct argument
TEST(CliTest, CommandExecution) {
  const char *argv[] = {"my_program", "test", "arg1"};
  CLI cli("my_program", 3, const_cast<char **>(argv));

  Cmd cmd = {"A test command", testCommand};
  cli.add_cmd("test", cmd);

  testing::internal::CaptureStdout();
  cli.run();
  std::string result = testing::internal::GetCapturedStdout();
  EXPECT_EQ(result, "Received arguments: arg1\n");
}

// TEST: GIVEN a CLI with no command specified WHEN the help command is invoked
// THEN it should print the available commands
TEST(CliTest, HelpPrinting) {
  const char *argv[] = {"my_program"};
  CLI cli("my_program", 1, const_cast<char **>(argv));

  Cmd cmd = {"A test command", testCommand};
  cli.add_cmd("test", cmd);

  testing::internal::CaptureStdout();
  cli.run();
  std::string result = testing::internal::GetCapturedStdout();
  std::string expectedOutput =
      "Usage: my_program <command> [args...]\nCommands:\n  test - A test "
      "command\n";
  EXPECT_EQ(result, expectedOutput);
}

// TEST: GIVEN a CLI with no matching command WHEN an unknown command is entered
// THEN it should print a "Command not found" error and show available commands
TEST(CliTest, CommandNotFound) {
  const char *argv[] = {"my_program", "unknown"};
  CLI cli("my_program", 2, const_cast<char **>(argv));

  Cmd cmd = {"A test command", testCommand};
  cli.add_cmd("test", cmd);

  testing::internal::CaptureStdout();
  cli.run();
  std::string result = testing::internal::GetCapturedStdout();
  std::string expectedOutput =
      "Command not found: unknown\nUsage: my_program <command> "
      "[args...]\nCommands:\n  test - A test command\n";
  EXPECT_EQ(result, expectedOutput);
}
