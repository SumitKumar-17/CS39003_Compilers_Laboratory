#ifndef _DEF_H_
#define _DEF_H_

//A basic definition file for the lexer

int yylex();
int yywrap();

extern char *yytext;
extern int yyleng;
extern int yylineno;

#endif 