%{
    #include "poly.h"
    #include <stdio.h>

    #include <stdlib.h>
    #include <string.h>

    TreeNode *root;
%}

%union{
    char identifierVal[1024];
    int intVal;
	int powerVal;

    TreeNode *node;
}


%type <node> S P T N X M 

%token PLUS 
%token MINUS 
%token EXPONENT

%token<identifierVal> PLACEHOLDER_X_POWER_ONE

%token<intVal> ZERO
%token<intVal> ONE 
%token<intVal> INTEGER_CONSTANT


%start  S



%%
/*All the Rules */
S:
	P
		{
			// yyinfo("S -> P");
			$$ = createNode("S", 1);
            addChild($$, $1);

			root=$$;
		}
	| PLUS P
		{
			// yyinfo("S -> + P");
			$$ = createNode("S", 2);
			addChild($$,createLeaf("+"));
            addChild($$, $2);

			root=$$;
		}
	| MINUS P
		{
			// yyinfo("S -> - P");
			$$ = createNode("S", 2);
			addChild($$,createLeaf("-"));
            addChild($$, $2);

			root=$$;
		}
	;

P:
	T
		{
			// yyinfo("P -> T");
			$$=createNode("P",1);
			addChild($$,$1);
		}
	| T PLUS P
		{
			// yyinfo("P -> T + P");
			$$=createNode("P",3);
			addChild($$,$1);
			addChild($$,createLeaf("+"));
			addChild($$,$3);
		}
	| T MINUS P
		{
			// yyinfo("S -> T - P");
			$$=createNode("P",3);
			addChild($$,$1);
			addChild($$,createLeaf("-"));
			addChild($$,$3);
		}
	;


T:
	ONE
		{
			// yyinfo("T -> 1");
			$$ = createNode("T", 1);
            char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
		}
	| N
		{
			// yyinfo("T -> N");
			$$=createNode("T",1);
			addChild($$,$1);
		}
	| X
		{
			// yyinfo("T -> X");
			$$=createNode("T",1);
			addChild($$,$1);
		}
	| N X
		{
			// yyinfo("T -> N X");
			$$=createNode("T",2);
			addChild($$,$1);
			addChild($$,$2);
		}
	;

X:	
	PLACEHOLDER_X_POWER_ONE 
		{
			// yyinfo("X -> x");
			$$=createNode("X",1);
			addChild($$,createLeaf($1));
		}

	| PLACEHOLDER_X_POWER_ONE EXPONENT N
		{
			// yyinfo("X -> x ^ N");
			$$=createNode("X",3);
			addChild($$,createLeaf($1));
			addChild($$,createLeaf("^"));
			addChild($$,$3);
		}
	;

N:
	INTEGER_CONSTANT
		{
			// yyinfo("N -> D");
			$$=createNode("N",1);
			char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
		}
	| ONE M
		{
			// yyinfo("N -> 1 M");
			$$=createNode("N",2);
			char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
			addChild($$,$2);
		}
	| INTEGER_CONSTANT M
		{
			// yyinfo("N -> D M");
			$$=createNode("N",2);
			char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
			addChild($$,$2);
		}
	;

M:
	ZERO
		{
			// yyinfo("M -> 0");
			$$=createNode("M",1);
			char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
		}
	| ONE
		{
			// yyinfo("M -> 1");
			$$=createNode("M",1);
			char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
		}
	| INTEGER_CONSTANT
		{
			// yyinfo("M -> D");
			$$=createNode("M",1);
			char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
		}
	| ZERO M
		{
			// yyinfo("M -> 0 M");
			$$=createNode("M",2);
			char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
			addChild($$,$2);
		}
	| ONE M
		{
			// yyinfo("M -> 1 M");
			$$=createNode("M",2);
			char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
			addChild($$,$2);
		}
	| INTEGER_CONSTANT M
		{
			// yyinfo("M -> D M");
			$$=createNode("M",2);
			char buf[1024];
			sprintf(buf, "%d", $1);
            addChild($$, createLeaf(buf));
			addChild($$,$2);
		}
	;


%%
