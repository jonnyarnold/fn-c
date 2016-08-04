#pragma once

#include <cmath>

#include "src/interpreter/objects/constant.h"
#include "src/interpreter/objects/def.h"
#include "src/errors.h"

// Generic number class, incorporating integers and doubles.
class Number {

  // Denotes the active union member.
  enum{INT, DOUBLE} active;

  // Stores the value.
  union {
    int i;
    double d;
  } value;

public:

  Number(int i) { this->active = INT; this->value.i = i; }
  Number(double d) { 
    // We check if the double could be an int;
    // if it can, we store it as an int instead.
    if (roundf(d) == d) { 
      this->active = INT; 
      this->value.i = (int)d;
    } else {
      this->active = DOUBLE; 
      this->value.d = d; 
    }
  }

  // Returns the string
  std::string asString() {
    if(this->active == INT) { return std::to_string(this->value.i); }
    else if(this->active == DOUBLE) { return std::to_string(this->value.d); }
  }

  // Returns the stored value as a size_t.
  // Useful for hashing.
  std::size_t asSizeT() {
    if(this->active == INT) {
      return std::hash<int>()(this->value.i); 
    } else if(this->active == DOUBLE) {
      return std::hash<double>()(this->value.d);
    }
  }

  double asDouble() {
    if(this->active == INT) { return (double)this->value.i; }
    else if(this->active == DOUBLE) { return this->value.d; }
  }

  int asInt() {
    if(this->active == INT) { return this->value.i; }
    else { 
      throw FnRuntimeError("Attempted conversion from double to integer.");
    }
  }
};

// Represents a number (either integer or double).
class fnNumber : public fnConstant<Number*> {  
public:
  fnNumber(int i);
  fnNumber(double d);

  virtual std::size_t hash() override {
    return this->value->asSizeT();
  }

  virtual std::string asString(int indentationLevel) override {
    return this->value->asString();
  }

  double asDouble() { return this->value->asDouble(); }
  int asInt() { return this->value->asInt(); }
};
