#include "src/exec.h"

namespace fn {

  vm::Value exec(const char fileName[], bool debug) {
    // Code -> AST
    Parser parser = fn::Parser(debug);
    ast::Block* program = parser.parseFile(fileName);

    // AST -> Instructions
    CodeGenerator generator = fn::CodeGenerator(debug);
    generator.digest(program);

    // Instructions -> Result
    VM vm = fn::VM(debug);
    vm::Value result = vm.run(generator.instructions);

    delete program;
    return result;
  }

  vm::Value Execution::exec(std::string code) {
    ast::Block* program = parser.parseCode(code);
    generator.digest(program);
    return vm.run(generator.instructions);
  }

}

