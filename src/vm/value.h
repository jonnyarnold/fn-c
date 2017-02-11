// Union of all possible VM values.

#pragma once

#include <string> // std::string
#include "src/number.h" // Number

namespace fn { namespace vm {
  
  // fnVMValue is a union capable of storing any value.
  typedef union _Value {
    bool asBool;
    Number asNumber;

    std::string toString() {
      return "???";
    }
  } Value;

}}
