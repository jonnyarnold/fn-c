#ifndef FN_CLI
#define FN_CLI

#include <iostream>
#include <vector>
#include "vendor/cxxopts.h"

#include "ast.h"
#include "interpreter/runtime.h"
#include "interpreter/builtins.h"

extern FILE* yyin;
extern int yyparse();
extern astBlock* programBlock;

void printHelp() {
  std::cout << "This is fn, a fun-ctional programming language!\n\n";
  std::cout << "To run a file, use `fn run <file>`.\n";
}

int run(const char fileName[]) {
  // Lex file.
  yyin = fopen(fileName,"r"); 
  yyparse();
  fclose(yyin);

  fnExecution* context = new fnExecution();
  context->blockStack->push(new fnTopBlock());

  fnValue* returnValue = programBlock->execute(context);

  std::cout << static_cast<fnInt*>(returnValue)->value;

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

    return run(fileName.c_str());
    
  }

  // We did nothing. That's bad.
  return -1;
}

#endif
