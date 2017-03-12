#pragma once 

#include "vendor/catch.h" // Needed by all specs.

#include <string> // std::string
#include "src/bytecode.h" // CodeBlob
#include "src/vm/vm.h" // VM, Value

using namespace fn;

// Get the value of a code listing.
vm::Value resultOf(bytecode::CodeBlob instructions);
vm::Value resultOf(std::string code);
