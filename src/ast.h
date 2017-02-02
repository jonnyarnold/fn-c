#ifndef FN_AST
#define FN_AST

#include <string>
#include <vector>

// FORWARD DECLARATIONS
// See fn.runtime.h
class fnMachine;
class fnValue;

class astStatement {
public:
  virtual ~astStatement() {};
  virtual fnValue* execute(fnMachine*) = 0;
  virtual std::string asString(int indent) = 0;
  std::string asString() {
    return this->asString(0);
  }
};

class astValue : public astStatement {
public:
  virtual ~astValue() {};
  virtual fnValue* execute(fnMachine*) = 0;
  virtual std::string asString(int indent) = 0;
};

class astReference : public astValue {
public:
  virtual ~astReference() {};
  virtual fnValue* execute(fnMachine*) = 0;
  virtual std::string asString(int indent) = 0;
};

class astId : public astReference {
public:
  std::string* name;
  astId(std::string* name) { this->name = name; }
  ~astId() { delete this->name; }
  fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    return *this->name;
  }
};

class astDeref : public astReference {
public:
  astValue* parent;
  astValue* child;
  astDeref(astValue* parent, astValue* child) { this->parent = parent; this->child = child; }
  ~astDeref() { delete this->child; delete this->parent; }
  fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    return "(DEREF parent:" + this->parent->asString(indent) + " child:" + this->child->asString(indent) + ")";
  }
};

class astBlock : public astValue {
  std::vector<astStatement*> statements;

public:
  astBlock(std::vector<astStatement*> statements) { this->statements = statements; }
  astBlock() {}

  ~astBlock() {
    this->statements.clear();
  }

  virtual fnValue* execute(fnMachine*) override;
  fnValue* executeStatements(fnMachine*);

  int size() { return statements.size(); }

  virtual std::string asString(int indent) override {
    std::string result = "(BLOCK [\n";

    for(auto statement: this->statements) {
      result += std::string(indent + 2, ' ') + statement->asString(indent + 2) + "\n";
    }

    result += std::string(indent, ' ') + "])";

    return result;
  }
};

class astAssignment : public astStatement {
public:
  astReference* key;
  astValue* value;
  astAssignment(astReference* key, astValue* value) { this->key = key; this->value = value; }
  ~astAssignment() { delete this->value; delete this->key; }
  virtual fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    return "(ASSIGNMENT id:" + this->key->asString(indent) + " value:" + this->value->asString(indent) + ")";
  }
};

class astInt : public astValue {
  int value;

public:
  astInt(int value) { this->value = value; }
  virtual fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    return "(INT " + std::to_string(this->value) + ")";
  }
};

class astDouble : public astValue {
  double value;

public:
  astDouble(double value) { this->value = value; }
  virtual fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    return "(DOUBLE " + std::to_string(this->value) + ")";
  }
};

class astString : public astValue {
  std::string* value;

public:
  astString(std::string* value) { this->value = value; }
  ~astString() { delete this->value; }
  virtual fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    return "(STRING " + *this->value + ")";
  }
};

class astBool : public astValue {
  bool value;

public:
  astBool(bool value) { this->value = value; }
  virtual fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    return "(BOOL " + std::to_string(this->value) + ")";
  }
};

class astFnCall : public astValue {
  astReference* target;
  std::vector<astValue*>* args;

public:
  astFnCall(astReference* target, std::vector<astValue*>* args) { this->target = target; this->args = args; }
  ~astFnCall() { delete this->args; delete this->target; }
  virtual fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    std::string result = "(CALL target:" + this->target->asString(indent) + " args:[\n";

    for(auto arg: *this->args) {
      result += std::string(indent + 2, ' ') + arg->asString(indent + 2) + "\n";
    }

    result += std::string(indent, ' ') + "])";

    return result;
  }
};

class astFnDef : public astValue {
  std::vector<std::string>* params;
  astBlock* body;

public:
  astFnDef(std::vector<std::string>* params, astBlock* body) { this->params = params; this->body = body; }
  ~astFnDef() { delete this->body; delete this->params; }
  virtual fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    std::string result = "(DEF params:[\n";

    for(auto param: *this->params) {
      result += std::string(indent + 2, ' ') + param + "\n";
    }

    result += std::string(indent, ' ') + "] body:" + this->body->asString(indent) + ")";

    return result;
  }
};

class astCondition : public astStatement {
  astValue* test;
  astBlock* body;

public:
  astCondition(astValue* test, astBlock* body) { this->test = test; this->body = body; }
  ~astCondition() { delete this->body; delete this->test; }
  virtual fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    return "(COND test:" +
      this->test->asString(indent) +
      " body:" +
      this->body->asString(indent) + ")";
  }
};

class astConditional : public astValue {
  std::vector<astCondition*>* conditions;

public:
  astConditional(std::vector<astCondition*>* conditions) { this->conditions = conditions; }
  ~astConditional() { delete this->conditions; }
  fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    std::string result = "(WHEN [\n";

    for(auto condition: *this->conditions) {
      result += std::string(indent + 2, ' ') + condition->asString(indent + 2) + "\n";
    }

    result += std::string(indent, ' ') + "])";

    return result;
  }
};

#endif
