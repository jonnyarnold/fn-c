%{
  #include <string>
  #include <iostream>

  #define IS_DEBUG false
  #define DEBUG(msg) if(IS_DEBUG) { std::cout << msg << std::endl; }

  #include "tmp/lex.h"
  #include "src/ast.h"

  // stuff from flex that bison needs to know about:
  extern int yylex();
  extern int line;

  void yyerror(const char *s);

  // Here we go!
  astBlock* programBlock;
%}

/* Represents the many different ways we can access our data */
%union {
  // Literals
  std::string *v_string;
  double      v_double;
  int         v_int;
  bool        v_bool;

  // AST
  astBlock* v_block;
  astStatement* v_statement;
  astId* v_id;
  astValue* v_value;
  astConditional* v_conditional;
  astCondition* v_condition;
  astAssignment* v_assignment;
  astFnCall* v_fncall;
  astFnDef* v_fndef;

  // Intermediaries
  std::vector<astStatement*>* v_statements;
  std::vector<std::string>* v_strings;
  std::vector<astValue*>* v_values;
  std::vector<astId>* v_ids;
  std::vector<astCondition>* v_conditions;
}

// Terminal symbols.
%token <v_string> TSTRING TINFIX TID
%token <v_double> TDOUBLE
%token <v_int>    TINT
%token <v_bool>   TBOOL
%token <void>     TWHEN

// Non-terminal symbols.
%type <v_block> program block
%type <v_statements> statements
%type <v_statement> statement
%type <v_id> identifier
%type <v_value> value literal test
%type <v_values> args
%type <v_strings> params
%type <v_conditional> conditional
%type <v_conditions> conditions
%type <v_condition> condition
%type <v_assignment> assignment
%type <v_fndef> functionDef
%type <v_fncall> functionCall infixOperation

// Infix operators
// FIXME: All operators have the same precedence.
%left TINFIX

// The starting rule.
%start program

%%

program:
  /* empty */ { DEBUG("program(1)"); std::vector<astStatement*>* stmts = new std::vector<astStatement*>{}; programBlock = new astBlock(*stmts); }
| statements  { DEBUG("program(2)"); programBlock = new astBlock(*$1); }

statements:
  statement            { DEBUG("statement(1)"); $$ = new std::vector<astStatement*>{$1}; }
| statement statements { DEBUG("statement(2)"); ($2)->push_back($1); $$ = $2; }
  ;

statement:
  assignment
| value
  ;

assignment:
  identifier '=' value { DEBUG("assignment"); $$ = new astAssignment($1, $3); }

value:
  literal
| infixOperation
| identifier
| functionCall
| block
| functionDef
| conditional
  ;

literal:
  TINT    { DEBUG("int"); $$ = new astInt($1); }
| TDOUBLE { DEBUG("double"); $$ = new astDouble($1); }
| TSTRING { DEBUG("string"); $$ = new astString($1); }
| TBOOL   { DEBUG("bool"); $$ = new astBool($1); }
  ;

infixOperation:
  value TINFIX value { DEBUG("infix"); astId* id = new astId($2); $$ = new astFnCall(id, new std::vector<astValue*>{$1, $3}); }

identifier:
  TID                { DEBUG("id(1)"); $$ = new astId($1); }
| TINFIX             { DEBUG("id(2)"); $$ = new astId($1); }
| TID '.' identifier { DEBUG("id(3)"); $$ = new astId($1, $3); }


functionCall:
  identifier '(' args ')' { DEBUG("fnCall"); $$ = new astFnCall($1, $3); }

args:
  /* empty */    { DEBUG("args(1)"); $$ = new std::vector<astValue*>{}; }
| value          { DEBUG("args(2)"); $$ = new std::vector<astValue*>{$1}; }
| value ',' args { DEBUG("args(3)"); ($3)->push_back($1); $$ = $3; }

functionDef:
  '(' params ')' block { $$ = new astFnDef($2, $4); }

params:
  /* empty */    { DEBUG("params(1)"); $$ = new std::vector<std::string>(); }
| TID            { DEBUG("params(2)"); $$ = new std::vector<std::string>{*$1}; }
| TID ',' params { DEBUG("params(3)"); std::string* str = new std::string(*$1); ($3)->push_back(*str); $$ = $3; }

block:
  '{' '}'            { DEBUG("block(1)"); std::vector<astStatement*>* stmts = new std::vector<astStatement*>{}; $$ = new astBlock(*stmts); }
| '{' statements '}' { DEBUG("block(2)"); $$ = new astBlock(*$2); }

conditional:
  TWHEN '{' conditions '}' { DEBUG("conditional"); $$ = new astConditional($3); }

conditions:
  /* empty */          { DEBUG("conditions(1)"); $$ = new std::vector<astCondition>(); }
| condition            { DEBUG("conditions(2)"); $$ = new std::vector<astCondition>{*$1}; }
| condition conditions { DEBUG("conditions(3)"); ($2)->push_back(*$1); $$ = $2; }

condition:
  test block { DEBUG("condition"); $$ = new astCondition($1, $2); }

test:
  TBOOL { DEBUG("bool"); $$ = new astBool($1); }
| infixOperation
| identifier
| functionCall

%%

void yyerror(const char* s) {
  std::cout << "Parse error on line " << line << ":" << *s;
  exit(-1);
}
