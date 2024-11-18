#include <bits/stdc++.h>
#include "lex.yy.c"
using namespace std;

const int MAX = 210;
int yywrap()
{
    return 1;
}

struct Id
{
    string name;
    int value;
};

struct In
{
    string token;
    int tokenid;
};
In Input[MAX];
int insize;

Id IdTable[MAX];
int idsize;

int NumTable[MAX];
int numsize;

void addIDtable(string s)
{
    for (int i = 0; i < idsize; i++)
    {
        if (IdTable[i].name == s)
        {
            return;
        }
    }
    IdTable[idsize++].name = s;
}

void addNumtable(int s)
{
    NumTable[numsize++] = s;
}

void printIdtable()
{
    for (int i = 0; i < idsize; i++)
    {
        cout << IdTable[i].name << " ";
    }
    cout << '\n';
    for (int i = 0; i < idsize; i++)
    {
        cout << IdTable[i].value << " ";
    }
    cout << '\n';
}

void printNumtable()
{
    for (int i = 0; i < numsize; i++)
    {
        cout << NumTable[i] << " ";
    }
    cout << '\n';
}

struct Tree
{
    string opr;
    int val = 0;
    Tree *left;
    Tree *right;
    int depth;
};

int idindex(string s)
{
    for (int i = 0; i < idsize; i++)
    {
        if (IdTable[i].name == s)
        {
            return i;
        }
    }
    return -1;
}

int numindex(int s)
{
    for (int i = 0; i < numsize; i++)
    {
        if (NumTable[i] == s)
        {
            return i;
        }
    }
    return -1;
}

