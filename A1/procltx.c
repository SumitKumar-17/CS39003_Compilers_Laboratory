#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procltx.h"
#include "lex.yy.c"

int inline_math_count = 0;
int displayed_math_count = 0;

void add_item(symboltable *T, const char *name) {
    node *p = *T;
    while (p) {
        if (!strcmp(p->name, name)) {
            p->count++;
            return;
        }
        p = p->next;
    }
    p = (node *)malloc(sizeof(node));
    strcpy(p->name, name);
    p->count = 1;
    p->next = *T;
    *T = p;
}

void print_statistics(symboltable T, const char *title) {
    printf("%s:\n", title);
    while (T) {
        printf("%s (%d)\n", T->name, T->count);
        T = T->next;
    }
}

int main() {
    int nextok;
    symboltable commands = NULL;
    symboltable environments = NULL;

    while ((nextok = yylex())) {
        switch (nextok) {
            case TILDE: 
                add_item(&commands, "~"); 
                break;
            case ALPHA_CMD: 
                add_item(&commands, yytext); 
                break;
            case NON_ALPHA_CMD: {
                char single_cmd[3] = {yytext[0], yytext[1], '\0'};
                add_item(&commands, single_cmd);
                break;
            }
            case BEGIN_ENV: {
                char env_name[MAX_STR_LEN];
                sscanf(yytext, "\\begin{%[^}]}", env_name);
                add_item(&environments, env_name);
                break;
            }
            case END_ENV:
                break;
            case INLINE_MATH:
                inline_math_count++;
                break;
            case DISPLAYED_MATH:
                displayed_math_count++;
                break;
            case COMMENT:
            case OTHER:
                break;
            default:
                printf("Unknown token\n");
                break;
        }
    }

    print_statistics(commands, "Commands used");
    print_statistics(environments, "Environments used");
    printf("%d inline math equations found\n", inline_math_count);
    printf("%d displayed math equations found\n", displayed_math_count);

    return 0;
}
