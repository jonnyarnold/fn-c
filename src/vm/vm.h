// The Fn VM is a basic, hand-made virtual machine.
// Using a handful of VM instructions,
// this VM is capable of running any Fn program.

#pragma once

#include "stdlib.h" // size_t
#include <vector> // std::vector

#include "src/vm/number.h" // fnVMNumber

// The smallest unit of instruction.
// (Note that instructions are at least this size,
// but are usually more.)
typedef char fnByte;

// Opcodes are the first byte of any instruction,
// and denote the operation that the VM should perform.
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

// fnVMValue is a union capable of storing any value.
typedef union _fnVMValue {
  bool asBool;
  fnVMNumber asNumber;
} fnVMValue;

// fnVM is the virtual machine that instructions are run in.
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
