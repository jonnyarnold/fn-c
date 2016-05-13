#ifndef FN_CLI
#define FN_CLI

#include "src/ast.h"

extern FILE* yyin;
extern int yyparse();
extern astBlock* programBlock;

int parseCli(int argc, char* argv[]);

#endif
