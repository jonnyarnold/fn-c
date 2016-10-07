
#include "src/interpreter/objects/string.h"
#include "src/interpreter/objects/number.h"
#include "src/interpreter/objects/def.h"
#include "src/interpreter/objects/value.h"

typedef std::unordered_map<std::string, fnValue*> ValueDict;

fnString::fnString(std::string* value) : fnConstant(value) {
  ValueDict methods = ValueDict{
    {"length", new fnNumber((int)value->length())},
  };

  this->locals.insert(methods.begin(), methods.end());
}

