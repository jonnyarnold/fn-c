#include <iostream>
#include <vector>
#include "vendor/cxxopts.h"

#include "src/parser.h"
#include "src/interpreter/machine.h"

int run(const char fileName[], bool debug) {
  astBlock* program = (new fnParser(debug))->parseFile(fileName);

  fnMachine* context = new fnMachine(debug);
  fnValue* returnValue = program->execute(context);

  return 0;
}

int repl(bool debug) {
  fnParser* parser = new fnParser(debug);
  fnMachine* context = new fnMachine(debug);

  std::string currentLine;
  fnValue* lastReturnValue;

  // Start the REPL loop
  std::cout << "fn REPL\nCTRL+C to exit\n";
  while(true) {
    std::cout << "> ";
    std::getline(std::cin, currentLine);

    // Parse
    astBlock* command = parser->parseCode(currentLine);

    // If we used astBlock::execute, we'd run our code in
    // an isolated block, which we don't want to do in the REPL.
    // So we run it manually!
    lastReturnValue = command->executeStatements(context);

    std::cout << lastReturnValue->asString() << std::endl;
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
