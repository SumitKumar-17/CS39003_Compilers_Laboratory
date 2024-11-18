%{
    #include "tinyC2_22CS30056_22CS30015.h"
    #include <stdio.h>

    #include <stdlib.h>
    #include <string.h>

    TreeNode *root;
%}

%union{
    char identifierVal[1024];
    int intVal;
    float floatVal;
    char charVal[1024];
    char stringVal[1024];

    TreeNode *node;
}


%type <node> translation_unit external_declaration function_definition declaration_specifiers declaration_specifiers_opt compound_statement declaration_list declaration jump_statement expression_opt statement expression iteration_statement selection_statement expression_statement block_item block_item_list block_item_list_opt labeled_statement constant_expression designator designator_list designation designation_opt initializer_list initializer assignment_expression specifier_qualifier_list type_name  identifier_list declarator parameter_declaration parameter_list parameter_type_list type_qualifier_list type_qualifier type_qualifier_list_opt pointer identifier_list_opt assignment_expression_opt direct_declarator function_specifier storage_class_specifier init_declarator init_declarator_list init_declarator_list_opt assignment_operator unary_expression conditional_expression logical_OR_expression logical_AND_expression AND_expression inclusive_OR_expression exclusive_OR_expression equality_expression relational_expression shift_expression additive_expression multiplicative_expression cast_expression postfix_expression argument_expression_list argument_expression_list_opt primary_expression declaration_list_opt pointer_opt specifier_qualifier_list_opt  type_specifier unary_operator


%token AUTO
%token ENUM
%token RESTRICT
%token UNSIGNED
%token BREAK
%token EXTERN
%token RETURN
%token VOID
%token CASE
%token FLOAT
%token SHORT
%token VOLATILE
%token CHAR
%token FOR
%token SIGNED
%token WHILE
%token CONST
%token GOTO
%token SIZEOF
%token _BOOL
%token CONTINUE
%token IF
%token STATIC
%token _COMPLEX
%token DEFAULT
%token INLINE
%token STRUCT
%token _IMAGINARY
%token DO
%token INT
%token SWITCH
%token DOUBLE
%token LONG
%token TYPEDEF
%token ELSE
%token REGISTER
%token UNION

%token<identifierVal> IDENTIFIER
%token<intVal> INTEGER_CONSTANT
%token<floatVal> FLOAT_CONSTANT
%token<charVal> CHAR_CONSTANT
%token<stringVal> STRING_LITERAL

%token ELLIPSIS
%token LEFT_SHIFT_ASSIGNMENT
%token RIGHT_SHIFT_ASSIGNMENT
%token INCREMENT
%token DECREMENT
%token ARROW
%token LEFT_SHIFT
%token RIGHT_SHIFT
%token STAR_ASSIGNMENT
%token FRONT_SLASH_ASSIGNMENT
%token MODULO_ASSIGNMENT
%token PLUS_ASSIGNMENT
%token MINUS_ASSIGNMENT
%token LESS_THAN_EQUAL_TO
%token GREATER_THAN_EQUAL_TO
%token EQUAL_TO
%token LOGICAL_AND
%token LOGICAL_OR
%token NOT_EQUAL_TO
%token BITWISE_AND_ASSIGNMENT
%token BITWISE_XOR_ASSIGNMENT
%token BITWISE_OR_ASSIGNMENT
%token LEFT_SQUARE_BRACKET
%token RIGHT_SQUARE_BRACKET
%token FRONT_SLASH
%token QUESTION_MARK
%token ASSIGNMENT
%token COMMA
%token BITWISE_AND
%token BITWISE_OR
%token BITWISE_XOR
%token LEFT_PARENTHESIS
%token RIGHT_PARENTHESIS
%token LEFT_BRACE
%token RIGHT_BRACE
%token DOT
%token STAR
%token PLUS
%token MINUS
%token TILDA
%token EXCLAMATION
%token MODULO
%token LESS_THAN
%token GREATER_THAN
%token COLON
%token SEMICOLON
%token HASH

%nonassoc RIGHT_PARENTHESIS
%nonassoc ELSE

%start  translation_unit

/* all the grammar rules go here */

%%
/*All the Rules for 1)Expressions */
primary_expression:
	IDENTIFIER
		{
			// yyinfo("primary_expression -> IDENTIFIER"); printf("IDENTIFIER = `%s`\n", $1);
			$$ = createNode("primary_expression", 1);
            addChild($$, createLeaf($1));
		}
	| INTEGER_CONSTANT
		{
			// yyinfo("primary_expression -> INTEGER_CONSTANT"); printf("INTEGER_CONSTANT = `%d`\n", $1);
			$$ = createNode("primary_expression", 1);
			char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
		}
	| FLOAT_CONSTANT
		{
			// yyinfo("primary_expression -> FLOATING_CONSTANT"); printf("FLOAT_CONSTANT = `%f`\n", $1);
			$$ = createNode("primary_expression", 1);
			char buf[1024];
			sprintf(buf, "%f", $1);
			addChild($$, createLeaf(buf));
		}
	| CHAR_CONSTANT
		{
			// yyinfo("primary_expression -> CHAR_CONSTANT"); printf("CHARACTER_CONSTANT = `%s`\n", $1);
			$$ = createNode("primary_expression", 1);
            addChild($$, createLeaf($1));
		}
	| STRING_LITERAL
		{
			// yyinfo("primary_expression -> STRING_LITERAL"); printf("STRING_LITERAL = `%s`\n", $1);
			$$ = createNode("primary_expression", 1);
            addChild($$, createLeaf($1));
		}
	| LEFT_PARENTHESIS expression RIGHT_PARENTHESIS
		{
			// yyinfo("primary_expression -> ( expression )");
			$$ = createNode("primary_expression", 3);
            addChild($$, createLeaf("("));
            addChild($$, $2);
            addChild($$, createLeaf(")"));

		}
	;

