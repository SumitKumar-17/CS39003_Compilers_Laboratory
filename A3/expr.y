%{

    #include "expr_translator.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>


%}

%union {
    int num;
    char *str;
    ExprNode *expr;
}

%start PROGRAM
%token <str> ID
%token <num> NUM
%token SET PLUS MINUS MULT DIV MOD POW

%type <expr> EXPR ARG
%type <str> STMT SETSTMT EXPRSTMT

%%

PROGRAM:
    STMT PROGRAM
    | STMT
    ;

STMT:
    SETSTMT
    | EXPRSTMT
    ;

SETSTMT:
     '(' SET ID NUM ')' { add_symbol($3, $4); printf("Variable %s is set to %d\n", $3, $4); }
    | '(' SET ID ID ')' { add_symbol($3, get_symbol($4)); printf("Variable %s is set to %d\n", $3, get_symbol($4)); }
    | '(' SET ID EXPR ')' { int value = eval_expr($4); add_symbol($3, value); printf("Variable %s is set to %d\n", $3, value); free_expr($4); }
    ;


EXPRSTMT:
    EXPR { int result = eval_expr($1);  printf("Standalone expression evaluates to %d\n", result); free_expr($1); }
    ;

EXPR:
    '(' PLUS ARG ARG ')' { $$ = create_op_node(PLUS, $3, $4); }
    | '(' MINUS ARG ARG ')' { $$ = create_op_node(MINUS, $3, $4); }
    | '(' MULT ARG ARG ')' { $$ = create_op_node(MULT, $3, $4); }
    | '(' DIV ARG ARG ')' { $$ = create_op_node(DIV, $3, $4); }
    | '(' MOD ARG ARG ')' { $$ = create_op_node(MOD, $3, $4); }
    | '(' POW ARG ARG ')' { $$ = create_op_node(POW, $3, $4); }
    ;

ARG:
    ID { $$ = create_leaf_node(ID, $1); }
    | NUM { $$ = create_leaf_node(NUM, &$1); }
    | EXPR { $$=$1;}
    ;

%%

void yyerror(const string &s)
{
	printf("ERROR [Line %d] : %s:%s\n", yylineno, s.c_str(),yytext);
}

void yyinfo(const string &s)
{
#ifdef PARSE
	printf("INFO [Line %d] : %s\n", yylineno, s.c_str());
#endif
}
