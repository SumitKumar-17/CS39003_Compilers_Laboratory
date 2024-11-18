#ifndef _DEF_H_
#define _DEF_H_

//A basic definition file for the lexer

int yylex();
int yyparse();

void yyerror(char *);
void yyinfo(char *);

typedef struct TreeNode {
    char* nodeName;     // Name/type of the node (e.g., "primary_expression")
    int childCount;     // Number of children
    int maxChildren;
    struct TreeNode** children; // Array of pointers to children nodes
} TreeNode;

// Function to create a new node
TreeNode* createNode(char* name, int childCount)  ;

// Function to add a child to a node
void addChild(TreeNode *parent, TreeNode *child);

//Function to create leaf node
TreeNode* createLeaf(char *name);

// Function to free the memory allocated for the tree
void freeTree(TreeNode *node) ;


// Function to print the tree (for debugging)
void printParseTree(TreeNode *node, int indent);

int yywrap(void);

extern int indent_level;
extern char *yytext;
// extern int yyleng;
extern int yylineno;

extern TreeNode *root;

#endif
