#ifndef FN_AST
#define FN_AST

#include <string>
#include <vector>

#include <llvm/IR/Value.h>

class astStatement {
public:
  virtual ~astStatement() {};
  virtual llvm::Value* codegen() = 0;
};

class astValue : public astStatement {
public:
  virtual ~astValue() {};
  virtual llvm::Value* codegen() = 0;
};

class astId : public astValue {
  std::string* name;
  astId* child;

public:
  astId(std::string* name, astId* child) { this->name = name; this->child = child; }
  astId(std::string* name) : astId(name, NULL) {}
  virtual llvm::Value* codegen() override;
};

class astBlock : public astValue {
  std::vector<astStatement*> statements;

public:
  astBlock(std::vector<astStatement*> statements) { this->statements = statements; }
  astBlock() {}
  virtual llvm::Value* codegen() override;

  uint size() { return statements.size(); }
};

class astAssignment : public astStatement {
  astId* key;
  astValue* value;

public:
  astAssignment(astId* key, astValue* value) { this->key = key; this->value = value; }
  virtual llvm::Value* codegen() override;
};

class astInt : public astValue {
  int value;

public:
  astInt(int value) { this->value = value; }
  virtual llvm::Value* codegen() override;
};

class astDouble : public astValue {
  double value;

public:
  astDouble(double value) { this->value = value; }
  virtual llvm::Value* codegen() override;
};

class astString : public astValue {
  std::string* value;

public:
  astString(std::string* value) { this->value = value; }
  virtual llvm::Value* codegen() override;
};

class astBool : public astValue {
  bool value;

public:
  astBool(bool value) { this->value = value; }
  virtual llvm::Value* codegen() override;
};

class astFnCall : public astValue {
  astId* name;
  std::vector<astValue*>* args;

public:
  astFnCall(astId* name, std::vector<astValue*>* args) { this->name = name; this->args = args; }
  virtual llvm::Value* codegen() override;
};

class astFnDef : public astValue {
  std::vector<astId>* params;
  astBlock* body;

public:
  astFnDef(std::vector<astId>* params, astBlock* body) { this->params = params; this->body = body; }
  virtual llvm::Value* codegen() override; 
};

class astCondition : public astStatement {
  astValue* test;
  astBlock* body;

public:
  astCondition(astValue* test, astBlock* body) { this->test = test; this->body = body; }
  virtual llvm::Value* codegen() override;
};

class astConditional : public astValue {
  std::vector<astCondition>* conditions;

public:
  astConditional(std::vector<astCondition>* conditions) { this->conditions = conditions; }
  llvm::Value* codegen() override;
};

#endif