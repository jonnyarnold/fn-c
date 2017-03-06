#include "src/codegen/codegen.h"

#include <iostream> // std::cout

#define DEBUG(msg) if (this->debug) { std::cout << msg << std::endl; }

using namespace fn;

// Shorthand for CodeGenerator::digest(ast::Statement*)
#define try_digest_as(type) if (type* cast = dynamic_cast<type*>(statement)) { return this->digest(cast); }

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
  bytecode::ValueIndex index = this->variableIndices[id->name];
  return bytecode::iLoad(index);
}

bytecode::CodeBlob CodeGenerator::digest(ast::Deref* deref) {
  DEBUG("Start digesting " << deref->asString(0));

  DEBUG("End digesting " << deref->asString(0));
}

bytecode::CodeBlob CodeGenerator::digest(ast::Assignment* assignment) {
  DEBUG("Start digesting " << assignment->asString(0));

  bytecode::CodeBlob blob;

  // TODO: How can we avoid type checking here?
  ast::Deref* ref = dynamic_cast<ast::Deref*>(assignment->key);
  if(ref != NULL) {

    // Set context as that of parent before setting.
    blob = this->digest(ref->parent);
    // TODO: Push scope!
    blob.append(this->digest(ref->child));
    // TODO: Pop scope!

  } else {

    // If we don't have an astDeref, we have an astId.
    ast::Id* id = dynamic_cast<ast::Id*>(assignment->key);
    blob = this->digest(assignment->value);

    bytecode::ValueIndex index = this->reserveIndexFor(id->name);
    blob.append(bytecode::iSaveLastValue(index));
  }

  DEBUG("End digesting " << assignment->asString(0));

  return blob;
}

bytecode::CodeBlob CodeGenerator::digest(ast::Block* block) {
  DEBUG("Start digesting " << block->asString(0));
  bytecode::CodeBlob blockBlob = bytecode::CodeBlob();

  for (auto statement : block->statements) {
    bytecode::CodeBlob statementBlob = this->digest(statement);
    blockBlob.append(statementBlob);
  }

  DEBUG("End digesting " << block->asString(0));
  this->instructions->append(blockBlob);
  return blockBlob;
}

bytecode::CodeBlob CodeGenerator::digest(ast::Bool* b) {
  DEBUG("Digesting " << b->asString(0));
  return b->value ? bytecode::iTrue() : bytecode::iFalse();
}

bytecode::CodeBlob CodeGenerator::digest(ast::Number* n) {

}

bytecode::CodeBlob CodeGenerator::digest(ast::String* s) {

}

bytecode::CodeBlob CodeGenerator::digest(ast::Call* call) {

}

bytecode::CodeBlob CodeGenerator::digest(ast::Def* def) {

}

bytecode::CodeBlob CodeGenerator::digest(ast::Condition* condition) {

}

bytecode::CodeBlob CodeGenerator::digest(ast::Conditional* conditional) {

}

bytecode::ValueIndex CodeGenerator::reserveIndexFor(std::string name) {
  bytecode::ValueIndex index = this->nextIndex;
  this->variableIndices[name] = index;
  this->nextIndex++;
  return index;
}
