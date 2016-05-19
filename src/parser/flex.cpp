%{
  #include <string>
  #include <iostream>

  #include "src/ast.h"
  #include "tmp/parse.h"

  // Keep track of the line number.
  int line = 1;
%}

%option noyywrap

%%

[ \t]                           ;
#[^\n]*                         ;

\n                               { line++; }

[0-9]+\.[0-9]*                   { yylval.v_double = atof(yytext); return TDOUBLE; }
[0-9]+                           { yylval.v_int = atoi(yytext); return TINT; }

\"[^\"]*\"                       {
  std::string* workingString = new std::string(yytext);
  workingString->erase(0, 1);
  workingString->erase(workingString->size() - 1);

  yylval.v_string = workingString;
  return TSTRING;
}

true                             { yylval.v_bool = true; return TBOOL; }
false                            { yylval.v_bool = false; return TBOOL; }

when                             { return TWHEN; }
fn                               { return TFN; }

"("|")"|"{"|"}"|"\."|"="|","|";" { return yytext[0]; }

"+"|"-"|"*"|"/"|"and"|"or"|"eq"  { yylval.v_string = new std::string(yytext); return TINFIX; }

[^ \t\n\+\-\*\/\(\)\{\}\.\,\;]+  { yylval.v_string = new std::string(yytext); return TID; }

.                                { std::cout << "Uh oh: " << yytext; yyterminate(); }

%%


