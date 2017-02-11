#pragma once 

#include "vendor/catch.h" // Needed by all specs.

#include "src/bytecode.h"
#include "src/vm/vm.h"

using namespace fn;

// Get the value of a code listing.
vm::Value resultOf(bytecode::CodeBlob instructions);

// // Expect a code listing to fail.
// bool failure(const char code[]);
