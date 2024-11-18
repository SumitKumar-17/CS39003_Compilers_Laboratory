// #pragma once
#ifndef _DEF_H_
#define _DEF_H_

//A basic definition file for the lexer
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;

// The struct for ach of the particular symbol entry present in the symbol table
struct SymbolType;

// The struct for the symbol table. It contains the name of the table, the symbols present in the table, the parent table and the update function
struct SymbolTable;

// The struct for the symbol. It contains the name of the symbol, the size of the symbol, the offset of the symbol, the type of the symbol, the nested table, the initial value and whether the symbol is a function or not
struct Symbol;

//The struct for the Label. It contains the name of the label
struct Label;

//The struct of the Quad. It contains the operation, the arguments and the result
struct Quad;

//The struct for the Expression. It contains the symbol and the type of the expression.
struct Expression;

//The struct for the Array. It contains the type of the array, the symbol and the temporary symbol
struct Array;

//The struct for the Statement. It contains the next list.
struct Statement;


/*
	The struct for the SymbolType. 
	It contains the type of the symbol, the width of the symbol,
	 the array type of the symbol and the constructor for the symbol type
*/
struct SymbolType
{
	enum struct SymbolEnum
	{
		VOID,
		INT,
		FLOAT,
		CHAR,
		PTR,
		FUNC,
		ARRAY,
		BLOCK,
	} type{};

	size_t width{};
	SymbolType *array_type{};

	SymbolType(SymbolEnum, SymbolType * = nullptr, size_t = 1);
	string toString();
	size_t getSize();
};


/*
	The struct for the SymbolTable. 
	It contains the name of the table, the symbols present in the table, 
	the parent table and the update function. It also displays the nested tabl from where the fucntion is called.
*/
struct SymbolTable
{
	string name{};
	map<string, Symbol> symbols{};
	SymbolTable *parent{};

	SymbolTable(const string & = "NULL", SymbolTable * = nullptr);
	void update();
	Symbol *lookup(const string &);
	void print();
};

/*
	The struct for the Symbol. 
	It contains the name of the symbol, the size of the symbol, the offset of the symbol, 
	the type of the symbol, the nested table, the initial value and whether the symbol is a function or not.
*/
struct Symbol
{
	string name{};
	size_t size{}, offset{};
	SymbolType *type{};
	SymbolTable *nested_table{};
	string initial_value{};
	bool is_function{};

	// Symbol() = default;
	Symbol(const string & = "", SymbolType::SymbolEnum = SymbolType::SymbolEnum::VOID, const string & = "");
	Symbol *convert(SymbolType::SymbolEnum);
	Symbol *update(SymbolType *);
};


/*
	The struct for the Quads.
	It contains the operation, the arguments and the result.
*/
struct Quad
{
	string op{}, arg1{}, arg2{}, result{};

	/*
		Constructor for the Quad.
		There are two constructors for the Quad.
		One for the binary operations and the other for the unary operations.
	*/
	Quad(const string &, const string &, const string & = "=", const string & = "");
	Quad(const string &, int, const string & = "=", const string & = "");
	void print();
};


/*
	The constructor for the Expression.
	It contains the symbol and the type of the expression.
	The statemensa re there which have the nextlist,truelist and the falselist.
	nextlist=> The list of the next instructions.
	truelist=> The list of the true instructions, which are exeuted when true value of the expression is obtained.
	falselist=> The list of the false instructions, which are executed when the false value of the expression is obtained.
*/
struct Expression
{
	Symbol *symbol{};

	enum struct ExprEnum
	{
		NONBOOL,
		BOOL,
	} type{};

	// The list of nextlist,truelist,falselist
	list<size_t> true_list{}, false_list{}, next_list{};

	//These are the functions which are used to convert the expression to the integer and the boolean.
	void to_int();
	void to_bool();
};

/*
	The struct for the Array.
	It contains the type of the array, the symbol and the temporary symbol.
*/
struct Array
{
	enum struct ArrayEnum
	{
		OTHER,
		PTR,
		ARRAY,
	} type{};

	Symbol *symbol{}, *temp{};
	SymbolType *sub_array_type{};
};

/*
	The struct for the Statement.
	It contains the next list.
*/
struct Statement
{
	list<size_t> next_list{};
};

/* 
	The backpatching function .it fills out the list of the instructions, of which the address field is blank  in the goto intstructions.
*/
void backpatch(const list<size_t> &list_, size_t addr);

/* 
	This make s the lsit for each of the statements.
*/

list<size_t> make_list(size_t);
/* 
	This merges the list of the instructions,for which two conditions are to be merged as per the Syntax Directed Translation.
*/
list<size_t> merge_list(list<size_t> &first, list<size_t> second);

/*
	This is the function which is used to get the next instruction.
*/
size_t next_instruction();

/*
	The emit function which fives out the 3-address code.
*/
void emit(const string &, const string &, const string & = "", const string & = "");
void emit(const string &, const string &, int, const string & = "");

/*
	The function which is used to get the temporary symbol.
*/
Symbol *gentemp(SymbolType::SymbolEnum, const string & = "");

/*
	The function which is used to change the referenece of the table.
*/
void change_table(SymbolTable *);

/*
	This fucntion checks the type of the symbol.
*/
bool type_check(Symbol *&, Symbol *&);

/*
	External referces which are used in the lexer and the parser.
*/
extern vector<Quad *> quad_array;
extern SymbolTable *current_table, *global_table;
extern Symbol *current_symbol;
extern SymbolType::SymbolEnum current_type;
extern int table_count, temp_count;

int yylex();
int yyparse();

void yyerror(const string &);
void yyinfo(const string &);

// typedef struct TreeNode {
//     char* nodeName;     // Name/type of the node (e.g., "primary_expression")
//     int childCount;     // Number of children
//     int maxChildren;
//     struct TreeNode** children; // Array of pointers to children nodes
// } TreeNode;

// // Function to create a new node
// TreeNode* createNode(char* name, int childCount)  ;

// // Function to add a child to a node
// void addChild(TreeNode *parent, TreeNode *child);

// //Function to create leaf node
// TreeNode* createLeaf(char *name);

// // Function to free the memory allocated for the tree
// void freeTree(TreeNode *node) ;


// // Function to print the tree (for debugging)
// void printParseTree(TreeNode *node, int indent);

// int yywrap(void);

//This was a gloabl indent level pointer defined
// extern int indent_level;

extern char *yytext;
extern int yylineno;
// extern int yyleng;

//The root of the parse tree
// extern TreeNode *root;

#endif
