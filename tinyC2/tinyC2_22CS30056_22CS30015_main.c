#include "tinyC2_22CS30056_22CS30015.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This is the file to generate the symbol table and print the tokens
// Roll No: 22CS30056 Sumit Kumar
// Roll No: 22CS30015 Aviral Singh

int yywrap(void)
{
   return 1;
}

TreeNode *createNode(char *name, int childCount)
{
   TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
   if (name != NULL)
   {
      newNode->nodeName = strdup(name);
   }
   else
   {
      newNode->nodeName = NULL;
   }
   newNode->childCount = 0;
   newNode->maxChildren = childCount;
   newNode->children = (TreeNode **)malloc(sizeof(TreeNode *) * childCount);
   return newNode;
}

// Function to add a child to a node
void addChild(TreeNode *parent, TreeNode *child)
{
   parent->children[parent->childCount++] = child;
}

// Function to create leaf node
TreeNode *createLeaf(char *name)
{
   return createNode(name, 0);
}

// Function to free the memory allocated for the tree
void freeTree(TreeNode *node)
{
   if (node == NULL)
      return;

   for (int i = 0; i < node->childCount; i++)
   {
      freeTree(node->children[i]);
   }

   free(node->children);
   if(node->nodeName!=NULL){
      free(node->nodeName);
   }
   free(node);
}

void printParseTree(TreeNode *node, int indent)
{
   if (node == NULL)
      return;

   // Print the current node with the given indentation
   for (int i = 0; i < indent; i++)
      printf("    "); 
   printf("%s\n", node->nodeName);

   // Print all children with increased indentation
   for (int i = 0; i < node->childCount; i++)
   {
      printParseTree(node->children[i], indent + 1);
   }
}

void yyerror(char *s)
{
   printf("Error found at %d line: %s\n", yylineno, s);
}

void yyinfo(char *s)
{
   printf("%s\n", s);
}

int main()
{
   yyparse();

   // Check if parsing was successful
   if (root)
   {
      // Print the parse tree starting from the root
      printf("The nodes at same indentation level are the siblings of same parent.\n");
      printf("Parse Tree:\n");
      printParseTree(root, 0);

      // Free the allocated memory for the parse tree
      // freeTree(root);
   }
   else
   {
      printf("Parsing failed or no parse tree was generated.\n");
   }

   return 0;
}
