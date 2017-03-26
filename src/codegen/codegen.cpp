#include "src/codegen/codegen.h"

#include <iostream> // std::cout

#define DEBUG(msg) if (this->debug) { std::cout << msg << std::endl; }

using namespace fn;

// Shorthand for CodeGenerator::digest(ast::Statement*)
#define try_digest_as(type) if (type* cast = dynamic_cast<type*>(statement)) { return this->digest(cast); }

bytecode::CodeBlob CodeGenerator::digestTopLevel(ast::Statement* statement) {
  bytecode::CodeBlob topLevelInstructions = this->digest(statement);
  this->instructions.append(topLevelInstructions);
  return topLevelInstructions;
}

bytecode::CodeBlob CodeGenerator::digest(ast::Statement* statement) {
  // TODO: Can we avoid this kind of forced polymorphism?
  try_digest_as(ast::Id);
  try_digest_as(ast::Deref);
  try_digest_as(ast::Assignment);
  try_digest_as(ast::Block);
  try_digest_as(ast::Bool);
  try_digest_as(ast::Number);
  try_digest_as(ast::String);
  try_digest_as(ast::Call);
  try_digest_as(ast::Def);
  try_digest_as(ast::Condition);
  try_digest_as(ast::Conditional);

  throw "Cannot digest " + statement->asString();
}

bytecode::CodeBlob CodeGenerator::digest(ast::Id* id) {
  bytecode::ValueIndex index = this->getIndexFor(id->name);
  if (index == 0) { throw "Undefined: " + id->name; }

  this->currentTable()->advanceIndex();
  return bytecode::iLoad(index);
}

bytecode::CodeBlob CodeGenerator::digest(ast::Deref* deref) {
  // bytecode::CodeBlob blob = this->digest(deref->parent);
  // TODO: Push scope!
  // blob.append(this->digest(deref->child));
  // TODO: Pop scope!
  // return blob;

  return this->digest(deref->child);
}

bytecode::CodeBlob CodeGenerator::digest(ast::Assignment* assignment) {
  bytecode::CodeBlob blob;

  // TODO: How can we avoid type checking here?
  ast::Deref* ref = dynamic_cast<ast::Deref*>(assignment->key);
  if(ref != NULL) {

    blob = this->digest(ref->parent);
    // TODO: Push scope!
    blob.append(this->digest(ref->child));
    // TODO: Pop scope!
    return blob;

  } else {

    // If we don't have an astDeref, we have an astId.
    ast::Id* id = dynamic_cast<ast::Id*>(assignment->key);
    blob = this->digest(assignment->value);
    this->rememberIndexFor(id->name);
  }

  return blob;
}

bytecode::CodeBlob CodeGenerator::digest(ast::Block* block) {
  bytecode::CodeBlob blockBlob = bytecode::CodeBlob();

  // TODO: Push new scope?
  for (auto statement : block->statements) {
    blockBlob.append(this->digest(statement));
  }
  // TODO: Pop scope into variable?

  return blockBlob;
}

bytecode::CodeBlob CodeGenerator::digest(ast::Bool* b) {
  this->currentTable()->advanceIndex();
  return b->value ? bytecode::iTrue() : bytecode::iFalse();
}

bytecode::CodeBlob CodeGenerator::digest(ast::Number* n) {
  this->currentTable()->advanceIndex();
  return bytecode::iNumber(n->value);
}

bytecode::CodeBlob CodeGenerator::digest(ast::String* s) {

}

bytecode::CodeBlob CodeGenerator::digest(ast::Call* call) {
  bytecode::CodeBlob callBlob = bytecode::CodeBlob();
  std::vector<bytecode::ValueIndex> args = std::vector<bytecode::ValueIndex>();

  // Digest the arguments
  for (auto arg : call->args) {
    callBlob.append(this->digest(arg));
    args.push_back(this->currentTable()->lastIndex());
  }

  // Every call returns a value
  this->currentTable()->advanceIndex();

  bytecode::ValueIndex defIndex = this->getIndexFor(call->target);
  if (defIndex) {
    callBlob.append(bytecode::iCall(defIndex, args.size(), &args[0]));
    return callBlob;
  }

  // Check builtins
  // TODO: Can we get rid of this?
  if (ast::Id* id = dynamic_cast<ast::Id*>(call->target)) {
    if (id->name == "and") {
      callBlob.append(bytecode::iAnd(args[0], args[1]));
      return callBlob;
    }

    if (id->name == "or") {
      callBlob.append(bytecode::iOr(args[0], args[1]));
      return callBlob;
    }

    if (id->name == "not") {
      callBlob.append(bytecode::iNot(args[0]));
      return callBlob;
    }

    if (id->name == "+") {
      callBlob.append(bytecode::iAdd(args[0], args[1]));
      return callBlob;
    }

    if (id->name == "-") {
      callBlob.append(bytecode::iSubtract(args[0], args[1]));
      return callBlob;
    }

    if (id->name == "*") {
      callBlob.append(bytecode::iMultiply(args[0], args[1]));
      return callBlob;
    }

    if (id->name == "/") {
      callBlob.append(bytecode::iDivide(args[0], args[1]));
      return callBlob;
    }

    if (id->name == "eq") {
      callBlob.append(bytecode::iEq(args[0], args[1]));
      return callBlob;
    }
  }

  throw "Undefined: " + call->target->asString(0);
}

bytecode::CodeBlob CodeGenerator::digest(ast::Def* def) {

  // Ensure the arguments and defined names
  // get the right index.
  // TODO: Inject externals
  ValueIndexTable defTable = ValueIndexTable();
  for(auto param : def->params) {
    defTable.set(param);
  }
  this->valueIndexStack.push(&defTable);

  bytecode::CodeBlob blockBlob = this->digest(def->body);
  blockBlob.append(bytecode::iReturnLast());

  bytecode::CodeBlob defBlob = bytecode::iDefHeader(blockBlob.size());
  defBlob.append(blockBlob);

  this->valueIndexStack.pop();

  this->currentTable()->advanceIndex();
  return defBlob;
}

bytecode::CodeBlob CodeGenerator::digest(ast::Condition* condition) {

}

bytecode::CodeBlob CodeGenerator::digest(ast::Conditional* conditional) {

}

bytecode::ValueIndex CodeGenerator::rememberIndexFor(ast::Reference* reference) {
  if (ast::Id* id = dynamic_cast<ast::Id*>(reference)) {
    return this->rememberIndexFor(id->name);
  }
  else { throw "Not yet implemented!"; }
}

bytecode::ValueIndex CodeGenerator::rememberIndexFor(std::string name) {
  bytecode::ValueIndex index = this->currentTable()->lastIndex();
  this->currentTable()->set(name, index);
  DEBUG(name << " => V" << std::to_string(index));

  return index;
}

bytecode::ValueIndex CodeGenerator::getIndexFor(ast::Reference* reference) {
  if (ast::Id* id = dynamic_cast<ast::Id*>(reference)) {
    return this->getIndexFor(id->name);
  }

  if (ast::Deref* deref = dynamic_cast<ast::Deref*>(reference)) {
    if (ast::Reference* ref = dynamic_cast<ast::Reference*>(deref->child)) {
      return this->getIndexFor(ref);
    }
  }

  throw "Not yet implemented!";
}

bytecode::ValueIndex CodeGenerator::getIndexFor(std::string name) {
  bytecode::ValueIndex index = this->currentTable()->get(name);
  DEBUG("V" << std::to_string(index) << " => " << name);

  return index;
}

ValueIndexTable* CodeGenerator::currentTable() {
  return this->valueIndexStack.top();
}
