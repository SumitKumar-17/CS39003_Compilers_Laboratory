#include <bits/stdc++.h>
#include "lex.yy.c"
#include <string>

using namespace std;

#define ADD 6
#define SUB 7
#define MUL 8
#define DIV 9
#define MOD 10

typedef struct _node {
    string name; 
    int tokenid;
    struct _node *next;
} node;

typedef struct _Input {
    string token;
    int tokenid;
    int inputIndexValue;
    struct _Input *next;
} Input;


// . Each node in the tree should
// store (i) a type (OP/ID/NUM), (ii) a union of an operator, a reference to an entry in T, and a reference to an entry in C, and (iii) two
// child pointers. If needed, you may additionally include parent pointers in the nodes
typedef struct TreeNode{
    struct TreeNode *left;
    struct TreeNode *right;
    string type;
    string op;
    string *id;
    string *num;
    int depth;
    TreeNode() : left(nullptr), right(nullptr), id(nullptr), num(nullptr) {}
}TreeNode;


typedef Input *input;
typedef node *symboltable;
typedef node *consttable;
typedef node *numtable;
int consttablesize = 0;

int yywrap(){
    return 1;
}

// Function to add inputs to the input list
input addinput(input I, string token, int tokenid,int inputIndex){
    Input *newNode = new Input();
    newNode->token = token;
    newNode->tokenid = tokenid;
    newNode->inputIndexValue = inputIndex;
    newNode->next = NULL;

    if (I == NULL) {
        return newNode; 
    }
    Input *p = I;
    while (p->next != NULL) {
        p = p->next;
    }

    p->next = newNode; 
    return I;
}

// Function to add numbers to the number table
numtable addnumtbl(numtable N, string num) {
    node *p = N;
    while (p) {
        p = p->next;
    }
    cout << "Adding new number: " << num << endl;
    p = new node();
    p->name = num;
    p->tokenid = NUM;
    p->next = N;
    return p;
}

// Function to add identifiers to the symbol table
symboltable addtbl(symboltable T, string id) {
    node *p = T;
    while (p) {
        if (p->name == id) {  // Check if the identifier already exists
            cout << "Identifier " << id << " already exists\n";
            return T;
        }
        p = p->next;
    }
    cout << "Adding new identifier: " << id << endl;
    p = new node();
    p->name = id;
    p->tokenid = ID;
    p->next = T;
    return p;
}

// Function to add numbers to the constant table
consttable addconst(consttable C, string num) {
    node *p = C;
    while (p) {
        if (p->name == num) {  // Check if the constant already exists
            cout << "Constant " << num << " already exists\n";
            return C;
        }
        p = p->next;
    }
    cout << "Adding new constant: " << num << endl;
    consttablesize++;
    p = new node();
    p->name = num;
    p->tokenid = NUM;
    p->next = C;
    return p;
}

void printInputTable(input I){
    while (I){
        cout << I->tokenid <<"(" << I->token << ")" << " ";
        cout<<"\n";
        I = I->next;
    }
    cout << '\n';
}

void printSymbolTable(symboltable T){
    while (T){
        cout << T->tokenid << "(" << T->name << ")" << " ";
        cout<<"\n";
        T = T->next;
    }
    cout << '\n';
}

void printConstTable(consttable C){
    while (C){
        cout << C->tokenid << "(" << C->name << ")" << " ";
        cout<<"\n";
        C = C->next;
    }
    cout << '\n';
}

void initParsingTable(string table[3][9]) {
    table[0][0] = "( OP ARG ARG )";  // EXPR -> ( OP ARG ARG ) when input is '('
    table[0][7] = "ID";             // EXPR -> ID when input is ID
    table[0][8] = "NUM";            // EXPR -> NUM when input is NUM

    table[1][2] = "+"; // OP -> + when input is '+'
    table[1][3] = "-"; // OP -> - when input is '-'
    table[1][4] = "*"; // OP -> * when input is '*'
    table[1][5] = "/"; // OP -> / when input is '/'
    table[1][6] = "%"; // OP -> % when input is '%'

    table[2][0] = "EXPR"; // ARG -> EXPR when input is '('
    table[2][7] = "ID";   // ARG -> ID when input is ID
    table[2][8] = "NUM";  // ARG -> NUM when input is NUM
}

