// exec() and Execution combine the steps
// of processing Fn code to provide an easy interface
// for executing Fn code.

#pragma once

#include "src/parser/parser.h"
#include "src/codegen/codegen.h"
#include "src/vm/vm.h"

namespace fn {
  vm::Value exec(const char fileName[], bool debug);

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

    vm::Value exec(std::string code);
  };
}

