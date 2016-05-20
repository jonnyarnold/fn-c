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

class astId : public astValue {
public:
  std::string* name;
  astId* child;
  astId(std::string* name, astId* child) { this->name = name; this->child = child; }
  astId(std::string* name) : astId(name, NULL) {}
  fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    if (this->child == NULL) {
      return "(ID name:" + *this->name + ")";
    }

    return "(ID name:" + *this->name + " child:" + this->child->asString(indent) + ")";
  }
};

class astBlock : public astValue {
  std::vector<astStatement*> statements;

public:
  astBlock(std::vector<astStatement*> statements) { this->statements = statements; }
  astBlock() {}
  virtual fnValue* execute(fnMachine*) override;

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
  astId* key;
  astValue* value;
  astAssignment(astId* key, astValue* value) { this->key = key; this->value = value; }
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
  astId* name;
  std::vector<astValue*>* args;

public:
  astFnCall(astId* name, std::vector<astValue*>* args) { this->name = name; this->args = args; }
  virtual fnValue* execute(fnMachine*) override;

  virtual std::string asString(int indent) override {
    std::string result = "(CALL name:" + this->name->asString(indent) + " args:[\n";

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
