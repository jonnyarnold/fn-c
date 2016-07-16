%{
  #include <string>
  #include <iostream>
  #include <algorithm>

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
  astReference* v_reference;
  astId* v_id;
  astDeref* v_deref;
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
  std::vector<astCondition*>* v_conditions;
}

// Terminal symbols.
%token <v_string> TSTRING TINFIX TID
%token <v_double> TDOUBLE
%token <v_int>    TINT
%token <v_bool>   TBOOL
%token <void>     TWHEN TFN

// Non-terminal symbols.
%type <v_block> program block
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
%type <v_fndef> functionDef
%type <v_fncall> functionCall infixOperation

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
  statements { programBlock = new astBlock(*$1); }

statements:
  /* empty */                     { $$ = new std::vector<astStatement*>(); }
| statements statement            { ($1)->push_back($2); $$ = $1; }
| statements statement ';'        { ($1)->push_back($2); $$ = $1; }
  ;

statement:
  assignment
| value
  ;

assignment:
  reference '=' value { $$ = new astAssignment($1, $3); }
  ;

value:
  literal
| infixOperation
| reference
| functionCall
| block
| functionDef
| conditional
  ;

brackets:
  '(' value ')' { $$ = $2; }
  ;

literal:
  TINT    { $$ = new astInt($1); }
| TDOUBLE { $$ = new astDouble($1); }
| TSTRING { $$ = new astString($1); }
| TBOOL   { $$ = new astBool($1); }
  ;

infixOperation:
  value TINFIX value { astId* id = new astId($2); $$ = new astFnCall(id, new std::vector<astValue*>{$1, $3}); }

reference:
  identifier
| brackets
| deref
  ;

identifier:
  TID    { $$ = new astId($1); }
| TINFIX { $$ = new astId($1); }
  ;

deref:
  reference '.' reference { $$ = new astDeref($1, $3); }

functionCall:
  reference '(' args ')' {

    // args are parsed in reverse order...
    std::reverse(($3)->begin(), ($3)->end());

    $$ = new astFnCall($1, $3);
  }

args:
  /* empty */    { $$ = new std::vector<astValue*>{}; }
| value          { $$ = new std::vector<astValue*>{$1}; }
| value ',' args { ($3)->push_back($1); $$ = $3; }

functionDef:
  TFN '(' params ')' block {

    // params are parsed in reverse order...
    std::reverse(($3)->begin(), ($3)->end());

    $$ = new astFnDef($3, $5);
  }

params:
  /* empty */    { $$ = new std::vector<std::string>(); }
| TID            { $$ = new std::vector<std::string>{*$1}; }
| TID ',' params { std::string* str = new std::string(*$1); ($3)->push_back(*str); $$ = $3; }

block:
  '{' statements '}' { $$ = new astBlock(*$2); }

conditional:
  TWHEN '{' conditions '}' { $$ = new astConditional($3); }

conditions:
  /* empty */          { $$ = new std::vector<astCondition*>(); }
| conditions condition { ($1)->push_back($2); $$ = $1; }

condition:
  test block { $$ = new astCondition($1, $2); }

test:
  TBOOL { $$ = new astBool($1); }
| infixOperation
| reference
| functionCall

%%

void yyerror(char const* s) {
  std::cout << "Parse error on line " << line << ": " << s;
  exit(-1);
}
