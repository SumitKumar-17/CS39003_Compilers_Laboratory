%{
	#include "ass3_22CS30056_22CS30015.h"
    #define KEYWORD 1000
	#define IDENTIFIER 1001
	#define CONSTANT 1002
	#define STRING_LITERAL 1003
	#define PUNCTUATOR 1004
	#define ERROR 1005
%}


%option yylineno

LETTER_WITH_ [_a-zA-Z]
DIGIT  [0-9]
NON_ZERO_DIGIT [1-9]
DIGIT_SEQUENCE  {DIGIT}+|\"{C_CHAR_SEQUENCE}\"
EXPONENT [eE]
SIGN [+-]
ESCAPE_SEQUENCE  \\\'|\\\"|\\\?|\\\\|\\a|\\b|\\f|\\n|\\r|\\t|\\v
C_CHAR {ESCAPE_SEQUENCE}|[^'\\\n]
C_CHAR_SEQUENCE {C_CHAR}+

EXPONENT_PART {EXPONENT}{SIGN}?{DIGIT_SEQUENCE}
FRACTIONAL_CONSTANT {DIGIT_SEQUENCE}?\.{DIGIT_SEQUENCE}|{DIGIT_SEQUENCE}\.
INTEGER_CONSTANT 0|{NON_ZERO_DIGIT}{DIGIT}*
FLOAT_CONSTANT  {FRACTIONAL_CONSTANT}{EXPONENT_PART}?|{DIGIT_SEQUENCE}{EXPONENT_PART}


IDENTIFIER {LETTER_WITH_}({LETTER_WITH_}|{DIGIT})*

LINE_COMMENT				"//"[^\n]*
BLOCK_COMMENT				\/\*[^\*]*(\*+[^\*\/][^\*]*)*[\*]+\/

S_CHAR  {ESCAPE_SEQUENCE}|[^"\\\n]
S_CHAR_SEQUENCE {S_CHAR}+


CHAR_CONSTANT					\'{C_CHAR_SEQUENCE}\'
STRING_LITERAL				\"{S_CHAR_SEQUENCE}*\"

PUNCTUATOR   "["|"]"|"("|")"|"{"|"}"|"."|"->"|"++"|"--"|"&"|"*"|"+"|"-"|"~"|"!"|"/"|"%"|"<<"|">>"|"<"|">"|"<="|">="|"=="|"!="|"^"|"|"|"&&"|"||"|"?"|":"|";"|"..."|"="|"*="|"/="|"%="|"+="|"-="|"<<="|">>="|"&="|"^="|"|="|","|"#"



%%

"auto"						|
"enum"						|
"restrict"					|
"unsigned"					|
"break"						|
"extern"					|
"return"					|
"void"						|
"case"						|
"float"						|
"short"						|
"volatile"					|
"char"						|
"for"						|
"signed"					|
"while"						|
"const"						|
"goto"						|
"sizeof"					|
"_Bool"						|
"continue"					|
"if"						|
"static"					|
"_Complex"					|
"default"					|
"inline"					|
"struct"					|
"_Imaginary"				|
"do"						|
"int"						|
"switch"					|
"double"					|
"long"						|
"typedef"					|
"else"						|
"register"					|
"union"						{return KEYWORD;}

{IDENTIFIER}				{return IDENTIFIER;}
{INTEGER_CONSTANT}			{return CONSTANT;}
{FLOAT_CONSTANT}			{return CONSTANT;}
{CHAR_CONSTANT}				{return CONSTANT;}
{STRING_LITERAL}			{return STRING_LITERAL;}
{PUNCTUATOR}				{return PUNCTUATOR;}
{LINE_COMMENT}				{;}

{BLOCK_COMMENT}				{;}
[ \t]						{;}
[\r]?[\n]					{printf("\n");}
.							{return ERROR;}
%%