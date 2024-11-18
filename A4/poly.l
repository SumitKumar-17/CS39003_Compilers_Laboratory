%{
    #include "poly.h"
    #include "poly.tab.h"

    #include <stdio.h>
    #include <string.h>
%}

%option yylineno
%option nounput

PLACEHOLDER_X_POWER_ONE "x"
ONE "1"
ZERO "0"
INTEGER_CONSTANT [2-9] 

%%

"+"                                  { return PLUS; }
"-"                                  { return MINUS; }
"^"                                  { return EXPONENT;}

{PLACEHOLDER_X_POWER_ONE}             { strcpy(yylval.identifierVal,yytext); return PLACEHOLDER_X_POWER_ONE; }
{ONE}                                { yylval.intVal=1; return ONE; }
{ZERO}                               { yylval.intVal=0; return ZERO; }
{INTEGER_CONSTANT}                   { yylval.intVal=atoi(yytext); return INTEGER_CONSTANT; }



[ \t]						{;}
[\r]?[\n]					{;}
.							{printf("Unknown token on line %d: %s\n", yylineno, yytext); return yytext[0];}
%%