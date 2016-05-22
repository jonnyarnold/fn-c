#include <iostream>
#include <vector>
#include "vendor/cxxopts.h"

#include "src/ast.h"
#include "src/interpreter/runtime.h"
#include "src/interpreter/machine.h"

extern FILE* yyin;
extern int yyparse();
extern astBlock* programBlock;

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char* str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

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

int repl(bool debug) {

  fnMachine* context = new fnMachine(debug);

  std::string currentLine;
  fnValue* lastReturnValue;

  // Start the REPL loop
  std::cout << "fn REPL\nCTRL+C to exit\n";
  while(true) {
    std::cout << "> ";
    std::getline(std::cin, currentLine);

    // Parse
    YY_BUFFER_STATE buffer = yy_scan_string(currentLine.c_str());
    yyparse();
    yy_delete_buffer(buffer);

    // If we used astBlock::execute, we'd run our code in
    // an isolated block, which we don't want to do in the REPL.
    // So we run it manually!
    lastReturnValue = programBlock->executeStatements(context);

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
