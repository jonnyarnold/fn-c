#pragma once

#include "src/interpreter/objects/value.h"
#include "src/interpreter/objects/bool.h"
#include "src/interpreter/objects/string.h"
#include "src/interpreter/objects/def.h"
#include "src/interpreter/machine.h"

#include "src/exec.h"
#include "src/errors.h"

// Loads the result of executing the file at the given path
// into a value for use.
//
// Files are loaded in a sandboxed manner:
// no variables will be given to the file for execution.
class fnLoad : public bareFnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    
    fnString* path = dynamic_cast<fnString*>(values[0]);
    if (path == NULL) {
      throw FnRuntimeError("load called with non-string argument " + values[0]->asString());
    }

    return exec(path->value->c_str(), context->debug);
  }

public:
  fnLoad() : bareFnDef(
    NULL, 
    new std::vector<std::string>{"path"}
  ) {};
};
