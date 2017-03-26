// An Abstract Syntax Tree is used
// to represent a Fn program in a tree format;
// this provides the right ordering when
// generating code.
//
// This is the data structure that links the parser
// and code generator.

#pragma once

#include <string> // std::string
#include <vector> // std::vector

#include "src/number.h" // fn::Number

namespace fn { namespace ast {

  // Base class for all statements.
  class Statement {
  public:
    virtual ~Statement() {}
    virtual std::string asString(int indent) = 0;
    std::string asString() { return this->asString(0); }
  };

  // Base class for all statements
  // that refer to values.
  class Value : public Statement {
  public:
    virtual ~Value() {}
    virtual std::string asString(int indent) = 0;
  };

  // Base class for all statements
  // that alias defined values.
  class Reference : public Value {
  public:
    virtual ~Reference() {}
    virtual std::string asString(int indent) = 0;
  };

  // Represents a variable name.
  class Id : public Reference {
  public:
    std::string name;
    Id(std::string name) { this->name = name; }
    ~Id() = default;

    std::string asString(int indent) override {
      return "(ID " + this->name + ")";
    }
  };

  // Represents a nested variable reference.
  // (The variable exists within another value.)
  class Deref : public Reference {
  public:
    Value* parent;
    Value* child;
    Deref(Value* parent, Value* child) {
      this->parent = parent;
      this->child = child;
    }
    ~Deref() { delete this->child; delete this->parent; }

    std::string asString(int indent) override {
      return "(DEREF parent:" +
        this->parent->asString(indent) +
        " child:" +
        this->child->asString(indent) + ")";
    }
  };

  // Represents the definition of a variable.
  class Assignment : public Statement {
  public:
    Reference* key;
    Value* value;
    Assignment(Reference* key, Value* value) {
      this->key = key;
      this->value = value;
    }
    ~Assignment() { delete this->value; delete this->key; }

    virtual std::string asString(int indent) override {
      return "(ASSIGNMENT id:" + this->key->asString(indent) + " value:" + this->value->asString(indent) + ")";
    }
  };

  // Represents a collection of statements which,
  // when executed, return a value.
  //
  // They can be thought of as zero-arity, immediately executed
  // function closures, if you're feeling masochistic.
  class Block : public Value {
  public:
    std::vector<Statement*> statements;

    Block(std::vector<Statement*> statements) { this->statements = statements; }
    Block() { this->statements = std::vector<Statement*>(); }
    ~Block() {
      for(auto statement : this->statements) { delete statement; }
    }

    std::string asString(int indent) override {
      std::string result = "(BLOCK [\n";

      for(auto statement: this->statements) {
        result += std::string(indent + 2, ' ') +
          statement->asString(indent + 2) +
          "\n";
      }

      result += std::string(indent, ' ') + "])";

      return result;
    }
  };

  // Represents a boolean value.
  class Bool : public Value {
  public:
    bool value;
    Bool(bool value) { this->value = value; }
    ~Bool() = default;

    std::string asString(int indent) override {
      return "(BOOL " + std::to_string(this->value) + ")";
    }
  };

  // Represents a numeric value.
  class Number : public Value {
  public:
    fn::Number value;
    Number(std::string value) {
      this->value = fn::Number(value);
    }

    ~Number() = default;

    std::string asString(int indent) override {
      return "(NUMBER " + this->value.toString() + ")";
    }
  };

  // Represents a string of characters.
  class String : public Value {
  public:
    std::string value;
    String(std::string value) { this->value = value; }
    ~String() = default;

    std::string asString(int indent) override {
      return "(STRING " + this->value + ")";
    }
  };

  // Represents a call (request to execute) to a function.
  class Call : public Value {
  public:
    Reference* target;
    std::vector<Value*> args;

    Call(Reference* target, std::vector<Value*> args) {
      this->target = target;
      this->args = args;
    }

    ~Call() {
      for(auto arg : this->args) { delete arg; }
      delete this->target;
    }

    std::string asString(int indent) override {
      std::string result = "(CALL target:" +
        this->target->asString(indent) +
        " args:[\n";

      for(auto arg: this->args) {
        result += std::string(indent + 2, ' ') + arg->asString(indent + 2) + "\n";
      }

      result += std::string(indent, ' ') + "])";

      return result;
    }
  };

  // Represents a function definition (sometimes known as a prototype).
  class Def : public Value {
  public:
    std::vector<std::string> params;
    Block* body;

    Def(std::vector<std::string> params, Block* body) {
      this->params = params;
      this->body = body;
    }
    ~Def() { delete this->body; }

    size_t numParams() { return this->params.size(); }

    virtual std::string asString(int indent) override {
      std::string result = "(DEF params:[\n";

      for(auto param: this->params) {
        result += std::string(indent + 2, ' ') + param + "\n";
      }

      result += std::string(indent, ' ') +
        "] body:" +
        this->body->asString(indent) +
        ")";

      return result;
    }
  };

  // Used within Conditionals; represents a test
  // and the code to execute if the test passes.
  class Condition : public Statement {
    Value* test;
    Block* body;

  public:
    Condition(Value* test, Block* body) {
      this->test = test;
      this->body = body;
    }
    ~Condition() { delete this->body; delete this->test; }

    std::string asString(int indent) override {
      return "(COND test:" +
        this->test->asString(indent) +
        " body:" +
        this->body->asString(indent) + ")";
    }
  };

  // Represents a collection of Conditions
  // (conditionally executed code paths).
  class Conditional : public Value {
    std::vector<Condition*> conditions;

  public:
    Conditional(std::vector<Condition*> conditions) {
      this->conditions = conditions;
    }
    ~Conditional() {
      for(auto condition : this->conditions) { delete condition; }
    }

    std::string asString(int indent) override {
      std::string result = "(WHEN [\n";

      for(auto condition: this->conditions) {
        result += std::string(indent + 2, ' ') +
          condition->asString(indent + 2) +
          "\n";
      }

      result += std::string(indent, ' ') + "])";

      return result;
    }
  };

}}
