#ifndef FN_AST
#define FN_AST

#include <string>
#include <vector>

#include "fn.runtime.h"

class astStatement {
public:
  virtual ~astStatement() {};
  virtual fnValue* execute(fnExecution*) = 0;
};

class astValue : public astStatement {
public:
  virtual ~astValue() {};
  virtual fnValue* execute(fnExecution*) = 0;
};

class astId : public astValue {
public:
  std::string* name;
  astId* child;
  astId(std::string* name, astId* child) { this->name = name; this->child = child; }
  astId(std::string* name) : astId(name, NULL) {}
  fnValue* execute(fnExecution*) override;
};

class astBlock : public astValue {
  std::vector<astStatement*> statements;

public:
  astBlock(std::vector<astStatement*> statements) { this->statements = statements; }
  astBlock() {}
  virtual fnValue* execute(fnExecution*) override;

  int size() { return statements.size(); }
};

class astAssignment : public astStatement {
public:
  astId* key;
  astValue* value;
  astAssignment(astId* key, astValue* value) { this->key = key; this->value = value; }
  virtual fnValue* execute(fnExecution*) override;
};

class astInt : public astValue {
  int value;

public:
  astInt(int value) { this->value = value; }
  virtual fnValue* execute(fnExecution*) override;
};

class astDouble : public astValue {
  double value;

public:
  astDouble(double value) { this->value = value; }
  virtual fnValue* execute(fnExecution*) override;
};

class astString : public astValue {
  std::string* value;

public:
  astString(std::string* value) { this->value = value; }
  virtual fnValue* execute(fnExecution*) override;
};

class astBool : public astValue {
  bool value;

public:
  astBool(bool value) { this->value = value; }
  virtual fnValue* execute(fnExecution*) override;
};

class astFnCall : public astValue {
  astId* name;
  std::vector<astValue*>* args;

public:
  astFnCall(astId* name, std::vector<astValue*>* args) { this->name = name; this->args = args; }
  virtual fnValue* execute(fnExecution*) override;
};

class astFnDef : public astValue {
  std::vector<astId>* params;
  astBlock* body;

public:
  astFnDef(std::vector<astId>* params, astBlock* body) { this->params = params; this->body = body; }
  virtual fnValue* execute(fnExecution*) override;
};

class astCondition : public astStatement {
  astValue* test;
  astBlock* body;

public:
  astCondition(astValue* test, astBlock* body) { this->test = test; this->body = body; }
  virtual fnValue* execute(fnExecution*) override;
};

class astConditional : public astValue {
  std::vector<astCondition>* conditions;

public:
  astConditional(std::vector<astCondition>* conditions) { this->conditions = conditions; }
  fnValue* execute(fnExecution*) override;
};

#endif