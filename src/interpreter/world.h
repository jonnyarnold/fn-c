#pragma once

#include "src/interpreter/objects/value.h"
#include "src/interpreter/objects/def.h"

// The top-level block.
class fnWorld : public fnValue {
public:
  fnWorld();
};
