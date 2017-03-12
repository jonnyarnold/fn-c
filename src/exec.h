// exec() and Execution combine the steps
// of processing Fn code to provide an easy interface
// for executing Fn code.

#pragma once

#include <string> // std::string
#include "src/parser/parser.h" // Parser
#include "src/codegen/codegen.h" // CodeGenerator
#include "src/vm/vm.h" // VM

namespace fn {
  vm::Value execFile(const char fileName[]);
  vm::Value execCode(std::string code);

  class Execution {
    bool debug;
    Parser parser;
    CodeGenerator generator;
    VM vm;

  public:
    Execution(bool debug) { 
      this->debug = debug; 

      this->parser = Parser(debug);
      this->generator = CodeGenerator(debug);
      this->vm = VM(debug);
    }
    Execution() : Execution(false) {}

    vm::Value execFile(const char fileName[]);
    vm::Value execCode(std::string code);
  };
}

