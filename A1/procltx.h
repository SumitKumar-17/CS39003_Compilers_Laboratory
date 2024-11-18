#ifndef PROCLTX_H
#define PROCLTX_H

#define MAX_STR_LEN 256
#define MAX_ITEMS 1024

typedef struct _node {
   char name[MAX_STR_LEN];
   int count;
   struct _node *next;
} node;

typedef node *symboltable;

enum Tokens {
    TILDE = 1,
    ALPHA_CMD,
    NON_ALPHA_CMD,
    BEGIN_ENV,
    END_ENV,
    INLINE_MATH,
    DISPLAYED_MATH,
    COMMENT,
    OTHER
};

void add_item(symboltable *T, const char *name);
void print_statistics(symboltable T, const char *title);

extern int inline_math_count;
extern int displayed_math_count;

#endif
