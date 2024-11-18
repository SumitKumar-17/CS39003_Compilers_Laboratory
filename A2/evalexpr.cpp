#include <bits/stdc++.h>
#include "lex.yy.c"
using namespace std;

#define ADD 6
#define SUB 7
#define MUL 8
#define DIV 9
#define MOD 10

const int MAXVALUE = 10000;

typedef struct _inputToken{
    string token;
    int tokenID;
}inputToken;

inputToken inputTbl[MAXVALUE];
int inputTblSize;

typedef struct _idVarToken{
    string name;
    int value;
}idVarToken;

idVarToken idTbl[MAXVALUE];
int idTblSize;

int numConstTbl[MAXVALUE];
int numConstTblSize;

typedef struct _treeNode {
    string typeToken;
    int value=0;
    int depth;
    _treeNode *leftChild;
    _treeNode * rightChild;
}treeNode;


int yywrap(){
    return 1;
}

void addInputTbl(int tokenID, string token){
    inputTbl[inputTblSize].token = token;
    inputTbl[inputTblSize].tokenID = tokenID;
    inputTblSize++;
}

void addIdTbl(int tokenID, string token){
   for(int i=0; i<idTblSize; i++){
       if(idTbl[i].name == token){
           return;
       }
   }
    idTbl[idTblSize].name = token;
    idTbl[idTblSize].value = 0;
    idTblSize++;
}

void printIdTbl(){
    cout<<"ID Table:\n";
    for(int i=0; i<idTblSize; i++){
        cout<<idTbl[i].name<<"("<<idTbl[i].value<<")"<<endl;
    }
    cout<<endl;
}

void printInputTbl(){
    cout<<"Input Table:\n";
    for(int i=0; i<inputTblSize; i++){
        cout<<inputTbl[i].tokenID<<"("<<inputTbl[i].token<<")"<<endl;
    }
    cout<<endl;
}

void printNumConstTbl(){
    cout<<"Number Constant Table:\n";
    for(int i=0; i<numConstTblSize; i++){
        cout<<numConstTbl[i]<<endl;
    }
    cout<<endl;
}

// void addIdTblValMap(int *idTblValueMap,int value){
//     idTbl[*idTblValueMap].value = value;
//     idTblValueMap++;
// }
int getIndexValueFromNumTbl(int s){
    for(int i=0;i<numConstTblSize;i++){
        if(numConstTbl[i]==s) return i;
    }
    return -1;
}