Input *getInputAtIndex(Input *I, int index){
    while (I){
        if (I->inputIndexValue == index){
            return I;
        }
        I = I->next;
    }
    return NULL;
}

int findMidpoint(Input *I, int start, int end) {
    int par = 0;  // Counter for parentheses
    for (int i = start + 2; i <= end; ++i) {
        string token = getInputAtIndex(I, i)->token;
        if (token == "(") par++;
        else if (token == ")") par--;
        else if (par == 0 && (token == "+" || token == "-" || token == "*" || token == "/" || token == "%")) {
            return i;  // Found midpoint at an operator
        }
    }
    return -1;  // Error: Midpoint not found
}

TreeNode *parseInput(Input *I,int start,int end ,int depth){
    if(start>end)return NULL;
    string openBracket={"("};
    string closeBracket={")"};
    

     if (start == end) {
        TreeNode *root = new TreeNode();
        string temp = getInputAtIndex(I, start)->token;
        root->depth = depth;
        // cout<<temp<<endl;

        
         if ((temp[0] >= '0' && temp[0] <= '9') || temp[0] == '-'){
            root->type = "NUM";
            root->num = new string(temp);;
        }else if(temp=="+" || temp=="-" || temp=="*" || temp=="/" || temp=="%"){
            root->type = "OP";
            root->op =string(temp);
        }
        else {
            // cout<<"ID"<<endl;
            root->type = "ID";
            // cout<<temp<<endl;
            root->id = new string(temp);
        }

        root->left = NULL;
        root->right = NULL;
        return root;
    }
    
    if(getInputAtIndex(I,start)->token==openBracket && getInputAtIndex(I,end)->token==closeBracket){
        TreeNode *root = new TreeNode();
        root->depth = depth;
        root->type = "OP";
        root->op = getInputAtIndex(I, start + 1)->token;

        int mid;
        int par = 0;
        for (int i = start + 2; i < end; i++)
        {
            if (getInputAtIndex(I,i)->token==openBracket)
                par++;
            else if (getInputAtIndex(I,i)->token==closeBracket)
                par--;
             if (par == 0 ) {
                mid = i;
                break;
            }
        }

        if (mid == -1) {
            fprintf(stderr, "Failed to find midpoint for expression\n");
            exit(-1);
        }
        // cout<<"here"<<endl;
        root->left = parseInput(I, start + 2, mid, depth + 1);
        root->right = parseInput(I, mid + 1, end - 1, depth + 1);
        return root;
    }
    return NULL;
}



void printParseTree(TreeNode * root){
    if(root==NULL) {
        return;
    }
    if(root->left==NULL && root->right==NULL){
        if(root->type=="ID"){
            for(int i=1;i<root->depth;i++) cout<<"\t";
            cout<< "---->";
            cout<<"ID("<< *root->id <<")\n";
        }else if(root->type=="OP"){
            for(int i=1;i<root->depth;i++) cout<<"\t";
            cout<< "---->";
            cout<<"OP("<< root->op <<")\n";
        }
        else{
            for(int i=1;i<root->depth;i++) cout<<"\t";
            cout<< "---->";
            cout<<"NUM("<< *root->num <<")\n";
        }
        return;
    }
    for(int i=1;i<root->depth;i++) cout<<"\t";

    if(root->depth!=0) cout<<"---->";
    cout << "OP(" << root->op << ")\n";
    printParseTree(root->left);
    printParseTree(root->right);
}

int isFromGrammar(){
    return 1;
}

int Eval(TreeNode *root,consttable C,symboltable T){
    if(root==NULL) return 0;
    if (root->left == NULL && root->right == NULL) {
        if (root->type == "ID") {
            node *p = T;
            node *q = C;
            while (p && q ) {
                // cout<<"sumit"<<p->name;<<endl;
                // cout << "sumit"<< q->name<<" "<<p->name<<endl;
                    // string dfg="11";
                    //     int df =stoi(dfg);
                    //     cout<<df<<endl;
                // if (q->name == *root->id) {
                    return stoi(q->name);
                // }
                p = p->next;
                q = q->next;
            }
        }
        else  {
            return stoi(*root->num);
        }
    }
    int left = Eval(root->left,C,T);
    int right = Eval(root->right,C,T);
    int ans;
    if(root->op=="+"){
        ans = left + right;
    }else if(root->op=="-"){
        ans = left - right;
    }else if(root->op=="*"){
        ans = left * right;
    }else if(root->op=="/"){
        ans = left / right;
    }else if(root->op=="%"){
        ans = left % right;
    }
    // cout<<ans<< ")"<<endl;
    return ans;
}


