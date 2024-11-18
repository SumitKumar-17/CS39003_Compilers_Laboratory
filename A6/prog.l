
%{
    #include "y.tab.h"
%}

%option yylineno
%option nounput
%option noyywrap

identifier   [_a-zA-Z][_0-9a-zA-Z]*
num   [-]?([0]|[1-9][0-9]*)

%%


"set"           { return SET;}
"when"          { return WHEN;}
"loop"          { return TERMINAL_LOOP;}
"while"         { return WHILE;}

"+"             { return PLUS;}
"-"             { return MINUS;}
"*"             { return MULT;}
"/"             { return DIV;}
"%"             { return MOD;}

"<"             { return LESS_THAN;}
">"             { return GREATER_THAN;}
"<="            { return LESS_THAN_EQUAL;}
">="            { return GREATER_THAN_EQUAL;}
"/="            { return ASSIGN_DIVISION;}
"="             { return VALUE_ASSIGN;}

"("         { return LPAREN;}
")"         { return RPAREN;}

{identifier}              { yylval.vals_or_identifiers=strdup(yytext);return IDENTIFIER;}
{num}                     { yylval.vals_or_identifiers=strdup(yytext);return NUMBERS; }

[ \t]						{;}
[\r]?[\n]					{;}

.			{printf("Unknown token on line %d: %s\n", yylineno, yytext); return yytext[0];}
%%
