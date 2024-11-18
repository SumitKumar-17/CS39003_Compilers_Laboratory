#pragma once

#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>


using namespace std;
    
typedef struct ExprNode {
    int type;
    union {
        int num;
        char *id;
        int op;
    } data;
    struct ExprNode *left;
    struct ExprNode *right;
} ExprNode;

ExprNode *create_leaf_node(int type, void *value);
ExprNode *create_op_node(int op, ExprNode *left, ExprNode *right);
int eval_expr(ExprNode *node);
void free_expr(ExprNode *node);

typedef struct Symbol {
    char *name;
    int value;
    struct Symbol *next;
} Symbol;

void add_symbol(char *name, int value);
int get_symbol(char *name);


int yyparse();
int yylex();

void yyerror(const string &);
void yyinfo(const string &);

// void yyerror();
// void yyinfo();

extern char *yytext;
extern int yylineno;


#endif
