// The Command Line Interface for fn,
// utilising cxxopts.

#include <iostream> // std::cout
#include <vector> // std::vector
#include "vendor/cxxopts.h" // cxxopts

#include "src/exec.h" // fn::exec

using namespace fn;

int run(const char fileName[], bool debug) {
  exec(fileName, debug);
  return 0;
}

int repl(bool debug) {
  Execution context = Execution(debug);

  std::string currentLine;
  vm::Value lastReturnValue;

  // Start the REPL loop
  std::cout << "fn REPL\nCTRL+C to exit\n";
  while(true) {
    // Read
    std::cout << "> ";
    std::getline(std::cin, currentLine);

    // Eval
    lastReturnValue = context.exec(currentLine);

    // Print
    std::cout << lastReturnValue.toString() << std::endl;
  }
}

int parseCli(int argc, char* argv[])
{
  cxxopts::Options options("fn", " - a fun-ctional programming language!");

  options.add_options()
    ("command", "One of: run, repl, help", cxxopts::value<std::string>())
    ("args", "", cxxopts::value<std::vector<std::string>>())
    ("d,debug", "Prints debugging information with the command");

  options.parse_positional(std::vector<std::string>{"command", "args"});

  options.parse(argc, argv);

  std::string command = options["command"].as<std::string>();
  bool debug = options.count("debug");

  if(command == "help" || command == "") {

    std::cout << options.help();
    return 0;

  } else if(command == "run") {

    std::vector<std::string> args = options["args"].as<std::vector<std::string>>();
    std::string fileName = args[0];

    return run(fileName.c_str(), debug);

  } else if(command == "repl") {

    return repl(debug);

  }

  // We did nothing. That's bad.
  return -1;
}
