#include "cli.hpp"
#include <gtest/gtest.h>

// Dummy command
void testCommand(std::vector<std::string> args) {
  if (args.empty()) {
    std::cout << "No arguments provided" << std::endl;
    return;
  }
  std::cout << "Received arguments: " << args[0] << std::endl;
}

// Test command execution
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

// Test help printing
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

// Test unknown command
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