int getIndexValueFromIdTbl(string s){
    for(int i=0;i<idTblSize;i++){
        if(idTbl[i].name==s) return i;
    }
    return -1;
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

treeNode * makeParseTree(int start,int end,int depth){
    if(start>end) return NULL;
    if(start==end){
        treeNode *root=new treeNode();
        string tempVar=inputTbl[start].token;

        if((tempVar[0]>='0' && tempVar[0]<='9') ||tempVar[0]=='-'){
            root->typeToken="NUM";
            root->depth=depth;
            root->value=getIndexValueFromNumTbl(stoll(tempVar));
        }else{
            root->typeToken="ID";
            root->depth=depth;
            root->value=getIndexValueFromIdTbl(tempVar);
        }
        root->leftChild=NULL;
        root->rightChild=NULL;
        return root;
    }

    if(inputTbl[start].token=="(" && inputTbl[end].token==")"){
        treeNode * root=new treeNode();
        root->typeToken=inputTbl[start+1].token;
        root->depth=depth;
        int mid;
        int paren=0;

        for(int i=start+2;i<end;i++){
            if(inputTbl[i].token=="(") paren++;
            else if (inputTbl[i].token==")") paren--;
            if(paren==0){
                mid=i;
                break;
            }
        }

        root->leftChild=makeParseTree(start+2,mid,1+depth);
        root->rightChild=makeParseTree(mid+1,end-1,1+depth);
        return root;
    }
    return NULL;
}

void printParseTree(treeNode * root){
    if(root->leftChild==NULL && root->rightChild==NULL){
        if(root->typeToken=="ID"){
            for(int i=1;i<root->depth;i++) cout <<"\t";
            cout<<"----->";
            cout<<"ID("<<idTbl[root->value].name<<")"<<endl;
        }
        else {
            for(int i=1;i<root->depth;i++) cout <<"\t";
            cout<<"----->";
            cout<<"NUM("<<numConstTbl[root->value]<<")"<<endl;
        }
        return;
    }

    for(int i=1;i<root->depth;i++) cout <<"\t";
    if(root->depth!=0) cout<<"----->";
    cout<<"OP("<<root->typeToken<<")"<<endl;
    printParseTree(root->leftChild);
    printParseTree(root->rightChild);
}

void deleteTree(treeNode* root){
    if(root==NULL) return;
    deleteTree(root->leftChild);
    deleteTree(root->rightChild);
    delete root;
}

int evaluateParseTree(treeNode * root){
    if(root==NULL) return 0;
    if(root->leftChild==NULL && root->rightChild==NULL){
        if(root->typeToken=="ID") return idTbl[root->value].value;
        else return numConstTbl[root->value];
    }
    int leftChild=evaluateParseTree(root->leftChild);
    int rightChild=evaluateParseTree(root->rightChild);
    int calc;
    
    if(root->typeToken=="*") calc=leftChild*rightChild;
    if(root->typeToken=="+") calc=leftChild+rightChild;
    if(root->typeToken=="-") calc=leftChild-rightChild;
    if(root->typeToken=="/") calc=leftChild/rightChild;
    if(root->typeToken=="%") calc=leftChild%rightChild;

    return calc;
}

bool checkSyntax(){
    stack<int>parseStack;
    int index=0;
    parseStack.push(12);
    //0 is not any tokentype ,1 and 2 are parenthesis , 3 to 10 are tokentypes of Num,ID,OP(ADD,SUB,MUL,DIV,MOD)
    
    //The M(A,a) table is defined here as a 2D array
    //12=>0 =>EXPR detected
    //13=>1 =>OP detected
    //14=>2 =>ARG deteted
    // EXPR(12) ⟶ ( OP ARG ARG )
    // OP(13) ⟶ + | – | * | / | %
    // ARG(14) ⟶ ID | NUM | EXPR

    while(!parseStack.empty()){
        int stackTop=parseStack.top();
        parseStack.pop();

        if(stackTop>=1 && stackTop <=10){
            if(index<inputTblSize && stackTop==inputTbl[index].tokenID){
                index++;
            }else{
                cout << "*** Error:Invalid token found at the place of " << (index < inputTblSize ? inputTbl[index].token : "not defined character") << '\n';
                return false;
            }
        }else{
            if(stackTop==12){
                if(index<inputTblSize && inputTbl[index].tokenID==OB){
                    parseStack.push(CB);
                    parseStack.push(14);
                    parseStack.push(14);
                    parseStack.push(13);
                    parseStack.push(OB);
                   
                }else
                {
                    cout << "***Error: Expected opening bracket\n";
                    return false;
                }
            } else if (stackTop == 13){
                if (index < inputTblSize && (inputTbl[index].tokenID >= ADD && inputTbl[index].tokenID <= MOD))
                {
                    parseStack.push(inputTbl[index].tokenID);
                }
                else
                {
                    cout << "*** Error: Operator expected in place of "<<inputTbl[index].token<<"\n";
                    return false;
                }
            }else if (stackTop == 14){
                if (index < inputTblSize){
                    if (inputTbl[index].tokenID == OB)
                    {
                        parseStack.push(12);
                    }
                    else if (inputTbl[index].tokenID == ID)
                    {
                        parseStack.push(ID);
                    }
                    else if (inputTbl[index].tokenID == NUM)
                    {
                        parseStack.push(NUM);
                    }
                    else
                    {
                        cout << "*** Error: ID/NUM/LP expected in place of "<<inputTbl[index].token<<"\n";
                        return false;
                    }
                }
                else
                {
                    cout << "***Error: Unexpected end of InputTbl\n";
                    return false;
                }
            }
        }
            
    }

    if (index != inputTblSize)
    {
        cout << "***Error: InputTbl not fully consumed\n";
        return false;
    }
    return true;
}

int main()
{
    int token;

    inputTblSize = 0;
    idTblSize = 0;
    numConstTblSize = 0;

    int closeParen=0;
    int openParen=0;
    int idTblValueMap=0;
    while((token=yylex())){
        if(openParen<closeParen){
            cout<<"***Error: Parentheses are not balanced"<<endl;
            exit(1);
        }
        switch(token){
            case OB:
                // cout<<"Token is open parenthesis "<<"("<<yytext<<")"<<endl;
                openParen++;
                addInputTbl(OB, string(yytext));
                break;
            case CB:
                // cout<<"Token is close parenthesis "<<"("<<yytext<<")"<<endl;
                closeParen++;
                addInputTbl(CB, string(yytext));
                break;
            case NUM:
                if(openParen!=closeParen){
                    // cout<<"Token is number "<<"("<<yytext<<")"<<endl;
                    numConstTbl[numConstTblSize] = stoll(string(yytext));
                    addInputTbl(NUM, string(yytext));
                    numConstTblSize++;
                }
                if(openParen==closeParen){
                    // cout<<"Token is number as variable values "<<"("<<yytext<<")"<<endl;
                    // addIdTblValMap(&idTblValueMap,stoll(string(yytext)));
                    idTbl[idTblValueMap].value = stoll(string(yytext));
                    idTblValueMap++;
                }
                break;
            case ID:
                // cout<<"Token is ID "<<"("<<yytext<<")"<<endl;
                addIdTbl(ID,string(yytext));
                addInputTbl(ID, string(yytext));
                break;
            case OP:
                // cout<<"Token is operator "<<"("<<yytext<<")"<<endl;
                if(string(yytext)=="+"){
                    addInputTbl(ADD, string(yytext));
                }
                else if(string(yytext)=="-"){
                    addInputTbl(SUB, string(yytext));
                }
                else if(string(yytext)=="*"){
                    addInputTbl(MUL, string(yytext));
                }
                else if(string(yytext)=="/"){
                    addInputTbl(DIV, string(yytext));
                }
                else if(string(yytext)=="%"){
                    addInputTbl(MOD, string(yytext));
                }
                else {
                    cout<<"*** Error: Invalid operator "<<yytext<<" found"<<endl;
                    exit(1);
                }
                break;
            default:
                break;
        }
    }
    
    //Functions to print all the tables created
    // printIdTbl();
    // printInputTbl();
    // printNumConstTbl();

    // Initialize the parsing table
    string parsingTable[3][9];
    initParsingTable(parsingTable);
    treeNode *parseTree=makeParseTree(0,inputTblSize-1,0);
    if(checkSyntax()){
        cout<<"\n";
        cout<<"Parsing is Successful\n";
        printParseTree(parseTree);
        if(idTblSize!=0){
            cout << "Reading variable values fron the InputTbl\n";
            for (int i = 0; i < idTblSize; i++){
                cout<<idTbl[i].name <<" = "<<idTbl[i].value<<endl;
            }
        }
        cout << "The value of the expression is ";
        cout <<evaluateParseTree(parseTree) << '\n';
        deleteTree(parseTree);
    }


    return 0;
    
}