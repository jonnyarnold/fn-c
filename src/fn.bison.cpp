%{
  #include <string>
  #include <iostream>

  // #include "node.h"
  #include "../tmp/lex.h"

  // stuff from flex that bison needs to know about:
  extern int yylex();
  extern int line;
   
  void yyerror(const char *s);
%}

/* Represents the many different ways we can access our data */
%union {
  // Literals
  std::string *v_string;
  double      v_double;
  int         v_int;
  bool        v_bool;
}

// Terminal symbols.
%token <v_string> TSTRING TINFIX TID
%token <v_double> TDOUBLE
%token <v_int>    TINT
%token <v_bool>   TBOOL
%token <void>     TWHEN

// Non-terminal types.
// FIXME

// Infix operators
%left TINFIX

// The starting rule.
%start program

%%

program: statements;

statements:
  statement
| statement statements
  ;

statement: 
  assignment 
| value
  ;

assignment: 
  identifier '=' value

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
  TINT    
| TDOUBLE 
| TSTRING 
| TBOOL   
  ;

infixOperation: 
  value TINFIX value

identifier:
  TID
| TID '.' identifier

functionCall:
  identifier argList

argList:
  '(' arg ')'

arg:
  // empty
| value ',' arg
| value

functionDef:
  paramList block

paramList:
  '(' param ')'

param:
  // empty
| TID ',' param
| TID

block:
  '{' statements '}'

conditional:
  TWHEN '{' conditions '}'

conditions:
  condition
| condition conditions

condition:
  test block

test:
  TBOOL
| infixOperation
| identifier
| functionCall

%%

int main(int, char**) {
  yyparse();
}

void yyerror(const char *s) {
  std::cout << "Parse error on line " << line << ":\n" << *s;
  exit(-1);
}
