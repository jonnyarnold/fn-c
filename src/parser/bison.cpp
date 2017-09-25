%{
  #include <string> // std::string
  #include <algorithm> // std::reverse

  #include "tmp/lex.h"
  #include "src/ast.h"

  // stuff from flex that bison needs to know about:
  extern int yylex();
  extern int line;

  void yyerror(const char *s);

  // Here we go!
  using namespace fn;
  fn::ast::Block* programBlock;
%}

/* Represents the many different ways we can access our data */
%union {
  // Literals
  std::string* v_string;
  bool         v_bool;

  // AST elements
  fn::ast::Block* v_block;
  fn::ast::BlockValue* v_blockvalue;
  fn::ast::Statement* v_statement;
  fn::ast::Reference* v_reference;
  fn::ast::Id* v_id;
  fn::ast::Deref* v_deref;
  fn::ast::Value* v_value;
  fn::ast::Conditional* v_conditional;
  fn::ast::Condition* v_condition;
  fn::ast::Assignment* v_assignment;
  fn::ast::Call* v_call;
  fn::ast::Def* v_def;

  // Intermediaries
  std::vector<fn::ast::Statement*>* v_statements;
  std::vector<std::string>* v_strings;
  std::vector<fn::ast::Value*>* v_values;
  std::vector<fn::ast::Id>* v_ids;
  std::vector<fn::ast::Condition*>* v_conditions;
}

// Terminal symbols.
%token <v_string> TDOUBLE TSTRING TINFIX TID
%token <v_bool>   TBOOL

// Non-terminal symbols.
%type <v_block> program block
%type <v_blockvalue> blockvalue
%type <v_statements> statements
%type <v_statement> statement
%type <v_reference> reference
%type <v_id> identifier
%type <v_deref> deref
%type <v_value> value literal test brackets
%type <v_values> args
%type <v_strings> params
%type <v_conditional> conditional
%type <v_conditions> conditions
%type <v_condition> condition
%type <v_assignment> assignment
%type <v_def> functionDef
%type <v_call> functionCall infixOperation

// The starting rule.
%start program

// Operator precedence, lowest-to-highest
%left ';'
%nonassoc '='
%nonassoc ','
%left TINFIX
%right '.'
%nonassoc TWHEN
%nonassoc TFN
%right '('
%left ')'

%%

program:
  statements { 
    programBlock = new fn::ast::Block(*$1);
    delete $1; 
  }

statements:
  /* empty */                     { $$ = new std::vector<fn::ast::Statement*>(); }
| statements statement            { ($1)->push_back($2); $$ = $1; }
| statements statement ';'        { ($1)->push_back($2); $$ = $1; }
  ;

statement:
  assignment
| value
  ;

assignment:
  reference '=' value   { $$ = new fn::ast::Assignment($1, $3); }
  ;

value:
  literal
| infixOperation
| reference
| functionCall
| blockvalue
| functionDef
| conditional
  ;

brackets:
  '(' value ')' { $$ = $2; }
  ;

literal:
  TBOOL   { $$ = new fn::ast::Bool($1); }
| TDOUBLE { $$ = new fn::ast::Number(*$1); delete $1; }
| TSTRING { $$ = new fn::ast::String(*$1); delete $1; }
  ;

infixOperation:
  value TINFIX value { 
    $$ = new fn::ast::Call(
      new fn::ast::Id(*$2), 
      std::vector<fn::ast::Value*>{$1, $3}); 

    delete $2;
  }

reference:
  identifier
| brackets
| deref
  ;

identifier:
  TID    { $$ = new fn::ast::Id(*$1); delete $1; }
| TINFIX { $$ = new fn::ast::Id(*$1); delete $1; }
  ;

deref:
  reference '.' reference { $$ = new fn::ast::Deref($1, $3); }

functionCall:
  reference '(' args ')' {

    // args are parsed in reverse order...
    std::reverse(($3)->begin(), ($3)->end());

    $$ = new fn::ast::Call($1, *$3);
    delete $3;
  }

args:
  /* empty */    { $$ = new std::vector<fn::ast::Value*>{}; }
| value          { $$ = new std::vector<fn::ast::Value*>{$1}; }
| value ',' args { ($3)->push_back($1); $$ = $3; }

functionDef:
  TFN '(' params ')' block {

    // params are parsed in reverse order...
    std::reverse(($3)->begin(), ($3)->end());

    $$ = new fn::ast::Def(*$3, $5);
    delete $3;
  }

params:
  /* empty */    { $$ = new std::vector<std::string>(); }
| TID            { $$ = new std::vector<std::string>{*$1}; delete $1; }
| TID ',' params { std::string str = std::string(*$1); ($3)->push_back(str); $$ = $3; delete $1; }

block:
  '{' statements '}' { 
    $$ = new fn::ast::Block(*$2);
    delete $2; 
  }

blockvalue:
  '{' statements '}' { 
    $$ = new fn::ast::BlockValue(*$2);
    delete $2; 
  }

conditional:
  TWHEN '{' conditions '}' { 
    $$ = new fn::ast::Conditional(*$3); 
    delete $3; 
  }

conditions:
  /* empty */          { $$ = new std::vector<fn::ast::Condition*>(); }
| conditions condition { ($1)->push_back($2); $$ = $1; }

condition:
  test block { $$ = new fn::ast::Condition($1, $2); }

test:
  TBOOL { $$ = new fn::ast::Bool($1); }
| infixOperation
| reference
| functionCall

%%

void yyerror(char const* s) { throw "Parse error"; }
