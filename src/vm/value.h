// Union of all possible VM values.

#pragma once

#include "stdio.h" // sprintf
#include <string> // std::string
#include "src/number.h" // Number
#include "src/vm/def.h" // Def

namespace fn { namespace vm {

  class Value {
  public:
    virtual ~Value() {};
    virtual bool asBool() { return true; }
    virtual Number asNumber() { throw "Not a Number!"; }
    virtual bytecode::InstructionIndex getCallCounterPos() { throw "Not callable!"; }
    virtual std::string toString() = 0;
  };

  class BoolValue : public Value {
  protected:
    bool value;
  public:
    BoolValue(bool value) {
      this->value = value;
    }
    ~BoolValue() = default;
    bool asBool() override { return this->value; }
    std::string toString() override {
      return this->value ? "true" : "false";
    }
  };

  class NumberValue : public Value {
  protected:
    Number value;
  public:
    NumberValue(Number value) : value(value) {}
    ~NumberValue() = default;
    Number asNumber() override { return this->value; }
    std::string toString() override {
      return this->value.toString();
    }
  };

  class DefValue : public Value {
  protected:
    Def value;
  public:
    DefValue(Def value) : value(value) {}
    ~DefValue() = default;
    bytecode::InstructionIndex getCallCounterPos() override { return this->value.counterStart; }
    std::string toString() override {
      return "fn @ I"
        + std::to_string((uint)this->value.counterStart)
        + ", length "
        + std::to_string((uint)this->value.length);
    }
  };


  // // ValueType is used for determining the type of a Value.
  // typedef char ValueType;
  // #define TYPE_BOOL (fn::vm::ValueType)(1)
  // #define TYPE_NUMBER (fn::vm::ValueType)(2)
  // #define TYPE_DEF (fn::vm::ValueType)(3)

  // // Value is a union capable of storing any value.
  // class Value {
  // protected:
  //   ValueType type;

  //   union {
  //     bool asBool;
  //     Number asNumber;
  //     Def asDef;
  //   } value;

  // public:
  //   Value(bool value) {
  //     this->type = TYPE_BOOL;
  //     this->value.asBool = value;
  //   }

  //   Value(Number value) {
  //     this->type = TYPE_NUMBER;
  //     this->value.asNumber = value;
  //   }

  //   Value(Def value) {
  //     this->type = TYPE_DEF;
  //     this->value.asDef = value;
  //   }

  //   bool isBool() { return this->type == TYPE_BOOL; }
  //   bool isNumber() { return this->type == TYPE_NUMBER; }
  //   bool isDef() { return this->type == TYPE_DEF; }

  //   bool asBool() { return this->value.asBool; }
  //   Number asNumber() { return this->value.asNumber; }
  //   Def asDef() { return this->value.asDef; }

  //   template<typename T>
  //   T as() {
  //     switch (this->type) {
  //       case TYPE_BOOL: return this->value.asBool; break;
  //       case TYPE_NUMBER: return this->value.asNumber; break;
  //       case TYPE_DEF: return this->value.asDef; break;
  //     }
  //   }

  //   std::string toString() {
  //     switch(this->type) {
  //       case TYPE_BOOL:
  //         return this->value.asBool ? "true" : "false";
  //         break;

  //       case TYPE_NUMBER:
  //         return std::to_string(this->value.asNumber.coefficient)
  //           + " * 10^"
  //           + std::to_string(this->value.asNumber.exponent);
  //         break;

  //       case TYPE_DEF:
  //         return "fn @ I"
  //           + std::to_string((uint)this->value.asDef.counterStart)
  //           + ", length "
  //           + std::to_string((uint)this->value.asDef.length);
  //           break;

  //       default:
  //         return "???";
  //         break;
  //     }
  //   }
  // };


}}
