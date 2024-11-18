%{
    #include "expr.h"
    #include "expr.tab.h"

    #include <stdio.h>
    #include <string.h>
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

"("                          {return LEFT_PARENTHESIS;}
")"                          {return RIGHT_PARENTHESIS;}

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"%"         { return MOD; }
"**"        { return POW; }


[ \t]						{;}
[\r]?[\n]					{;}

.			{printf("Unknown token on line %d: %s\n", yylineno, yytext); return yytext[0];}
%%

