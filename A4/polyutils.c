#include "poly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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
   // printf("Freeing the memory allocated for the tree\n");
   if (node == NULL)
      return;

   for (int i = 0; i < node->childCount; i++)
   {
      freeTree(node->children[i]);
   }

   if(node->childCount>0){
      free(node->children);
   }
   if(node->nodeName!=NULL){
      free(node->nodeName);
   }
   if(node!=NULL){
      free(node);
   }
}

void printParseTree(TreeNode *node, int indent)
{
   if (node == NULL){
      // printf("Node is NULL\n");
            return;
   }

   // Print the current node with the given indentation
   for (int i = 0; i < indent; i++)
      printf("    ");
   printf("==> %s[]\n", node->nodeName);

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

void setatt(TreeNode *node) {
    if (node == NULL)
        return;

   if(strcmp(node->nodeName,"S")==0){
      if(node->childCount==2){
         TreeNode *T=node->children[1];
         //as - is  a terminal symbol so we are checking from the nodeName
         T->inh=strcmp(node->children[0]->nodeName,"+")==0?+1:-1;
         setatt(T);
      }else{
         TreeNode *T=node->children[0];
         T->inh=+1;
         setatt(T);
      }
   }
   else if(strcmp(node->nodeName,"P")==0){
      TreeNode *T=node->children[0];
      T->inh=node->inh;
      setatt(T);

      if(node->childCount==3){
         TreeNode *P1=node->children[2];
         P1->inh=strcmp(node->children[1]->nodeName,"+")==0?+1:-1;
         setatt(P1);
      }
   }
   else if(strcmp(node->nodeName,"T")==0){
     //none rules to be done for this node
     setatt(node->children[0]);
     if(node->childCount==2){
        TreeNode *X=node->children[1];
        setatt(X);
     }
   }else if(strcmp(node->nodeName,"N")==0){
      setatt(node->children[0]);
      if(node->childCount==2){
         TreeNode *M1=node->children[1];
         M1->inh=node->children[0]->val;
         setatt(M1);
         node->val=node->children[1]->val;
      }else{
         node->val=node->children[0]->val;
      }
   }else if(strcmp(node->nodeName,"X")==0){
      if(node->childCount==3){
         TreeNode *N=node->children[2];
         setatt(N);
      }
   }
   else if(strcmp(node->nodeName,"M")==0){
      setatt(node->children[0]);
      if(node->childCount==2){
         TreeNode *M1=node->children[1];
         M1->inh=node->inh*10+node->children[0]->val;
         setatt(M1);
         node->val=node->children[1]->val;
      } else{
         node->val=node->children[0]->val+node->inh*10;
      }
   }
   else{
      if(isdigit(node->nodeName[0])){
         node->val=atoi(node->nodeName);
      }
   }
}

void printAnnotatedTree(TreeNode *node, int indent) {
    if (node == NULL)
        return;

    // Print the indentation level
    for (int i = 0; i < indent; i++)
        printf("    ");

    // Print the node information
    if (strcmp(node->nodeName, "P") == 0 ) {
        printf("==> %s [inh = %c ]\n", node->nodeName, node->inh == -1 ? '-' : (node->inh == 0 ? ' ' : '+'));
    } else if (strcmp(node->nodeName, "N") == 0 ) {
        printf("==> %s [val = %d]\n", node->nodeName,node->val);
    }else if (strcmp(node->nodeName, "T") == 0 ) {
           printf("==> %s [inh = %c ]\n", node->nodeName, node->inh == -1 ? '-' : (node->inh == 0 ? ' ' : '+'));
    }else if (strcmp(node->nodeName, "M") == 0 ) {
        printf("==> %s [inh = %d, val = %d]\n", node->nodeName,node->inh,node->val);
    } else if(isdigit(node->nodeName[0])){
          printf("==> %s [val = %d]\n", node->nodeName,node->val);
    }
    else {
        printf("==> %s []\n", node->nodeName);
    }

    // Recursively print the children
    for (int i = 0; i < node->childCount; i++) {
        printAnnotatedTree(node->children[i], indent + 1);
    }
}

int evalpoly(TreeNode *node,int x){
   if(node==NULL){
      return 0;
   }

   if(strcmp(node->nodeName,"S")==0){
      if(node->childCount==1){
         return evalpoly(node->children[0],x);
      }else if(node->childCount==2){
         return evalpoly(node->children[1],x);
      }
   }

   else if(strcmp(node->nodeName,"P")==0){
      if(node->childCount==1){
         return evalpoly(node->children[0],x);
      }else if(node->childCount==3){
         int leftValue=evalpoly(node->children[0],x);
         int rightValue=evalpoly(node->children[2],x);
         return leftValue+rightValue;
      }
   }

   else if(strcmp(node->nodeName,"T")==0){
      int coeffcient=evalpoly(node->children[0],x);
      if(node->childCount==1){
         return node->inh*coeffcient;
      }else if(node->childCount==2){
         int varValue= evalpoly(node->children[1],x);
         return node->inh*coeffcient*varValue;
      }
   }

   else if(isdigit(node->nodeName[0])){
         return node->val;
   }

   else if(strcmp(node->nodeName,"X")==0){
      if(node->childCount==1){
         return x;
      }else if(node->childCount==3){
         int power = node->children[2]->val;
         int computed = pow(x,power);
         return computed;
      }
   }

   else if(strcmp(node->nodeName,"N")==0){
      return node->val;
   }

   else if(strcmp(node->nodeName,"M")==0){
      printf("should not come to M");
      return node->val;
   }

   else {
      printf("seems like some error casing");
      return node->val;
   }

   return 0;
}

void printderivate(TreeNode *node,int coeff) {
    if (node == NULL) {
        return;
    }

    // For "S" node
    if (strcmp(node->nodeName, "S") == 0) {
        if (node->childCount == 1) {
            printderivate(node->children[0],1); // S -> P
        } else if (node->childCount == 2) {
            printderivate(node->children[1],1); // S -> +P or S -> -P
        }
    }

    // For "P" node: differentiation for sums/differences (P -> P + T or P - T)
    else if (strcmp(node->nodeName, "P") == 0) {
        if (node->childCount == 1) {
            printderivate(node->children[0],1); // P -> T
        } else if (node->childCount == 3) {
            printderivate(node->children[0],1); // Left term (P)
            // printf(" %s ", node->children[1]->nodeName); // Operator (+ or -)
            printderivate(node->children[2],1); // Right term (T)
        }
    }
    // For "T" node: differentiation for products (T -> N * X^exp)
    else if (strcmp(node->nodeName, "T") == 0) {
        TreeNode *N = node->children[0]; // The coefficient
        if (node->childCount == 1) {
            if (strcmp(N->nodeName, "N") == 0 || isdigit(N->nodeName[0])) {
               // printf(" %s ",node->inh==1?"+":"-"); // Constants differentiate to 0, no need to print anything
               // printf("%d", 0);
               return;
            } else if (strcmp(N->nodeName, "X") == 0) {
               printf(" %s ",node->inh==1?"+":"-");
               printderivate(N,1); // Differentiate variable
            }
        } else if (node->childCount == 2) {
          printf(" %s ",node->inh==1?"+":"-");
            // printf(" %d * ", N->val);
            printderivate(node->children[1],N->val); // Differentiate the variable part
        }
    }

    // For "X" node: differentiation for powers (X -> x^exp)
    else if (strcmp(node->nodeName, "X") == 0) {
        if (node->childCount == 1) {
            printf(" %d ", coeff); // Derivative of x is 1
        } else if (node->childCount == 3) {
            TreeNode *expNode = node->children[2]; // The exponent
            int exponent = expNode->val;
            // TreeNode *coefNode = node->children[0]; // The coefficient

            // Multiply coefficient with exponent
            int newCoef = coeff * exponent;
            // printf("newcoef=%d",newCoef);

            if(exponent==2){
               printf(" %dx ", newCoef); // Apply the power rule
            }
            else{
               printf(" %dx^%d ", newCoef, exponent - 1); // Apply the power rule

            }
        }
    }

    // For "N" node: constants differentiate to 0 (no need to print anything)
    else if (strcmp(node->nodeName, "N") == 0 || isdigit(node->nodeName[0])) {
        // Constants differentiate to 0, no need to print anything
        return;
    }

    return;
}



int main()
{
   printf("Started Parsing...");
   yyparse();
   printf("\n");
   printf("Ended Parsing...");
   printf("\n");

   // Check if parsing was successful
   if (root)
   {
      // Print the parse tree starting from the root
      printf("The nodes at same indentation level are the siblings of same parent.\n");
      // printf("+++The Parse Tree formed is:\n");
      // printParseTree(root, 0);
      // printf("+++The Annotated Parse Tree Before is:\n");
      // printAnnotatedTree(root, 0);
   }
   else
   {
      printf("Parsing failed or no parse tree was generated.\n");
   }

   setatt(root);
   printf("\n");
   printf("+++The Annotated Parse Tree is:\n");
   printAnnotatedTree(root, 0);

   printf("\n+++ Printing the value of polynomial expression x=[-5,5]\n");
   for(int i=-5;i<=5;i++){
      int result=evalpoly(root,i);
      printf("+++ f(%d) = %d\n",i,result);
   }

   // printAnnotatedTree(root, 0);

   printf("\n+++ The derivative of the polynomial is:\n");
   printf("+++ f'(x) = ");
   printderivate(root,1);

   // Free the allocated memory for the parse tree
   // freeTree(root);


   return 0;
}
