// Union of all possible VM values.

#pragma once

#include "stdio.h" // sprintf
#include <string> // std::string
#include "src/number.h" // Number
#include "src/vm/def.h" // Def

namespace fn { namespace vm {

  class Value; // Defined very shortly!

  typedef std::vector<vm::Value*> ValueMap;

  class Value {
  public:
    virtual ~Value() {};
    virtual bool asBool() { return true; }
    virtual Number asNumber() { throw "Not a Number!"; }
    virtual bytecode::InstructionIndex getCallCounterPos() { throw "Not callable!"; }
    virtual bool isDef() { return false; }
    virtual Def asDef() { throw "Not a Def!"; }
    virtual bool eq(Value* other) = 0;
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
    bool eq(Value* other) override {
      return (this->value == other->asBool());
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
    bool eq(Value* other) override {
      return (this->value == other->asNumber());
    }
  };

  class DefValue : public Value {
  protected:
    Def value;
  public:
    DefValue(Def value) : value(value) {}
    ~DefValue() = default;
    bytecode::InstructionIndex getCallCounterPos() override { return this->value.counterStart; }
    bool isDef() override { return true; }
    Def asDef() override { return this->value; }
    std::string toString() override {
      return "fn @ I"
        + std::to_string((uint)this->value.counterStart)
        + ", length "
        + std::to_string((uint)this->value.length);
    }
    bool eq(Value* other) override {
      if(!other->isDef()) { return false; }
      return (this->value == other->asDef());
    }
  };

}}
