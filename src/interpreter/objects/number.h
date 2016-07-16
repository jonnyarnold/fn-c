#pragma once

#include "src/interpreter/objects/constant.h"

// TODO: WORK IN PROGRESS
// // Generic number class, incorporating integers and doubles.
// class Number {

//   // Denotes the active union member.
//   enum{INT, DOUBLE} active;

//   // Stores the value.
//   union {
//     int i;
//     double d;
//   } value;

// public:

//   Number(int i) { this->active = INT; this->value.i = i; }
//   Number(double d) { this->active = DOUBLE; this->value.d = d; }

//   // Returns the string
//   std::string asString() {
//     if(this->active == INT) { return std::to_string(this->value.i); }
//     else if(this->active == DOUBLE) { return std::to_string(this->value.d); }
//   }

//   // Returns the stored value as a size_t.
//   // Useful for hashing.
//   std::size_t asSizeT() {
//     if(this->active == INT) { return (std::size_t)(this->value.i); }
//     else if(this->active == DOUBLE) { return (std::size_t)(this->value.d); }
//   }
// };

// // Represents a number (either integer or double).
// class fnNumber : public fnConstant<Number> {
// public:
//   fnNumber(int i) : fnConstant(Number(i)) {}
//   fnNumber(double d) : fnConstant(Number(d)) {}

//   virtual std::size_t hash() override {
//     return this->value.asSizeT();
//   }

//   virtual std::string asString(int indentationLevel) override {
//     return this->value.asString();
//   }
// };

// Represents an int.
class fnInt : public fnConstant<int> {
  using fnConstant::fnConstant;

  virtual std::string asString(int indentationLevel) override {
    return std::to_string(this->value);
  }

  virtual std::size_t hash() override {
    return this->value;
  }
};

// Represents a double.
class fnDouble : public fnConstant<double> {
  using fnConstant::fnConstant;

  virtual std::string asString(int indentationLevel) override {
    return std::to_string(this->value);
  }

  virtual std::size_t hash() override {
    return this->value;
  }
};
