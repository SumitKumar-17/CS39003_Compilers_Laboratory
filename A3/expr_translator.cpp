
#include "expr_translator.h"
#include "expr.tab.h"


Symbol *symbol_table=NULL;

// void add_symbol(char *name, int value) {
//     Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
//     sym->name = strdup(name);
//     sym->value = value;
//     sym->next = symbol_table;
//     symbol_table = sym;
// }

void add_symbol(char *name, int value) {
    Symbol *sym = symbol_table;
    while (sym != NULL) {
        if (strcmp(sym->name, name) == 0) {
            sym->value = value;
            return;
        }
        sym = sym->next;
    }
    Symbol *new_sym = (Symbol *)malloc(sizeof(Symbol));
    new_sym->name = strdup(name);
    new_sym->value = value;
    new_sym->next = symbol_table;
    symbol_table = new_sym;
}

int get_symbol(char *name) {
    Symbol *sym = symbol_table;
    while (sym != NULL) {
        if (strcmp(sym->name, name) == 0) {
            return sym->value;
        }
        sym = sym->next;
    }
    printf("Undefined symbol %s present in the input\nExiting... ", name);
    exit(1);
    // return 0; // default value
}

ExprNode *create_leaf_node(int type, void *value) {
    ExprNode *node = (ExprNode *)malloc(sizeof(ExprNode));
    node->type = type;
    if (type == NUM) {
       node->data.num = *(int *)value;
    } else if (type == ID) {
        node->data.id = strdup((char *)value);
    }
    node->left = node->right = NULL;
    return node;
}

ExprNode *create_op_node(int op, ExprNode *left, ExprNode *right) {
    ExprNode *node = (ExprNode *)malloc(sizeof(ExprNode));
    node->type = op;
    node->data.op = op;
    node->left = left;
    node->right = right;
    return node;
}

int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

int eval_expr(ExprNode *node) {
    if (node->type == NUM) {
        return node->data.num;
    } else if (node->type == ID) {
        return get_symbol(node->data.id);
    } else {
        int left_val = eval_expr(node->left);
        int right_val = eval_expr(node->right);

        if(right_val == 0 && node->data.op == DIV) {
            yyerror("Division by zero\nExiting... ");
            exit(1);
        }else if(right_val == 0 && node->data.op == MOD) {
            yyerror("Modulo by zero\nExiting... ");
            exit(1);
        }

        switch (node->data.op) {
            case PLUS: return left_val + right_val;
            case MINUS: return left_val - right_val;
            case MULT: return left_val * right_val;
            case DIV: return left_val / right_val;
            case MOD: return left_val % right_val;
            case POW: return power(left_val, right_val); //power calculation
        }
    }
    return 0;
}

void free_expr(ExprNode *node) {
    if (node->type == ID) {
        free(node->data.id);
    }
    if (node->left) free_expr(node->left);
    if (node->right) free_expr(node->right);
    free(node);
}

void printsym() {
    Symbol *sym = symbol_table;
    while (sym != NULL) {
        printf("\tSymbol(`%s`) = Value(`%d`)\n", sym->name, sym->value);
        sym = sym->next;
    }
}



int main() {
    yyparse();

    // printf("\nSymbol Table:\n");
    // printsym();
    return 0;
}
