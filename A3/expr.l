%{
    #include "expr_translator.h"
    #include "expr.tab.h"
%}
%option yylineno
%option nounput
%option noyywrap

digit       [0-9]
letter      [a-zA-Z_]
id          {letter}({letter}|{digit})*
num         [+-]?{digit}+

%%

"set"       { return SET; }
{id}        { yylval.str = strdup(yytext); return ID; }
{num}       { yylval.num = atoi(yytext); return NUM; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"%"         { return MOD; }
"**"        { return POW; }

[ \t\n]     {}
.            { return yytext[0]; } 

%%

