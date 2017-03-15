#include "src/exec.h"

namespace fn {

  vm::Value execFile(const char fileName[]) {
    Execution execution = new Execution();
    return execution.execFile(fileName);
  }

  vm::Value execCode(std::string code) {
    Execution execution = new Execution();
    return execution.execCode(code);
  }

  vm::Value Execution::execFile(const char fileName[]) {
    ast::Block* program = this->parser.parseFile(fileName);
    this->generator.digestTopLevel(program);
    return this->vm.run(&(this->generator.instructions));
  }

  vm::Value Execution::execCode(std::string code) {
    ast::Block* program = this->parser.parseCode(code);
    this->generator.digestTopLevel(program);
    return this->vm.run(&(this->generator.instructions));
  }

}

