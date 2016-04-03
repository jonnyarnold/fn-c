#ifndef FN_AST
#define FN_AST

#include <string>
#include <vector>

#include <llvm/IR/Value.h>

class astStatement {
public:
  virtual ~astStatement() {};
  virtual llvm::Value* codegen();
};

class astValue : public astStatement {};

class astId : public astValue {
  std::string* name;
  astId* child;

public:
  astId(std::string* name, astId* child) {}
  astId(std::string* name) : astId(name, NULL) {}
  llvm::Value* codegen();
};

class astBlock : public astValue {
  std::vector<astStatement> statements;

public:
  astBlock(std::vector<astStatement> statements) {}
  astBlock() {}
  llvm::Value* codegen();
};

class astAssignment : public astStatement {
  astId* key;
  astValue* value;

public:
  astAssignment(astId* key, astValue* value) {}
  llvm::Value* codegen();
};

class astInt : public astValue {
  int value;

public:
  astInt(int value) {}
  llvm::Value* codegen();
};

class astDouble : public astValue {
  double value;

public:
  astDouble(double value) {}
  llvm::Value* codegen();
};

class astString : public astValue {
  std::string* value;

public:
  astString(std::string* value) {}
  llvm::Value* codegen();
};

class astBool : public astValue {
  bool value;

public:
  astBool(bool value) {}
  llvm::Value* codegen();
};

class astFnCall : public astValue {
  astId* name;
  std::vector<astValue>* args;

public:
  astFnCall(astId* name, std::vector<astValue>* args) {}
  llvm::Value* codegen();
};

class astFnDef : public astValue {
  std::vector<astId>* params;
  astBlock* body;

public:
  astFnDef(std::vector<astId>* params, astBlock* body) {}
  llvm::Value* codegen(); 
};

class astCondition : public astStatement {
  astValue* test;
  astBlock* body;

public:
  astCondition(astValue* test, astBlock* body) {}
  llvm::Value* codegen();
};

class astConditional : public astValue {
  std::vector<astCondition>* conditions;

public:
  astConditional(std::vector<astCondition>* conditions) {}
  llvm::Value* codegen();
};

#endif