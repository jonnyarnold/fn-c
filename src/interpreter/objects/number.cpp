#include "src/interpreter/objects/number.h"
#include "src/interpreter/objects/def.h"
#include "src/interpreter/objects/value.h"

typedef std::unordered_map<std::string, fnValue*> ValueDict;

// Adds two integers together.
class fnAdd : public fnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    double sum =
      static_cast<fnNumber*>(this->parent)->asDouble() +
      static_cast<fnNumber*>(values[0])->asDouble();

    return new fnNumber(sum);
  }

public:
  fnAdd(fnValue* parent) : fnDef(
    parent,
    new std::vector<std::string>{"other"}
  ) {};
};

// // Subtracts two integers.
class fnSubtract : public fnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    double diff =
      static_cast<fnNumber*>(this->parent)->asDouble() -
      static_cast<fnNumber*>(values[0])->asDouble();

    return new fnNumber(diff);
  }

public:
  fnSubtract(fnValue* parent) : fnDef(
    parent,
    new std::vector<std::string>{"other"}
  ) {};
};

class fnMultiply : public fnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    double product =
      static_cast<fnNumber*>(this->parent)->asDouble() *
      static_cast<fnNumber*>(values[0])->asDouble();

    return new fnNumber(product);
  }

public:
  fnMultiply(fnValue* parent) : fnDef(
    parent,
    new std::vector<std::string>{"other"}
  ) {};
};

class fnDivide : public fnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    double quotient =
      static_cast<fnNumber*>(this->parent)->asDouble() /
      static_cast<fnNumber*>(values[0])->asDouble();

    return new fnNumber(quotient);
  }

public:
  fnDivide(fnValue* parent) : fnDef(
    parent,
    new std::vector<std::string>{"other"}
  ) {};
};

class fnMoreThan : public fnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    bool moreThan =
      static_cast<fnNumber*>(this->parent)->asDouble() >
      static_cast<fnNumber*>(values[0])->asDouble();

    return new fnBool(moreThan);
  }

public:
  fnMoreThan(fnValue* parent) : fnDef(
    parent,
    new std::vector<std::string>{"other"}
  ) {};
};

class fnLessThan : public fnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    bool lessThan =
      static_cast<fnNumber*>(this->parent)->asDouble() <
      static_cast<fnNumber*>(values[0])->asDouble();

    return new fnBool(lessThan);
  }

public:
  fnLessThan(fnValue* parent) : fnDef(
    parent,
    new std::vector<std::string>{"other"}
  ) {};
};

fnNumber::fnNumber(int i) : fnConstant(new Number(i)) {
  ValueDict methods = ValueDict{
    {"+", new fnAdd(this)},
    {"-", new fnSubtract(this)},
    {"*", new fnMultiply(this)},
    {"/", new fnDivide(this)},
    {"moreThan", new fnMoreThan(this)},
    {"lessThan", new fnLessThan(this)},
  };

  this->locals.insert(methods.begin(), methods.end());
}

fnNumber::fnNumber(double d) : fnConstant(new Number(d)) {
  ValueDict methods = ValueDict{
    {"+", new fnAdd(this)},
    {"-", new fnSubtract(this)},
    {"*", new fnMultiply(this)},
    {"/", new fnDivide(this)},
    {"moreThan", new fnMoreThan(this)},
    {"lessThan", new fnLessThan(this)},
  };

  this->locals.insert(methods.begin(), methods.end());
}

fnNumber::~fnNumber() {
  delete this->value;
}
