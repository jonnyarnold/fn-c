#include <iostream>
#include <vector>
#include "vendor/cxxopts.h"

#include "src/ast.h"
#include "src/interpreter/runtime.h"
#include "src/interpreter/machine.h"

extern FILE* yyin;
extern int yyparse();
extern astBlock* programBlock;

int run(const char fileName[], bool debug) {
  // Lex file.
  yyin = fopen(fileName,"r");
  yyparse();
  fclose(yyin);

  // Whoosh.
  if(debug) { std::cout << programBlock->asString(0) << "\n\n"; }

  fnMachine* context = new fnMachine(debug);
  fnValue* returnValue = programBlock->execute(context);

  return 0;
}

int parseCli(int argc, char* argv[])
{
  cxxopts::Options options("fn", " - a fun-ctional programming language!");

  options.add_options()
    ("command", "One of: run, help", cxxopts::value<std::string>())
    ("args", "", cxxopts::value<std::vector<std::string>>())
    ("d,debug", "Prints debugging information with the command");

  options.parse_positional(std::vector<std::string>{"command", "args"});

  options.parse(argc, argv);

  std::string command = options["command"].as<std::string>();

  if(command == "help" || command == "") {

    std::cout << options.help();
    return 0;

  } else if(command == "run") {

    std::vector<std::string> args = options["args"].as<std::vector<std::string>>();
    std::string fileName = args[0];
    bool debug = options.count("debug");

    return run(fileName.c_str(), debug);

  }

  // We did nothing. That's bad.
  return -1;
}