Tree *MakeTree(int l, int r, int depth)
{
    if (l > r)
        return NULL;
    if (l == r)
    {
        Tree *root = (Tree *)malloc(sizeof(Tree));
        string temp = Input[l].token;
        if ((temp[0] >= '0' && temp[0] <= '9') || temp[0] == '-')
        {
            root->opr = "NUM";
            root->depth = depth;
            root->val = numindex(stoll(temp));
        }
        else
        {
            root->opr = "ID";
            root->depth = depth;
            root->val = idindex(temp);
        }
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    if (Input[l].token == "(" && Input[r].token == ")")
    {
        Tree *root = (Tree *)malloc(sizeof(Tree));
        root->opr = Input[l + 1].token;
        root->depth = depth;
        int mid;
        int par = 0;
        for (int i = l + 2; i < r; i++)
        {
            if (Input[i].token == "(")
                par++;
            else if (Input[i].token == ")")
                par--;
            if (par == 0)
            {
                mid = i;
                break;
            }
        }
        root->left = MakeTree(l + 2, mid, depth + 1);
        root->right = MakeTree(mid + 1, r - 1, depth + 1);

        return root;
    }
    return NULL;
}

void Traverse(Tree *root)
{
    if (root->left == NULL && root->right == NULL)
    {
        cout << root->val << " ";
        return;
    }
    if (root == NULL)
        return;
    Traverse(root->left);
    cout << root->opr << " ";
    Traverse(root->right);
}

int Eval(Tree *root)
{
    if (root->right == NULL && root->right == NULL)
    {
        if (root->opr == "ID")
        {
            return IdTable[root->val].value;
        }
        else
        {
            return NumTable[root->val];
        }
    }
    if (root == NULL)
        return 0;
    int left = Eval(root->left);
    int right = Eval(root->right);
    int ans;
    if (root->opr == "*")
    {
        ans = left * right;
    }
    if (root->opr == "/")
    {
        ans = left / right;
    }
    if (root->opr == "+")
    {
        ans = left + right;
    }
    if (root->opr == "-")
    {
        ans = left - right;
    }
    if (root->opr == "%")
    {
        ans = left % right;
    }
    return ans;
}

void printTree(Tree *root)
{
    if (root->left == NULL && root->right == NULL)
    {
        if (root->opr == "ID")
        {
            for (int i = 1; i < root->depth; i++)
            {
                cout << "\t";
            }
            cout << "----->";
            cout << "ID(" << IdTable[root->val].name << ")\n";
        }
        else
        {
            for (int i = 1; i < root->depth; i++)
            {
                cout << "\t";
            }
            cout << "----->";
            cout << "NUM(" << NumTable[root->val] << ")\n";
        }
        return;
    }
    for (int i = 1; i < root->depth; i++)
    {
        cout << "\t";
    }
    if (root->depth != 0)
        cout << "----->";
    cout << "OP(" << root->opr << ")\n";
    printTree(root->left);
    printTree(root->right);
}

void deleteTree(Tree *root)
{
    if (root == NULL)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int isfromgrammer()
{
    stack<int> s;
    int iter = 0;
    s.push(0); // Initial state

    while (!s.empty())
    {
        int a = s.top();
        s.pop();

        // cout << "Processing state: " << a << ", iter: " << iter << '\n';

        if (a >= 3 && a <= 11) // Token IDs are between 3 and 11
        {
            if (iter < insize && a == Input[iter].tokenid)
            {
                // cout << "Matched token: " << Input[iter].token << '\n';
                iter++;
            }
            else
            {
                cout << "***ERROR: Unexpected token " << (iter < insize ? Input[iter].token : "N/A") << '\n';
                return 0;
            }
        }
        else
        {
            if (a == 0)
            {
                if (iter < insize && Input[iter].tokenid == 3) // Open Bracket
                {
                    s.push(4);
                    s.push(2);
                    s.push(2);
                    s.push(1);
                    s.push(3);
                }
                else
                {
                    cout << "***ERROR: Expected opening bracket\n";
                    return 0;
                }
            }
            else if (a == 1)
            {
                if (iter < insize && (Input[iter].tokenid >= 7 && Input[iter].tokenid <= 11))
                {
                    s.push(Input[iter].tokenid);
                }
                else
                {
                    cout << "***ERROR: Unexpected token or operator\n";
                    return 0;
                }
            }
            else if (a == 2)
            {
                if (iter < insize)
                {
                    if (Input[iter].tokenid == 3)
                    {
                        s.push(0);
                    }
                    else if (Input[iter].tokenid == 5)
                    {
                        s.push(5);
                    }
                    else if (Input[iter].tokenid == 6)
                    {
                        s.push(6);
                    }
                    else
                    {
                        cout << "***ERROR: Expected a valid token\n";
                        return 0;
                    }
                }
                else
                {
                    cout << "***ERROR: Unexpected end of input\n";
                    return 0;
                }
            }
        }
    }

    if (iter != insize)
    {
        cout << "***ERROR: Input not fully consumed\n";
        return 0;
    }
    return 1;
}

int main()
{
    int token;
    idsize = 0;
    numsize = 0;
    insize = 0;
    int iditer = 0;
    int cp = 0, op = 0;
    while ((token = yylex()))
    {
        if (op < cp)
        {
            cout << "**ERROR\n";
            exit(-1);
        }
        switch (token)
        {
        case OR:
            // cout << "Open Braket\n";
            Input[insize].tokenid = 3;
            Input[insize++].token = "(";
            op++;
            break;
        case CR:
            // cout << "Close Braket\n";
            Input[insize].tokenid = 4;
            Input[insize++].token = ")";
            cp++;
            break;
        case NUM:
            if (op != cp)
            {
                addNumtable(stoll(string(yytext)));
                Input[insize].tokenid = 6;
                Input[insize++].token = string(yytext);
            }
            if (op == cp)
            {
                IdTable[iditer++].value = stoll(string(yytext));
            }
            break;
        case ID:
            addIDtable(string(yytext));
            Input[insize].tokenid = 5;
            Input[insize++].token = string(yytext);
            break;
        case OP:
            // cout << "Operator:- " << yytext << '\n';
            if (string(yytext) == "+")
            {
                Input[insize].tokenid = 7;
            }
            if (string(yytext) == "-")
            {
                Input[insize].tokenid = 8;
            }
            if (string(yytext) == "*")
            {
                Input[insize].tokenid = 9;
            }
            if (string(yytext) == "/")
            {
                Input[insize].tokenid = 10;
            }
            if (string(yytext) == "%")
            {
                Input[insize].tokenid = 11;
            }
            Input[insize++].token = string(yytext);
            break;
        default:
            break;
        }
    }

    // for( int i=0; i<insize; i++){
    //     cout<<Input[i].token<<" "<<Input[i].tokenid<<'\n';
    // }

        Tree *root = MakeTree(0, insize - 1, 0);
    if (isfromgrammer()==1)
    {
        cout << '\n';
        cout << "Parsing is Successful\n";
        printTree(root);
        if (idsize != 0)
        {
            cout << "Reading variable values fron the input\n";
            for (int i = 0; i < idsize; i++)
            {
                cout << IdTable[i].name << ": " << IdTable[i].value << '\n';
            }
        }
        cout << "The value of the expression is ";
        cout << Eval(root) << '\n';
        deleteTree(root);
    }
    
}