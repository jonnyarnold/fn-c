#include <iostream>

#include "src/interpreter/runtime.h"
#include "src/interpreter/objects/defs/include.h"
#include "src/interpreter/objects/defs/load.h"

class fnPrint : public fnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    std::string asString = values[0]->asString();
    std::cout << asString << "\n";
    return new fnString(&asString);
  }

public:
  fnPrint() : fnDef(
    NULL, 
    new std::vector<std::string>{"out"}
  ) {};
};

class fnNot : public fnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    bool result = !(values[0]->asBool());
    return new fnBool(result);
  }

public:
  fnNot() : fnDef(
    NULL, 
    new std::vector<std::string>{"obj"}
  ) {};
};

class fnListConstructor : public fnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    return new fnList(values);
  }

public:
  fnListConstructor() : fnDef(
    NULL, 
    new std::vector<std::string>{"...items"}
  ) {};
};

fnWorld::fnWorld() : fnValue() {
  this->locals = ValueDict{
    {"print", new fnPrint()},
    {"not", new fnNot()},
    {"List", new fnListConstructor()},
    {"include", new fnInclude(this)},
    {"load", new fnLoad()},
  };
}

fnWorld::~fnWorld() {
  this->locals.clear();
}
