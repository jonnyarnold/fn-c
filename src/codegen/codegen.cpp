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
  return bytecode::iLoad(id->name);
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
    blob.append(bytecode::iName(id->name));
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
  return b->value ? bytecode::iTrue() : bytecode::iFalse();
}

bytecode::CodeBlob CodeGenerator::digest(ast::Number* n) {
  return bytecode::iNumber(n->value);
}

bytecode::CodeBlob CodeGenerator::digest(ast::String* s) {
  // TODO
}

bytecode::CodeBlob CodeGenerator::digest(ast::Call* call) {
  bytecode::CodeBlob callBlob = bytecode::CodeBlob();

  // Digest the arguments.
  //
  // Arguments are digested in reverse order,
  // so they can be popped in order.
  for(auto arg = call->args.rbegin(); arg != call->args.rend(); ++arg) {
    callBlob.append(this->digest(*arg));
  }

  // Check builtins
  // TODO: Can we get rid of this?
  if (ast::Id* id = dynamic_cast<ast::Id*>(call->target)) {
    if (id->name == "and") {
      callBlob.append(bytecode::iAnd());
      return callBlob;
    }

    if (id->name == "or") {
      callBlob.append(bytecode::iOr());
      return callBlob;
    }

    if (id->name == "not") {
      callBlob.append(bytecode::iNot());
      return callBlob;
    }

    if (id->name == "+") {
      callBlob.append(bytecode::iAdd());
      return callBlob;
    }

    if (id->name == "-") {
      callBlob.append(bytecode::iSubtract());
      return callBlob;
    }

    if (id->name == "*") {
      callBlob.append(bytecode::iMultiply());
      return callBlob;
    }

    if (id->name == "/") {
      callBlob.append(bytecode::iDivide());
      return callBlob;
    }

    if (id->name == "eq") {
      callBlob.append(bytecode::iEq());
      return callBlob;
    }

    callBlob.append(this->digest(call->target));
    callBlob.append(bytecode::iCall());
    return callBlob;
  }

  throw "Uh oh.";
}

bytecode::CodeBlob CodeGenerator::digest(ast::Def* def) {
  bytecode::CodeBlob blockBlob = this->digest(def->body);
  blockBlob.append(bytecode::iReturnLast());

  bytecode::CodeBlob defBlob = bytecode::iDefHeader(blockBlob.size(), def->params);
  defBlob.append(blockBlob);

  return defBlob;
}

bytecode::CodeBlob CodeGenerator::digest(ast::Condition* condition) {

  bytecode::CodeBlob testBlob = this->digest(condition->test);
  bytecode::CodeBlob bodyBlob = this->digest(condition->body);
  bodyBlob.append(bytecode::iReturnLast());

  bytecode::CodeBlob condBlob = testBlob;
  condBlob.append(bytecode::iJumpIfLastFalse(bodyBlob.size()));
  condBlob.append(bodyBlob);

  return condBlob;

}

bytecode::CodeBlob CodeGenerator::digest(ast::Conditional* conditional) {
  bytecode::CodeBlob conditionBlob = bytecode::CodeBlob();

  for (auto condition : conditional->conditions) {
    conditionBlob.append(this->digest(condition));
  }

  return conditionBlob;
}