postfix_expression:
	primary_expression
		{
			// yyinfo("postfix_expression -> primary_expression");
			$$ = createNode("postfix_expression", 1);
            addChild($$, $1);
		}
	| postfix_expression LEFT_SQUARE_BRACKET expression RIGHT_SQUARE_BRACKET
		{
			// yyinfo("postfix_expression -> postfix_expression [ expression ]");
            $$ = createNode("postfix_expression", 4);
            addChild($$, $1);
            addChild($$, createLeaf("["));
            addChild($$, $3);
            addChild($$, createLeaf("]"));

		}
	| postfix_expression LEFT_PARENTHESIS argument_expression_list_opt RIGHT_PARENTHESIS
		{
			// yyinfo("postfix_expression -> postfix_expression ( argument_expression_list_opt )");
			$$ = createNode("postfix_expression", 3);
			addChild($$, $1);
            addChild($$, createLeaf("("));
            addChild($$, $3);
            addChild($$, createLeaf(")"));
		}
	| postfix_expression DOT IDENTIFIER
		{
			// yyinfo("postfix_expression -> postfix_expression . IDENTIFIER"); printf("IDENTIFIER = `%s`\n", $3);
            $$ = createNode("postfix_expression", 3);
            addChild($$, $1);
            addChild($$, createLeaf("."));
            addChild($$, createLeaf($3));
		}
	| postfix_expression ARROW IDENTIFIER
		{
			// yyinfo("postfix_expression -> postfix_expression -> IDENTIFIER"); printf("IDENTIFIER = `%s`\n", $3);
            $$ = createNode("postfix_expression", 3);
            addChild($$, $1);
            addChild($$, createLeaf("->"));
            addChild($$, createLeaf($3));
		}
	| postfix_expression INCREMENT
		{
			// yyinfo("postfix_expression -> postfix_expression ++");
            $$ = createNode("postfix_expression", 2);
            addChild($$, $1);
            addChild($$, createLeaf("++"));
		}
	| postfix_expression DECREMENT
		{
			// yyinfo("postfix_expression -> postfix_expression --");
            $$ = createNode("postfix_expression", 2);
            addChild($$, $1);
            addChild($$, createLeaf("--"));
		}
	| LEFT_PARENTHESIS type_name RIGHT_PARENTHESIS LEFT_BRACE initializer_list RIGHT_BRACE
		{
			// yyinfo("postfix_expression -> ( type_name ) { initializer_list }");
            $$ = createNode("postfix_expression", 6);
            addChild($$, createLeaf("("));
            addChild($$, $2);
            addChild($$, createLeaf(")"));
            addChild($$, createLeaf("{"));
            addChild($$, $5);
            addChild($$, createLeaf("}"));
        }
	| LEFT_PARENTHESIS type_name RIGHT_PARENTHESIS LEFT_BRACE initializer_list COMMA RIGHT_BRACE
		{
			// yyinfo("postfix_expression -> ( type_name ) { initializer_list , }");
			$$ = createNode("postfix_expression", 7);
			addChild($$, createLeaf("("));
			addChild($$, $2);
			addChild($$, createLeaf(")"));
			addChild($$, createLeaf("{"));
			addChild($$, $5);
			addChild($$, createLeaf(","));
			addChild($$, createLeaf("}"));
		}
	;