void deleteTree(TreeNode *root){
    if(root==NULL) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main(){
    int nexttok;
    input I = NULL;
    symboltable T = NULL;
    consttable C = NULL;
    numtable N = NULL;
    int inputSize = 0;
    int op=0,cp=0;

    while ((nexttok = yylex())){
        if (op < cp)
        {
            cout << "**ERROR\n";
            exit(-1);
        }
        switch(nexttok){
            case LP: 
                // printf("Beginning of subexpression\n"); 
                I = addinput(I, "(", LP, inputSize);
                op++;
                inputSize++;
                break;
            case RP: 
                // printf("End of subexpression\n"); 
                I = addinput(I, ")", RP,inputSize);
                cp++;
                inputSize++;
                break;
            case NUM: 
                if(op!=cp){
                    // printf("Number: %s\n", yytext);
                    I = addinput(I, yytext, NUM,inputSize);
                    N=addnumtbl(N, string(yytext));
                    inputSize++;
                }
                if(op==cp){
                    // cout<<"Number outside of parentheses\n";
                    C = addconst(C, string(yytext));
                }
                break;
            case OP: 
                // printf("Operator: %s\n", yytext);
                if(strcmp(yytext, "+") == 0){
                    I = addinput(I, "+", ADD,inputSize);
                } else if(strcmp(yytext, "-") == 0){
                    I = addinput(I, "-", SUB,inputSize);
                } else if(strcmp(yytext, "*") == 0){
                    I = addinput(I, "*", MUL,inputSize);
                } else if(strcmp(yytext, "/") == 0){
                    I = addinput(I, "/", DIV,inputSize);
                } else if(strcmp(yytext, "%") == 0){
                    I = addinput(I, "%", MOD,inputSize);
                } else {
                    printf("Unknown operator\n");
                    exit(-1);
                }
                inputSize++;
                break;
            case ID: 
                T = addtbl(T, string(yytext));
                // printf("Identifier: %s\n", yytext);
                I = addinput(I, string(yytext), ID,inputSize);
                inputSize++;
                break;
            default: 
                printf("Unknown token\n"); 
                break;
        }
    }
    if (op != cp) {
        cout << "**ERROR: Unbalanced parentheses\n";
        exit(-1);
    }
    
    //print the input list
    // cout<<"\nThe Input Table is:\n";
    // printInputTable(I);

    //print the sybmol table
    cout<<"\nThe Symbol Table is:\n";
    printSymbolTable(T);

    //print the constant table
    cout<<"\nThe Constant Table is:\n";
    printConstTable(C);

    // Initialize the parsing table
    string parsingTable[3][9];
    initParsingTable(parsingTable);

    TreeNode * parseTree = parseInput(I,0,inputSize-1,0);

    if (parseTree == NULL) {
    cout << "Failed to construct parse tree\n";
    }

    if(isFromGrammar()==1){
        cout << '\n';
        cout << "Parsing is Successful\n";
        printParseTree(parseTree);

        if (consttablesize != 0)
        {
            cout << "Reading variable values fron the input\n";
            for (int i = 0; i < consttablesize; i++)
            {  //rofm the numtable and print the value
            //    printVariableValue(T, C);
            }
        }
        cout << "The value of the expression is ";
        cout << Eval(parseTree,C,T) << '\n';
        deleteTree(parseTree);
    }  


    // while (T) {
    //     node *temp = T;
    //     T = T->next;
    //     free(temp->name);
    //     free(temp);
    // }

    // while (C) {
    //     node *temp = C;
    //     C = C->next;
    //     free(temp->name);
    //     free(temp);
    // }

    return 0;
}
