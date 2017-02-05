#pragma once

#include "stdlib.h"
#include <vector>

#include "src/vm/number.h"

// The smallest unit of instruction.
// (Note that instructions are at least this size,
// but are usually more.)
typedef char fnByte;

// Opcodes
typedef fnByte fnOp;
#define FN_OP_FALSE (fnOp)(0)
#define FN_OP_TRUE (fnOp)(1)
#define FN_OP_AND (fnOp)(2)
#define FN_OP_OR (fnOp)(3)
#define FN_OP_NOT (fnOp)(4)

#define FN_OP_NUMBER (fnOp)(5)
#define FN_OP_MULTIPLY (fnOp)(6)
#define FN_OP_DIVIDE (fnOp)(7)
#define FN_OP_ADD (fnOp)(8)
#define FN_OP_SUBTRACT (fnOp)(9)

#define FN_OP_STRING (fnOp)(10)


// TODO: Helper methods for defining instructions.

// Values in the fn VM.
typedef union _fnVMValue {
  bool asBool;
  fnVMNumber asNumber;
} fnVMValue;

class fnVM {
public:

  fnVM(bool debug);
  fnVM() : fnVM(false) {};
  ~fnVM();

  // Executes a number of instructions.
  // Returns the pointer to the return value.
  fnVMValue run(fnByte instructions[], size_t num_bytes);

protected:
  bool debug;

  std::vector<fnVMValue*> values;
  fnVMValue* value(fnByte index);

  fnVMValue* declareBool(fnByte value);
  fnVMValue* declareBool(bool value);
  
  fnVMValue* fnAnd(fnByte value[]);
  fnVMValue* fnOr(fnByte value[]);
  fnVMValue* fnNot(fnByte value[]);

  fnVMValue* declareNumber(fnByte value[]);
  fnVMValue* declareNumber(fnVMNumber value);

  fnVMValue* fnMultiply(fnByte value[]);
  fnVMValue* fnDivide(fnByte value[]);
  fnVMValue* fnAdd(fnByte value[]);
  fnVMValue* fnSubtract(fnByte value[]);
};