argument_expression_list_opt:
	argument_expression_list
		{
			// yyinfo("argument_expression_list_opt -> argument_expression_list");
			$$ = createNode("argument_expression_list_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("argument_expression_list_opt -> epsilon");
			$$ = createNode("argument_expression_list_opt", 0);
		}
	;

argument_expression_list:
	assignment_expression
		{
			// yyinfo("argument_expression_list -> assignment_expression");
			$$ = createNode("argument_expression_list", 1);
			addChild($$, $1);
		}
	| argument_expression_list COMMA assignment_expression
		{
			// yyinfo("argument_expression_list -> argument_expression_list , assignment_expression");
			$$ = createNode("argument_expression_list", 3);
			addChild($$, $1);
			addChild($$, createLeaf(","));
			addChild($$, $3);
		}
	;

unary_expression:
	postfix_expression
		{
			// yyinfo("unary_expression -> postfix_expression");
			$$ = createNode("unary_expression", 1);
			addChild($$, $1);
		}
	| INCREMENT unary_expression
		{
			// yyinfo("unary_expression -> ++ unary_expression");
			$$ = createNode("unary_expression", 2);
			addChild($$, createLeaf("++"));
			addChild($$, $2);
		}
	| DECREMENT unary_expression
		{
			// yyinfo("unary_expression -> -- unary_expression");
			$$ = createNode("unary_expression", 2);
			addChild($$, createLeaf("--"));
			addChild($$, $2);
		}
	| unary_operator cast_expression
		{
			// yyinfo("unary_expression -> unary_operator cast_expression");
			$$ = createNode("unary_expression", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	| SIZEOF unary_expression
		{
			// yyinfo("unary_expression -> sizeof unary_expression");
			$$ = createNode("unary_expression", 2);
			addChild($$, createLeaf("sizeof"));
			addChild($$, $2);
		}
	| SIZEOF LEFT_PARENTHESIS type_name RIGHT_PARENTHESIS
		{
			// yyinfo("unary_expression -> sizeof ( type_name )");
			$$ = createNode("unary_expression", 4);
			addChild($$, createLeaf("sizeof"));
			addChild($$, createLeaf("("));
			addChild($$, $3);
			addChild($$, createLeaf(")"));
		}
	;

unary_operator:
	BITWISE_AND
		{
			// yyinfo("unary_operator -> &");
			$$=createNode("unary_operator", 1);
			addChild($$, createLeaf("&"));
		}
	| STAR
		{
			// yyinfo("unary_operator -> *");
			$$=createNode("unary_operator", 1);
			addChild($$, createLeaf("*"));
		}
	| PLUS
		{
			// yyinfo("unary_operator -> +");
			$$=createNode("unary_operator", 1);
			addChild($$, createLeaf("+"));
		}
	| MINUS
		{
			// yyinfo("unary_operator -> -");
			$$=createNode("unary_operator", 1);
			addChild($$, createLeaf("-"));
		}
	| TILDA
		{
			// yyinfo("unary_operator -> ~");
			$$=createNode("unary_operator", 1);
			addChild($$, createLeaf("~"));
		}
	| EXCLAMATION
		{
			// yyinfo("unary_operator -> !");
			$$=createNode("unary_operator", 1);
			addChild($$, createLeaf("!"));
		}
	;

cast_expression:
	unary_expression
		{
			// yyinfo("cast_expression -> unary_expression");
			$$ = createNode("cast_expression", 1);
			addChild($$, $1);

		}
	| LEFT_PARENTHESIS type_name RIGHT_PARENTHESIS cast_expression
		{
			// yyinfo("cast_expression -> ( type_name ) cast_expression");
			$$ = createNode("cast_expression", 4);
			addChild($$, createLeaf("("));
			addChild($$, $2);
			addChild($$, createLeaf(")"));
			addChild($$, $4);
		}
	;

multiplicative_expression:
	cast_expression
		{
			// yyinfo("multiplicative_expression -> cast_expression");
			$$ = createNode("multiplicative_expression", 1);
			addChild($$, $1);
		}
	| multiplicative_expression STAR cast_expression
		{
			// yyinfo("multiplicative_expression -> multiplicative_expression * cast_expression");
			$$ = createNode("multiplicative_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("*"));
			addChild($$, $3);
		}
	| multiplicative_expression FRONT_SLASH cast_expression
		{
			// yyinfo("multiplicative_expression -> multiplicative_expression / cast_expression");
			$$ = createNode("multiplicative_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("/"));
			addChild($$, $3);
		}
	| multiplicative_expression MODULO cast_expression
		{
			// yyinfo("multiplicative_expression -> multiplicative_expression % cast_expression");
			$$ = createNode("multiplicative_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("%"));
			addChild($$, $3);
		}
	;

additive_expression:
	multiplicative_expression
		{
			// yyinfo("additive_expression -> multiplicative_expression");
			$$ = createNode("additive_expression", 1);
			addChild($$, $1);
		}
	| additive_expression PLUS multiplicative_expression
		{
			// yyinfo("additive_expression -> additive_expression + multiplicative_expression");
			$$ = createNode("additive_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("+"));
			addChild($$, $3);
		}
	| additive_expression MINUS multiplicative_expression
		{
			// yyinfo("additive_expression -> additive_expression - multiplicative_expression");
			$$ = createNode("additive_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("-"));
			addChild($$, $3);
		}
	;

shift_expression:
	additive_expression
		{
			// yyinfo("shift_expression -> additive_expression");
			$$ = createNode("shift_expression", 1);
			addChild($$, $1);
		}
	| shift_expression LEFT_SHIFT additive_expression
		{
			// yyinfo("shift_expression -> shift_expression << additive_expression");
			$$ = createNode("shift_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("<<"));
			addChild($$, $3);
		}
	| shift_expression RIGHT_SHIFT additive_expression
		{
			// yyinfo("shift_expression -> shift_expression >> additive_expression");
			$$ = createNode("shift_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf(">>"));
			addChild($$, $3);
		}
	;

relational_expression:
	shift_expression
		{
			// yyinfo("relational_expression -> shift_expression");
			$$ = createNode("relational_expression", 1);
			addChild($$, $1);
		}
	| relational_expression LESS_THAN shift_expression
		{
			// yyinfo("relational_expression -> relational_expression < shift_expression");
			$$ = createNode("relational_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("<"));
			addChild($$, $3);
		}
	| relational_expression GREATER_THAN shift_expression
		{
			// yyinfo("relational_expression -> relational_expression > shift_expression");
			$$ = createNode("relational_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf(">"));
			addChild($$, $3);
		}
	| relational_expression LESS_THAN_EQUAL_TO shift_expression
		{
			// yyinfo("relational_expression -> relational_expression <= shift_expression");
			$$ = createNode("relational_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("<="));
			addChild($$, $3);
		}
	| relational_expression GREATER_THAN_EQUAL_TO shift_expression
		{
			// yyinfo("relational_expression -> relational_expression >= shift_expression");
			$$ = createNode("relational_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf(">="));
			addChild($$, $3);
		}
	;

equality_expression:
	relational_expression
		{
			// yyinfo("equality_expression -> relational_expression");
			$$ = createNode("equality_expression", 1);
			addChild($$, $1);
		}
	| equality_expression EQUAL_TO relational_expression
		{
			// yyinfo("equality_expression -> equality_expression == relational_expression");
			$$ = createNode("equality_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("=="));
			addChild($$, $3);
		}
	| equality_expression NOT_EQUAL_TO relational_expression
		{
			// yyinfo("equality_expression -> equality_expression != relational_expression");
			$$ = createNode("equality_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("!="));
			addChild($$, $3);
		}
	;

AND_expression:
	equality_expression
		{
			// yyinfo("AND_expression -> equality_expression");
			$$ = createNode("AND_expression", 1);
			addChild($$, $1);
		}
	| AND_expression BITWISE_AND equality_expression
		{
			// yyinfo("AND_expression -> AND_expression & equality_expression");
			$$ = createNode("AND_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("&"));
			addChild($$, $3);
		}
	;

exclusive_OR_expression:
	AND_expression
		{
			// yyinfo("exclusive_OR_expression -> AND_expression");
			$$ = createNode("exclusive_OR_expression", 1);
			addChild($$, $1);
		}
	| exclusive_OR_expression BITWISE_XOR AND_expression
		{
			// yyinfo("exclusive_OR_expression -> exclusive_OR_expression ^ AND_expression");
			$$ = createNode("exclusive_OR_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("^"));
			addChild($$, $3);
		}
	;

inclusive_OR_expression:
	exclusive_OR_expression
		{
			// yyinfo("inclusive_OR_expression -> exclusive_OR_expression");
			$$ = createNode("inclusive_OR_expression", 1);
			addChild($$, $1);
		}
	| inclusive_OR_expression BITWISE_OR exclusive_OR_expression
		{
			// yyinfo("inclusive_OR_expression -> inclusive_OR_expression | exclusive_OR_expression");
			$$ = createNode("inclusive_OR_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("|"));
			addChild($$, $3);
		}
	;

logical_AND_expression:
	inclusive_OR_expression
		{
			// yyinfo("logical_AND_expression -> inclusive_OR_expression");
			$$ = createNode("logical_AND_expression", 1);
			addChild($$, $1);
		}
	| logical_AND_expression LOGICAL_AND inclusive_OR_expression
		{
			// yyinfo("logical_AND_expression -> logical_AND_expression && inclusive_OR_expression");
			$$ = createNode("logical_AND_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("&&"));
			addChild($$, $3);

		}
	;

logical_OR_expression:
	logical_AND_expression
		{
			// yyinfo("logical_OR_expression -> logical_AND_expression");
			$$ = createNode("logical_OR_expression", 1);
			addChild($$, $1);
		}
	| logical_OR_expression LOGICAL_OR logical_AND_expression
		{
			// yyinfo("logical_OR_expression -> logical_OR_expression || logical_AND_expression");
			$$ = createNode("logical_OR_expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf("||"));
			addChild($$, $3);
		}
	;

conditional_expression:
	logical_OR_expression
		{
			// yyinfo("conditional_expression -> logical_OR_expression");
			$$ = createNode("conditional_expression", 1);
			addChild($$, $1);
		}
	| logical_OR_expression QUESTION_MARK expression COLON conditional_expression
		{
			// yyinfo("conditional_expression -> logical_OR_expression ? expression : conditional_expression");
			$$ = createNode("conditional_expression", 5);
			addChild($$, $1);
			addChild($$, createLeaf("?"));
			addChild($$, $3);
			addChild($$, createLeaf(":"));
			addChild($$, $5);
		}
	;

assignment_expression:
	conditional_expression
		{
			// yyinfo("assignment_expression -> conditional_expression");
			$$ = createNode("assignment_expression", 1);
			addChild($$, $1);
		}
	| unary_expression assignment_operator assignment_expression
		{
			// yyinfo("assignment_expression -> unary_expression assignment_operator assignment_expression");
			$$ = createNode("assignment_expression", 3);
			addChild($$, $1);
			addChild($$, $2);
			addChild($$, $3);
		}
	;

assignment_operator:
	ASSIGNMENT
		{
			// yyinfo("assignment_operator -> =");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf("="));

		}
	| STAR_ASSIGNMENT
		{
			// yyinfo("assignment_operator -> *=");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf("*="));
		}
	| FRONT_SLASH_ASSIGNMENT
		{
			// yyinfo("assignment_operator -> /=");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf("/="));
		}
	| MODULO_ASSIGNMENT
		{
			// yyinfo("assignment_operator -> %=");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf("%="));
		}
	| PLUS_ASSIGNMENT
		{
			// yyinfo("assignment_operator -> += ");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf("+="));
		}
	| MINUS_ASSIGNMENT
		{
			// yyinfo("assignment_operator -> -= ");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf("-="));
		}
	| LEFT_SHIFT_ASSIGNMENT
		{
			// yyinfo("assignment_operator -> <<=");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf("<<="));
		}
	| RIGHT_SHIFT_ASSIGNMENT
		{
			// yyinfo("assignment_operator -> >>=");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf(">>="));
		}
	| BITWISE_AND_ASSIGNMENT
		{
			// yyinfo("assignment_operator -> &=");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf("&="));
		}
	| BITWISE_XOR_ASSIGNMENT
		{
			// yyinfo("assignment_operator -> ^=");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf("^="));
		}
	| BITWISE_OR_ASSIGNMENT
		{
			// yyinfo("assignment_operator -> |=");
			$$ = createNode("assignment_operator", 1);
			addChild($$, createLeaf("|="));
		}
	;

expression:
	assignment_expression
		{
			// yyinfo("expression -> assignment_expression");
			$$ = createNode("expression", 1);
			addChild($$, $1);
		}
	| expression COMMA assignment_expression
		{
			// yyinfo("expression -> expression , assignment_expression");
			$$ = createNode("expression", 3);
			addChild($$, $1);
			addChild($$, createLeaf(","));
			addChild($$, $3);
		}
	;

constant_expression:
	conditional_expression
		{
			// yyinfo("constant_expression -> conditional_expression");
			$$ = createNode("constant_expression", 1);
			addChild($$, $1);
		}
	;

/*All the rules of 2)Declarations */
declaration:
	declaration_specifiers init_declarator_list_opt SEMICOLON
		{
			// yyinfo("declaration -> declaration_specifiers init_declarator_list_opt ;");
			$$ = createNode("declaration", 3);
			addChild($$, $1);
			addChild($$, $2);
			addChild($$, createLeaf(";"));
		}
	;

init_declarator_list_opt:
	init_declarator_list
		{
			// yyinfo("init_declarator_list_opt -> init_declarator_list");
			$$=createNode("init_declarator_list_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("init_declarator_list_opt -> epsilon");
			$$=createNode("init_declarator_list_opt", 0);
		}
	;

declaration_specifiers:
	storage_class_specifier declaration_specifiers_opt
		{
			// yyinfo("declaration_specifiers -> storage_class_specifier declaration_specifiers_opt");
			$$=createNode("declaration_specifiers", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	| type_specifier declaration_specifiers_opt
		{
			// yyinfo("declaration_specifiers -> type_specifier declaration_specifiers_opt");
			$$=createNode("declaration_specifiers", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	| type_qualifier declaration_specifiers_opt
		{
			// yyinfo("declaration_specifiers -> type_qualifier declaration_specifiers_opt");
			$$=createNode("declaration_specifiers", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	| function_specifier declaration_specifiers_opt
		{
			// yyinfo("declaration_specifiers -> function_specifier declaration_specifiers_opt");
			$$=createNode("declaration_specifiers", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	;

declaration_specifiers_opt:
	declaration_specifiers
		{
			// yyinfo("declaration_specifiers_opt -> declaration_specifiers");
			$$=createNode("declaration_specifiers_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("declaration_specifiers_opt -> epsilon ");
			$$=createNode("declaration_specifiers_opt", 0);
		}
	;

init_declarator_list:
	init_declarator
		{
			// yyinfo("init_declarator_list -> init_declarator");
			$$=createNode("init_declarator_list", 1);
			addChild($$, $1);
		}
	| init_declarator_list COMMA init_declarator
		{
			// yyinfo("init_declarator_list -> init_declarator_list , init_declarator");
			$$=createNode("init_declarator_list", 3);
			addChild($$, $1);
			addChild($$, createLeaf(","));
			addChild($$, $3);
		}
	;

init_declarator:
	declarator
		{
			// yyinfo("init_declarator -> declarator");
			$$=createNode("init_declarator", 1);
			addChild($$, $1);
		}
	| declarator ASSIGNMENT initializer
		{
			// yyinfo("init_declarator -> declarator = initializer");
			$$=createNode("init_declarator", 3);
			addChild($$, $1);
			addChild($$, createLeaf("="));
			addChild($$, $3);
		}
	;

storage_class_specifier:
	EXTERN
		{
			// yyinfo("storage_class_specifier -> extern");
			$$=createNode("storage_class_specifier", 1);
			addChild($$, createLeaf("extern"));
		}
	| STATIC
		{
			// yyinfo("storage_class_specifier -> static");
			$$=createNode("storage_class_specifier", 1);
			addChild($$, createLeaf("static"));
		}
	| AUTO
		{
			// yyinfo("storage_class_specifier -> auto");
			$$=createNode("storage_class_specifier", 1);
			addChild($$, createLeaf("auto"));
		}
	| REGISTER
		{
			// yyinfo("storage_class_specifier -> register");
			$$=createNode("storage_class_specifier", 1);
			addChild($$, createLeaf("register"));
		}
	;

type_specifier:
	VOID
		{
			// yyinfo("type_specifier -> void");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("void"));
		}
	| CHAR
		{
			// yyinfo("type_specifier -> char");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("char"));
		}
	| SHORT
		{
			// yyinfo("type_specifier -> short");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("short"));
		}
	| INT
		{
			// yyinfo("type_specifier -> int");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("int"));
		}
	| LONG
		{
			// yyinfo("type_specifier -> long");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("long"));
		}
	| FLOAT
		{
			// yyinfo("type_specifier -> float");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("float"));
		}
	| DOUBLE
		{
			// yyinfo("type_specifier -> double");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("double"));
		}
	| SIGNED
		{
			// yyinfo("type_specifier -> signed");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("signed"));
		}
	| UNSIGNED
		{
			// yyinfo("type_specifier -> unsigned");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("unsigned"));
		}
	| _BOOL
		{
			// yyinfo("type_specifier -> _Bool");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("_Bool"));
		}
	| _COMPLEX
		{
			// yyinfo("type_specifier -> _Complex");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("_Complex"));
		}
	| _IMAGINARY
		{
			// yyinfo("type_specifier -> _Imaginary");
			$$=createNode("type_specifier", 1);
			addChild($$, createLeaf("_Imaginary"));
		}
	;

specifier_qualifier_list:
	type_specifier specifier_qualifier_list_opt
		{
			// yyinfo("specifier_qualifier_list -> type_specifier specifier_qualifier_list_opt");
			$$=createNode("specifier_qualifier_list", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	| type_qualifier specifier_qualifier_list_opt
		{
			// yyinfo("specifier_qualifier_list -> type_qualifier specifier_qualifier_list_opt");
			$$=createNode("specifier_qualifier_list", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	;

specifier_qualifier_list_opt:
	specifier_qualifier_list
		{
			// yyinfo("specifier_qualifier_list_opt -> specifier_qualifier_list");
			$$=createNode("specifier_qualifier_list_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo	("specifier_qualifier_list_opt -> epsilon");
			$$=createNode("specifier_qualifier_list_opt", 0);
		}
	;

type_qualifier:
	CONST
		{
			// yyinfo("type_qualifier -> const");
			$$=createNode("type_qualifier", 1);
			addChild($$, createLeaf("const"));
		}
	| RESTRICT
		{
			// yyinfo("type_qualifier -> restrict");
			$$=createNode("type_qualifier", 1);
			addChild($$, createLeaf("restrict"));
		}
	| VOLATILE
		{
			// yyinfo("type_qualifier -> volatile");
			$$=createNode("type_qualifier", 1);
			addChild($$, createLeaf("volatile"));
		}
	;

function_specifier:
	INLINE
		{
			// yyinfo("function_specifier -> inline");
			$$=createNode("function_specifier", 1);
			addChild($$, createLeaf("inline"));
		}
	;

declarator:
	pointer_opt direct_declarator
		{
			// yyinfo("declarator -> pointer_opt direct_declarator");
			$$=createNode("declarator", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	;

pointer_opt:
	pointer
		{
			// yyinfo("pointer_opt -> pointer");
			$$=createNode("pointer_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("pointer_opt -> epsilon");
			$$=createNode("pointer_opt", 0);
		}
	;

direct_declarator:
	IDENTIFIER
		{
			// yyinfo("direct_declarator -> IDENTIFIER"); printf("IDENTIFIER = `%s`\n", $1);
			$$=createNode("direct_declarator", 1);
			addChild($$, createLeaf($1));
		}
	| LEFT_PARENTHESIS declarator RIGHT_PARENTHESIS
		{
			// yyinfo("direct_declarator -> ( declarator )");
			$$=createNode("direct_declarator", 3);
			addChild($$, createLeaf("("));
			addChild($$, $2);
			addChild($$, createLeaf(")"));
		}
	| direct_declarator LEFT_SQUARE_BRACKET type_qualifier_list_opt assignment_expression_opt RIGHT_SQUARE_BRACKET
		{
			// yyinfo("direct_declarator -> direct_declarator [ type_qualifier_list_opt assignment_expression_opt ]");
			$$=createNode("direct_declarator", 5);
			addChild($$, $1);
			addChild($$, createLeaf("["));
			addChild($$, $3);
			addChild($$, $4);
			addChild($$, createLeaf("]"));
		}
	| direct_declarator LEFT_SQUARE_BRACKET STATIC type_qualifier_list_opt assignment_expression RIGHT_SQUARE_BRACKET
		{
			// yyinfo("direct_declarator -> direct_declarator [ static type_qualifier_list_opt assignment_expression ]");
			$$=createNode("direct_declarator", 6);
			addChild($$, $1);
			addChild($$, createLeaf("["));
			addChild($$, createLeaf("static"));
			addChild($$, $4);
			addChild($$, $5);
			addChild($$, createLeaf("]"));
		}
	| direct_declarator LEFT_SQUARE_BRACKET type_qualifier_list STATIC assignment_expression RIGHT_SQUARE_BRACKET
		{
			// yyinfo("direct_declarator -> direct_declarator [ type_qualifier_list static assignment_expression ]");
			$$=createNode("direct_declarator", 6);
			addChild($$, $1);
			addChild($$, createLeaf("["));
			addChild($$, $3);
			addChild($$, createLeaf("static"));
			addChild($$, $5);
			addChild($$, createLeaf("]"));
		}
	| direct_declarator LEFT_SQUARE_BRACKET type_qualifier_list_opt STAR RIGHT_SQUARE_BRACKET
		{
			// yyinfo("direct_declarator -> direct_declarator [ type_qualifier_list_opt * ]");
			$$=createNode("direct_declarator", 5);
			addChild($$, $1);
			addChild($$, createLeaf("["));
			addChild($$, $3);
			addChild($$, createLeaf("*"));
			addChild($$, createLeaf("]"));
		}
	| direct_declarator LEFT_PARENTHESIS parameter_type_list RIGHT_PARENTHESIS
		{
			// yyinfo("direct_declarator -> direct_declarator ( parameter_type_list )");
			$$=createNode("direct_declarator", 4);
			addChild($$, $1);
			addChild($$, createLeaf("("));
			addChild($$, $3);
			addChild($$, createLeaf(")"));
		}
	| direct_declarator LEFT_PARENTHESIS identifier_list_opt RIGHT_PARENTHESIS
		{
			// yyinfo("direct_declarator -> direct_declarator ( identifier_list_opt )");
			$$=createNode("direct_declarator", 4);
			addChild($$, $1);
			addChild($$, createLeaf("("));
			addChild($$, $3);
			addChild($$, createLeaf(")"));
		}
	;

type_qualifier_list_opt:
	type_qualifier_list
		{
			// yyinfo("type_qualifier_list_opt -> type_qualifier_list");
			$$=createNode("type_qualifier_list_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("type_qualifier_list_opt -> epsilon");
			$$=createNode("type_qualifier_list_opt", 0);
		}
	;

assignment_expression_opt:
	assignment_expression
		{
			// yyinfo("assignment_expression_opt -> assignment_expression");
			$$=createNode("assignment_expression_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("assignment_expression_opt -> epsilon");
			$$=createNode("assignment_expression_opt", 0);
		}
	;

identifier_list_opt:
	identifier_list
		{
			// yyinfo("identifier_list_opt -> identifier_list");
			$$=createNode("identifier_list_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("identifier_list_opt -> epsilon");
			$$=createNode("identifier_list_opt", 0);
		}
	;

pointer:
	STAR type_qualifier_list_opt
		{
			// yyinfo("pointer -> * type_qualifier_list_opt");
			$$=createNode("pointer", 2);
			addChild($$, createLeaf("*"));
			addChild($$, $2);
		}
	| STAR type_qualifier_list_opt pointer
		{
			// yyinfo("pointer -> * type_qualifier_list_opt pointer");
			$$=createNode("pointer", 3);
			addChild($$, createLeaf("*"));
			addChild($$, $2);
			addChild($$, $3);
		}
	;

type_qualifier_list:
	type_qualifier
		{
			// yyinfo("type_qualifier_list -> type_qualifier");
			$$=createNode("type_qualifier_list", 1);
			addChild($$, $1);
		}
	| type_qualifier_list type_qualifier
		{
			// yyinfo("type_qualifier_list -> type_qualifier_list type_qualifier");
			$$=createNode("type_qualifier_list", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	;

parameter_type_list:
	parameter_list
		{
			// yyinfo("parameter_type_list -> parameter_list");
			$$=createNode("parameter_type_list", 1);
			addChild($$, $1);
		}
	| parameter_list COMMA ELLIPSIS
		{
			// yyinfo("parameter_type_list -> parameter_list , ...");
			$$=createNode("parameter_type_list", 3);
			addChild($$, $1);
			addChild($$, createLeaf(","));
			addChild($$, createLeaf("..."));
		}
	;

parameter_list:
	parameter_declaration
		{
			// yyinfo("parameter_list -> parameter_declaration");
			$$=createNode("parameter_list", 1);
			addChild($$, $1);
		}
	| parameter_list COMMA parameter_declaration
		{
			// yyinfo("parameter_list -> parameter_list , parameter_declaration");
			$$=createNode("parameter_list", 3);
			addChild($$, $1);
			addChild($$, createLeaf(","));
			addChild($$, $3);
		}
	;

parameter_declaration:
	declaration_specifiers declarator
		{
			// yyinfo("parameter_declaration -> declaration_specifiers declarator");
			$$=createNode("parameter_declaration", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	| declaration_specifiers
		{
			// yyinfo("parameter_declaration -> declaration_specifiers");
			$$=createNode("parameter_declaration", 1);
			addChild($$, $1);
		}
	;

identifier_list:
	IDENTIFIER
		{
			// yyinfo("identifier_list -> IDENTIFIER"); printf("IDENTIFIER = `%s`\n", $1);
			$$=createNode("identifier_list", 1);
			addChild($$, createLeaf($1));
		}
	| identifier_list COMMA IDENTIFIER
		{
			// yyinfo("identifier_list -> identifier_list , IDENTIFIER"); printf("IDENTIFIER = `%s`\n", $3);
			$$=createNode("identifier_list", 3);
			addChild($$, $1);
			addChild($$, createLeaf(","));
			addChild($$, createLeaf($3));
		}
	;

type_name:
	specifier_qualifier_list
		{
			// yyinfo("type_name -> specifier_qualifier_list");
			$$=createNode("type_name", 1);
			addChild($$,$1);
		}
	;

initializer:
	assignment_expression
		{
			// yyinfo("initializer -> assignment_expression");
			$$=createNode("initializer", 1);
			addChild($$, $1);
		}
	| LEFT_BRACE initializer_list RIGHT_BRACE
		{
			// yyinfo("initializer -> { initializer_list }");
			$$=createNode("initializer", 3);
			addChild($$, createLeaf("{"));
			addChild($$, $2);
			addChild($$, createLeaf("}"));
		}
	| LEFT_BRACE initializer_list COMMA RIGHT_BRACE
		{
			// yyinfo("initializer -> { initializer_list , }");
			$$=createNode("initializer", 3);
			addChild($$, createLeaf("{"));
			addChild($$, $2);
			addChild($$, createLeaf(","));
			addChild($$, createLeaf("}"));
		}
	;

initializer_list:
	designation_opt initializer
		{
			// yyinfo("initializer_list -> designation_opt initializer");
			$$=createNode("initializer_list", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	| initializer_list COMMA designation_opt initializer
		{
			// yyinfo("initializer_list -> initializer_list , designation_opt initializer");
			$$=createNode("initializer_list", 4);
			addChild($$, $1);
			addChild($$, createLeaf(","));
			addChild($$, $3);
			addChild($$, $4);

		}
	;

designation_opt:
	designation
		{
			// yyinfo("designation_opt -> designation");
			$$=createNode("designation_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("designation_opt -> epsilon");
			$$=createNode("designation_opt", 0);
		}
	;

designation:
	designator_list ASSIGNMENT
		{
			// yyinfo("designation -> designator_list =");
			$$=createNode("designation", 2);
			addChild($$, $1);
			addChild($$, createLeaf("="));
		}
	;

designator_list:
	designator
		{
			// yyinfo("designator_list -> designator");
			$$=createNode("designator_list", 1);
			addChild($$, $1);
		}
	| designator_list designator
		{
			// yyinfo("designator_list -> designator_list designator");
			$$=createNode("designator_list", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	;

designator:
	LEFT_SQUARE_BRACKET constant_expression RIGHT_SQUARE_BRACKET
		{
			// yyinfo("designator -> [ constant_expression ]");
			$$=createNode("designator", 3);
			addChild($$, createLeaf("["));
			addChild($$, $2);
			addChild($$, createLeaf("]"));
		}
	| DOT IDENTIFIER
		{
			// yyinfo("designator -> . IDENTIFIER"); printf("IDENTIFIER = `%s`\n", $2);
			$$=createNode("designator", 2);
			addChild($$, createLeaf("."));
			addChild($$, createLeaf($2));
		}
	;

/* All the rules of 3)Statements */
statement:
	labeled_statement
		{
			// yyinfo("statement -> labeled_statement");
			$$=createNode("statement", 1);
			addChild($$, $1);
		}
	| compound_statement
		{
			// yyinfo("statement -> compound_statement");
			$$=createNode("statement", 1);
			addChild($$, $1);
		}
	| expression_statement
		{
			// yyinfo("statement -> expression_statement");
			$$=createNode("statement", 1);
			addChild($$, $1);
		}
	| selection_statement
		{
			// yyinfo("statement -> selection_statement");
			$$=createNode("statement", 1);
			addChild($$, $1);
		}
	| iteration_statement
		{
			// yyinfo("statement -> iteration_statement");
			$$=createNode("statement", 1);
			addChild($$, $1);
		}
	| jump_statement
		{
			// yyinfo("statement -> jump_statement");
			$$=createNode("statement", 1);
			addChild($$, $1);
		}
	;

labeled_statement:
	IDENTIFIER COLON statement
		{
			// yyinfo("labeled_statement -> IDENTIFIER : statement"); printf("IDENTIFIER = `%s`\n", $1);
			$$=createNode("labeled_statement", 3);
			addChild($$, createLeaf($1));
			addChild($$, createLeaf(":"));
			addChild($$, $3);
		}
	| CASE constant_expression COLON statement
		{
			// yyinfo("labeled_statement -> case constant_expression : statement");
			$$=createNode("labeled_statement", 4);
			addChild($$, createLeaf("case"));
			addChild($$, $2);
			addChild($$, createLeaf(":"));
			addChild($$, $4);
		}
	| DEFAULT COLON statement
		{
			// yyinfo("labeled_statement -> default : statement");
			$$=createNode("labeled_statement", 3);
			addChild($$, createLeaf("default"));
			addChild($$, createLeaf(":"));
			addChild($$, $3);
		}
	;

compound_statement:
	LEFT_BRACE block_item_list_opt RIGHT_BRACE
		{
			// yyinfo("compound_statement -> { block_item_list_opt }");
			$$=createNode("compound_statement", 3);
			addChild($$, createLeaf("{"));
			addChild($$, $2);
			addChild($$, createLeaf("}"));
		}
	;

block_item_list_opt:
	block_item_list
		{
			// yyinfo("block_item_list_opt -> block_item_list");
			$$=createNode("block_item_list_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("block_item_list_opt -> epsilon");
			$$=createNode("block_item_list_opt", 0);
		}
	;

block_item_list:
	block_item
		{
			// yyinfo("block_item_list -> block_item");
			$$=createNode("block_item_list", 1);
			addChild($$, $1);
		}
	| block_item_list block_item
		{
			// yyinfo("block_item_list -> block_item_list block_item");
			$$=createNode("block_item_list", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	;

block_item:
	declaration
		{
			// yyinfo("block_ite	m -> declaration");
			$$=createNode("block_item", 1);
			addChild($$, $1);
		}
	| statement
		{
			// yyinfo("block_item -> statement");
			$$=createNode("block_item", 1);
			addChild($$, $1);
		}
	;

expression_statement:
	expression_opt SEMICOLON
		{
			// yyinfo("expression_statement -> expression_opt ;");
			$$=createNode("expression_statement", 2);
			addChild($$, $1);
			addChild($$, createLeaf(";"));
		}
	;

expression_opt:
	expression
		{
			// yyinfo("expression_opt -> expression");
			$$=createNode("expression_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("expression_opt -> epsilon");
			$$=createNode("expression_opt", 0);
		}
	;

selection_statement:
	IF LEFT_PARENTHESIS expression RIGHT_PARENTHESIS statement
		{
			// yyinfo("selection_statement -> if ( expression ) statement");
			$$=createNode("selection_statement", 5);
			addChild($$, createLeaf("if"));
			addChild($$, createLeaf("("));
			addChild($$, $3);
			addChild($$, createLeaf(")"));
			addChild($$, $5);
		}
	| IF LEFT_PARENTHESIS expression RIGHT_PARENTHESIS statement ELSE statement
		{
			// yyinfo("selection_statement -> if ( expression ) statement else statement");
			$$=createNode("selection_statement", 7);
			addChild($$, createLeaf("if"));
			addChild($$, createLeaf("("));
			addChild($$, $3);
			addChild($$, createLeaf(")"));
			addChild($$, $5);
			addChild($$, createLeaf("else"));
			addChild($$, $7);
		}
	| SWITCH LEFT_PARENTHESIS expression RIGHT_PARENTHESIS statement
		{
			// yyinfo("selection_statement -> switch ( expression ) statement");
			$$=createNode("selection_statement", 5);
			addChild($$, createLeaf("switch"));
			addChild($$, createLeaf("("));
			addChild($$, $3);
			addChild($$, createLeaf(")"));
			addChild($$, $5);
		}
	;

iteration_statement:
	WHILE LEFT_PARENTHESIS expression RIGHT_PARENTHESIS statement
		{
			// yyinfo("iteration_statement -> while ( expression ) statement");
			$$=createNode("iteration_statement", 5);
			addChild($$, createLeaf("while"));
			addChild($$, createLeaf("("));
			addChild($$, $3);
			addChild($$, createLeaf(")"));
			addChild($$, $5);
		}
	| DO statement WHILE LEFT_PARENTHESIS expression RIGHT_PARENTHESIS SEMICOLON
		{
			// yyinfo("iteration_statement -> do statement while ( expression ) ;");
			$$=createNode("iteration_statement", 7);
			addChild($$, createLeaf("do"));
			addChild($$, $2);
			addChild($$, createLeaf("while"));
			addChild($$, createLeaf("("));
			addChild($$, $5);
			addChild($$, createLeaf(")"));
			addChild($$, createLeaf(";"));
		}
	| FOR LEFT_PARENTHESIS expression_opt SEMICOLON expression_opt SEMICOLON expression_opt RIGHT_PARENTHESIS statement
		{
			// yyinfo("iteration_statement -> for ( expression_opt ; expression_opt ; expression_opt ) statement");
			$$=createNode("iteration_statement", 9);
			addChild($$, createLeaf("for"));
			addChild($$, createLeaf("("));
			addChild($$, $3);
			addChild($$, createLeaf(";"));
			addChild($$, $5);
			addChild($$, createLeaf(";"));
			addChild($$, $7);
			addChild($$, createLeaf(")"));
			addChild($$, $9);
		}
	| FOR LEFT_PARENTHESIS declaration expression_opt SEMICOLON expression_opt RIGHT_PARENTHESIS statement
		{
			// yyinfo("iteration_statement -> for ( declaration expression_opt ; expression_opt ) statement");
			$$=createNode("iteration_statement", 8);
			addChild($$, createLeaf("for"));
			addChild($$, createLeaf("("));
			addChild($$, $3);
			addChild($$, $4);
			addChild($$, createLeaf(";"));
			addChild($$, $6);
			addChild($$, createLeaf(")"));
			addChild($$, $8);
		}
	;

jump_statement:
	GOTO IDENTIFIER SEMICOLON
		{
			// yyinfo("jump_statement -> goto IDENTIFIER ;"); printf("IDENTIFIER = `%s`\n", $2);
			$$=createNode("jump_statement", 3);
			addChild($$, createLeaf("goto"));
			addChild($$, createLeaf($2));
			addChild($$, createLeaf(";"));
		}
	| CONTINUE SEMICOLON
		{
			// yyinfo("jump_statement -> continue ;");
			$$=createNode("jump_statement", 2);
			addChild($$, createLeaf("continue"));
			addChild($$, createLeaf(";"));
		}
	| BREAK SEMICOLON
		{
			// yyinfo("jump_statement -> break ;");
			$$=createNode("jump_statement", 2);
			addChild($$, createLeaf("break"));
			addChild($$, createLeaf(";"));
		}
	| RETURN expression_opt SEMICOLON
		{
			// yyinfo("jump_statement -> return expression_opt ;");
			$$=createNode("jump_statement", 3);
			addChild($$, createLeaf("return"));
			addChild($$, $2);
			addChild($$, createLeaf(";"));
		}
	;

/* All the rules of 4)External definitions */
translation_unit:
	external_declaration
		{
			// yyinfo("translation_unit -> external_declaration");
			$$=createNode("translation_unit", 1);
			addChild($$, $1);

			root=$$;  //returning the Parse Tree formed here
			
		}
	| translation_unit external_declaration
		{
			// yyinfo("translation_unit -> translation_unit external_declaration");
			$$=createNode("translation_unit", 2);
			addChild($$, $1);
			addChild($$, $2);

			root=$$;  //returning the Parse Tree formed here
		}
	;

external_declaration:
	function_definition
		{
			// yyinfo("external_declaration -> function_definition");
			$$=createNode("external_declaration", 1);
			addChild($$, $1);
		}
	| declaration
		{
			// yyinfo("external_declaration -> declaration");
			$$=createNode("external_declaration", 1);
			addChild($$, $1);
		}
	;

function_definition:
	declaration_specifiers declarator declaration_list_opt compound_statement
		{
			// yyinfo("function_definition -> declaration_specifiers declarator declaration_list_opt compound_statement");
			$$=createNode("function_definition", 4);
			addChild($$, $1);
			addChild($$, $2);
			addChild($$, $3);
			addChild($$, $4);
		}
	;

declaration_list_opt:
	declaration_list
		{
			// yyinfo("declaration_list_opt -> declaration_list");
			$$=createNode("declaration_list_opt", 1);
			addChild($$, $1);
		}
	|
		{
			// yyinfo("declaration_list_opt -> epsilon");
			$$=createNode("declaration_list_opt", 0);
		}
	;

declaration_list:
	declaration
		{
			// yyinfo("declaration_list -> declaration");
			$$=createNode("declaration_list", 1);
			addChild($$, $1);
		}
	| declaration_list declaration
		{
			// yyinfo("declaration_list -> declaration_list declaration");
			$$=createNode("declaration_list", 2);
			addChild($$, $1);
			addChild($$, $2);
		}
	;
%%
