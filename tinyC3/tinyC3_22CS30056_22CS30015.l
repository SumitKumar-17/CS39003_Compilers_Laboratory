%{
	#include "tinyC3_22CS30056_22CS30015_translator.h"
    #include "tinyC3_22CS30056_22CS30015.tab.h"
%}


%option yylineno
%option nounput
%option noyywrap

/* Definitons RegEx Expressions */
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
CHAR_CONSTANT					\'{C_CHAR_SEQUENCE}\'
STRING_LITERAL				\"{S_CHAR_SEQUENCE}*\"


IDENTIFIER {LETTER_WITH_}({LETTER_WITH_}|{DIGIT})*

LINE_COMMENT				"//"[^\n]*
BLOCK_COMMENT				\/\*[^\*]*(\*+[^\*\/][^\*]*)*[\*]+\/

S_CHAR  {ESCAPE_SEQUENCE}|[^"\\\n]
S_CHAR_SEQUENCE {S_CHAR}+


/* Keywords return token */
%%

"auto"						{return AUTO;}
"enum"						{return ENUM;}
"restrict"					{return RESTRICT;}
"unsigned"					{return UNSIGNED;}
"break"						{return BREAK;}
"extern"					{return EXTERN;}
"return"					{return RETURN;}
"void"						{return VOID_KEY;}
"case"						{return CASE;}
"float"						{return FLOAT_KEY;}
"short"						{return SHORT;}
"volatile"					{return VOLATILE;}
"char"						{return CHAR_KEY;}
"for"						{return FOR;}
"signed"					{return SIGNED;}
"while"						{return WHILE;}
"const"						{return CONST;}
"goto"						{return GOTO;}
"sizeof"					{return SIZEOF;}
"_Bool"						{return _BOOL;}
"continue"					{return CONTINUE;}
"if"						{return IF;}
"static"					{return STATIC;}
"_Complex"					{return _COMPLEX;}
"default"					{return DEFAULT;}
"inline"					{return INLINE;}
"struct"					{return STRUCT;}
"_Imaginary"				{return _IMAGINARY;}
"do"						{return DO;}
"int"						{return INT_KEY;}
"switch"					{return SWITCH;}
"double"					{return DOUBLE;}
"long"						{return LONG;}
"typedef"					{return TYPEDEF;}
"else"						{return ELSE;}
"register"					{return REGISTER;}
"union"						{return UNION;}


"..."                        {return ELLIPSIS;}
"<<="                        {return LEFT_SHIFT_ASSIGNMENT;}
">>="                        {return RIGHT_SHIFT_ASSIGNMENT;}
"++"                         {return INCREMENT;}
"--"                         {return DECREMENT;}
"->"                         {return ARROW;}
"<<"                         {return LEFT_SHIFT;}
">>"                         {return RIGHT_SHIFT;}
"*="                         {return STAR_ASSIGNMENT;}
"/="                         {return FRONT_SLASH_ASSIGNMENT;}    
"%="                         {return MODULO_ASSIGNMENT;}
"+="                         {return PLUS_ASSIGNMENT;}
"-="                         {return MINUS_ASSIGNMENT;}
"<="                         {return LESS_THAN_EQUAL_TO;}
">="                         {return GREATER_THAN_EQUAL_TO;}
"=="                         {return EQUAL_TO;}
"&&"                         {return LOGICAL_AND;}
"||"                         {return LOGICAL_OR;}
"!="                         {return NOT_EQUAL_TO;}
"&="                         {return BITWISE_AND_ASSIGNMENT;}
"^="                         {return BITWISE_XOR_ASSIGNMENT;}
"|="                         {return BITWISE_OR_ASSIGNMENT;}
"["                          {return LEFT_SQUARE_BRACKET;}   
"]"                          {return RIGHT_SQUARE_BRACKET;}
"/"                          {return FRONT_SLASH;}
"?"                          {return QUESTION_MARK;}
"="                          {return ASSIGNMENT;}
","                          {return COMMA;}
"&"                          {return BITWISE_AND;}
"|"                          {return BITWISE_OR;}
"^"                          {return BITWISE_XOR;}
"("                          {return LEFT_PARENTHESIS;}
")"                          {return RIGHT_PARENTHESIS;}
"{"                          {return LEFT_BRACE;}
"}"                          {return RIGHT_BRACE;}
"."                          {return DOT;}
"*"                          {return STAR;}
"+"                          {return PLUS;}
"-"                          {return MINUS;}
"~"                          {return TILDA;}
"!"                          {return EXCLAMATION;}
"%"                          {return MODULO;}
"<"                          {return LESS_THAN;}
">"                          {return GREATER_THAN;}
":"                          {return COLON;}
";"                          {return SEMICOLON;}
"#"                          {return HASH;}


{LINE_COMMENT}				{;}
{BLOCK_COMMENT}				{;}

{IDENTIFIER}				{
                                yylval.symbol =current_table->lookup(yytext);
                                return IDENTIFIER;
                            }
                            
{INTEGER_CONSTANT}			{ yylval.intVal=atoi(yytext); return INTEGER_CONSTANT;}
{FLOAT_CONSTANT}			{strcpy(yylval.floatVal,yytext); return FLOATING_CONSTANT;}
{CHAR_CONSTANT}				{strcpy(yylval.charVal,yytext); return CHAR_CONSTANT;}
{STRING_LITERAL}			{strcpy(yylval.stringVal,yytext); return STRING_LITERAL;}

[ \t]						{;}
[\r]?[\n]					{;}
.							{printf("Unknown token on line %d: %s\n", yylineno, yytext); return yytext[0];}
%%