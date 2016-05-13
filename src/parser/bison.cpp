%{
  #include <string>
  #include <iostream>

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
  /* empty */ { std::cout << "program(1)\n"; std::vector<astStatement*>* stmts = new std::vector<astStatement*>{}; programBlock = new astBlock(*stmts); }
| statements  { std::cout << "program(2)\n"; programBlock = new astBlock(*$1); }

statements:
  statement            { std::cout << "statement(1)\n"; $$ = new std::vector<astStatement*>{$1}; }
| statement statements { std::cout << "statement(2)\n"; ($2)->push_back($1); $$ = $2; }
  ;

statement: 
  assignment 
| value      
  ;

assignment: 
  identifier '=' value { std::cout << "assignment\n"; $$ = new astAssignment($1, $3); }

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
  TINT    { std::cout << "int\n"; $$ = new astInt($1); }
| TDOUBLE { std::cout << "double\n"; $$ = new astDouble($1); }
| TSTRING { std::cout << "string\n"; $$ = new astString($1); }
| TBOOL   { std::cout << "bool\n"; $$ = new astBool($1); }
  ;

infixOperation: 
  value TINFIX value { std::cout << "infix\n"; astId* id = new astId($2); $$ = new astFnCall(id, new std::vector<astValue*>{$1, $3}); }

identifier:
  TID                { std::cout << "id(1)\n"; $$ = new astId($1); }
| TINFIX             { std::cout << "id(2)\n"; $$ = new astId($1); }
| TID '.' identifier { std::cout << "id(3)\n"; $$ = new astId($1, $3); }


functionCall:
  identifier '(' args ')' { std::cout << "fnCall\n"; $$ = new astFnCall($1, $3); }

args:
  /* empty */    { std::cout << "args(1)\n"; $$ = new std::vector<astValue*>{}; }
| value          { std::cout << "args(2)\n"; $$ = new std::vector<astValue*>{$1}; }
| value ',' args { std::cout << "args(3)\n"; ($3)->push_back($1); $$ = $3; }

functionDef:
  '(' params ')' block { $$ = new astFnDef($2, $4); }

params:
  /* empty */    { std::cout << "params(1)\n"; $$ = new std::vector<std::string>(); }
| TID            { std::cout << "params(2)\n"; $$ = new std::vector<std::string>{*$1}; }
| TID ',' params { std::cout << "params(3)\n"; std::string* str = new std::string(*$1); ($3)->push_back(*str); $$ = $3; }

block:
  '{' '}'            { std::cout << "block(1)\n"; std::vector<astStatement*>* stmts = new std::vector<astStatement*>{}; $$ = new astBlock(*stmts); }
| '{' statements '}' { std::cout << "block(2)\n"; std::cout << ($2)->size(); $$ = new astBlock(*$2); }

conditional:
  TWHEN '{' conditions '}' { std::cout << "conditional\n"; $$ = new astConditional($3); }

conditions:
  /* empty */          { std::cout << "conditions(1)\n"; $$ = new std::vector<astCondition>(); }
| condition            { std::cout << "conditions(2)\n"; $$ = new std::vector<astCondition>{*$1}; }
| condition conditions { std::cout << "conditions(3)\n"; ($2)->push_back(*$1); $$ = $2; }

condition:
  test block { std::cout << "condition\n"; $$ = new astCondition($1, $2); }

test:
  TBOOL { std::cout << "bool\n"; $$ = new astBool($1); }
| infixOperation
| identifier
| functionCall

%%

void yyerror(const char* s) {
  std::cout << "Parse error on line " << line << ":\n" << *s;
  exit(-1);
}
