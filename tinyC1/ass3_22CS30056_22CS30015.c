#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.yy.c"

//This is the file to generate the symbol table and print the tokens
//Roll No: 22CS30056 Sumit Kumar
//Roll No: 22CS30015 Aviral Singh

//Structure for the symbol table
typedef struct _node {
   char *name;             
   char *token_type;       
   struct _node *next;     
} node;
typedef node *symboltable;

//Function to add an entry to the symbol table
symboltable addtbl(symboltable T, char *id, char *token_type) {
   node *p = T;

   while (p) {
      if (!strcmp(p->name, id)) {
         return T;
      }
      p = p->next;
   }

   p = (node *)malloc(sizeof(node));
   p->name = (char *)malloc((strlen(id) + 1) * sizeof(char));
   strcpy(p->name, id);
   
   p->token_type = (char *)malloc((strlen(token_type) + 1) * sizeof(char));
   strcpy(p->token_type, token_type);
   
   p->next = T;
   return p;
}

//Function to print the symbol table
void print_symboltable(symboltable T) {
   node *p = T;
   printf("Symbol Table:\n");
   while (p) {
      printf("\tLexeme: %-15s | Token Type: %-15s\n", p->name, p->token_type);
      p = p->next;
   }
}

int yywrap(){
    return 1;
}

int main(){
    int nexttok;
    symboltable symtab = NULL; 
    while ((nexttok = yylex())){
        switch (nexttok){
        case KEYWORD:
            symtab = addtbl(symtab, yytext, "KEYWORD");
            printf("<KEYWORD ,'%s'>\n", yytext);
            break;
        case IDENTIFIER:
            symtab = addtbl(symtab, yytext, "IDENTIFIER");
            printf("<IDENTIFIER ,'%s'>\n", yytext);
            break;
        case CONSTANT:
            symtab = addtbl(symtab, yytext, "CONSTANT");
            printf("<CONSTANT ,'%s'>\n", yytext);
            break;
        case STRING_LITERAL:
            symtab = addtbl(symtab, yytext, "STRING_LITERAL");
            printf("<STRING_LITERAL ,'%s'>\n", yytext);
            break;
        case PUNCTUATOR:
            symtab = addtbl(symtab, yytext, "PUNCTUATOR");
            printf("<PUNCTUATOR ,'%s'>\n", yytext);
            break;
        case ERROR:
            printf("Unknown token '%s' found on line number: %d\n", yytext, yylineno);
        default:
            break;
        }
    }
    print_symboltable(symtab);

    return 0;
}