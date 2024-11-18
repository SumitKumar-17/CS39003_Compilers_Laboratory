#include <bits/stdc++.h>
#include "lex.yy.c"
using namespace std;

//function for list. file 
int yywrap(){
    return 1;
}
typedef struct _node {
    string name;
    struct _node* next;
} node;

typedef node* symboltable;
typedef node* consttable;

struct TreeNode {
    TreeNode* left;
    TreeNode* right;
    string type;  // OP/ID/NUM
    string op;
    string* id;
    string* num;

    TreeNode() : left(nullptr), right(nullptr), id(nullptr), num(nullptr) {}
};

TreeNode* createTreeNode(string type, string value, TreeNode* left = nullptr, TreeNode* right = nullptr) {
    TreeNode* newNode = new TreeNode();
    newNode->type = type;
    if (type == "OP") newNode->op = value;
    else if (type == "ID") {
        newNode->id = new string(value);
    }
    else if (type == "NUM") {
        newNode->num = new string(value);
    }
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

// Function to add identifiers to the symbol table
symboltable addtbl(symboltable T, string id) {
    node* p = T;
    while (p) {
        if (p->name == id) {
            return T;
        }
        p = p->next;
    }
    p = new node();
    p->name = id;
    p->next = T;
    return p;
}

// Function to add numbers to the constant table
consttable addconst(consttable C, string num) {
    node* p = C;
    while (p) {
        if (p->name == num) {
            return C;
        }
        p = p->next;
    }
    p = new node();
    p->name = num;
    p->next = C;
    return p;
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

int getTokenIndex(string token) {
    if (token == "(") return 0;
    if (token == ")") return 1;
    if (token == "+") return 2;
    if (token == "-") return 3;
    if (token == "*") return 4;
    if (token == "/") return 5;
    if (token == "%") return 6;
    if (isalpha(token[0]) || token[0] == '_') return 7; // ID
    if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) return 8; // NUM
    return -1; // Invalid token
}

void printParseTree(TreeNode* root, int depth = 0) {
    if (!root) return;
    for (int i = 0; i < depth; ++i) cout << " ";
    cout << root->type;
    if (root->type == "OP") cout << " (" << root->op << ")";
    else if (root->type == "ID") cout << " (" << *root->id << ")";
    else if (root->type == "NUM") cout << " (" << *root->num << ")";
    cout << "\n";
    printParseTree(root->left, depth + 2);
    printParseTree(root->right, depth + 2);
}

TreeNode* parse() {
    stack<string> stk;
    stack<string> parseStack;
    stack<TreeNode*> parseTreeStack;
    string currentToken;
    int tokenIndex = 0;
    string parsingTable[3][9];

    // Initialize the parsing table
    initParsingTable(parsingTable);

    // Read tokens and push them onto the stack
    while (true) {
        int nextok = yylex();
        if (nextok == 0) {
            stk.push("$"); // End of input marker
            break;
        }
        switch (nextok) {
            case LP:
                currentToken = "(";
                break;
            case RP:
                currentToken = ")";
                break;
            case NUM:
                currentToken = string(yytext);
                break;
            case OP:
                currentToken = string(yytext);
                break;
            case ID:
                currentToken = string(yytext);
                break;
            default:
                continue;
        }
        stk.push(currentToken);
    }

    stk.push("$"); // End of input marker

    // Initialize parsing stack
    parseStack.push("$");  // End of input marker
    parseStack.push("EXPR");

    while (!parseStack.empty()) {
        string top = parseStack.top();
        if (stk.empty()) {
            cout << "Unexpected end of input\n";
            return nullptr;
        }
        string currentToken = stk.top();

        if (top == currentToken) {
            parseStack.pop();
            stk.pop();
            if (top == "ID") {
                TreeNode* idNode = createTreeNode("ID", currentToken);
                parseTreeStack.push(idNode);
            } else if (top == "NUM") {
                TreeNode* numNode = createTreeNode("NUM", currentToken);
                parseTreeStack.push(numNode);
            } else if (top == "OP") {
                TreeNode* opNode = createTreeNode("OP", currentToken);
                parseTreeStack.push(opNode);
            } else if (top == "$") {
                break;
            }
        } else if (top == "EXPR" || top == "OP" || top == "ARG") {
            int row, col;
            if (top == "EXPR") row = 0;
            else if (top == "OP") row = 1;
            else if (top == "ARG") row = 2;

            col = getTokenIndex(currentToken);
            if (col == -1) {
                cout << "Invalid token: " << currentToken << "\n";
                return nullptr;
            }

            string production = parsingTable[row][col];
            if (production.empty()) {
                cout << "Syntax error\n";
                return nullptr;
            }

            parseStack.pop();
            istringstream iss(production);
            string token;
            stack<string> tempStack;
            while (iss >> token) {
                tempStack.push(token);
            }
            while (!tempStack.empty()) {
                parseStack.push(tempStack.top());
                tempStack.pop();
            }
        } else {
            cout << "Unknown non-terminal " << top << "\n";
            return nullptr;
        }
    }

    if (stk.top() == "$") {
        return parseTreeStack.top();
    } else {
        cout << "Unexpected end of input\n";
        return nullptr;
    }
}


int main() {
    TreeNode* parseTree = parse();
    if (parseTree) {
        cout << "Parse Tree:\n";
        printParseTree(parseTree);
    } else {
        cout << "Error in parsing.\n";
    }
    return 0;
}
