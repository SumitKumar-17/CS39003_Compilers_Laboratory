%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include <stdarg.h>

    void yyerror( const char * format, ... );
    int yylex();
    int yywrap(){
        return 1;
    }


    int instruction_number=0;
    int temporary_value=0;
    int current_block=0;
    int num_regs=5;
    int size_symbol_table=0;
    extern int yylineno;


    typedef struct quad{
        int position_quad;
        int block_number;
        int flag;


        char* oper;
        char *x;
        char *y;
        char * z;

        struct quad* ptr_next;
    }quad;

    typedef struct symb{
        char * name;
        int reg_or_mem;
        int *locs;
        struct symb*ptr_next;
    }sym;


    typedef struct regs{
        char vars[100];
        int isFree;
        int latestInMemory;
    }reg;


    sym* symtbl;
    quad* list_three_address_code;
    reg* reg_descriptor;

    void backpatching(int x){
        quad* ptr = list_three_address_code;
        while(ptr!=NULL)
        {
            if(ptr->position_quad==x)
            {
                char* temp =(char*)malloc(100*sizeof(char));
                sprintf(temp,"%d",instruction_number+1);
                ptr->z=strdup(temp);
                return;
            }
            ptr=ptr->ptr_next;
        }
    }

    void insertSymbolTable(char *id)
    {
        sym* new_sym= (sym*)malloc(sizeof(sym));
        new_sym->name=strdup(id);
        new_sym->reg_or_mem=-1;
        new_sym->locs=(int*)malloc(num_regs*sizeof(int));
        for(int i=0;i<num_regs;i++){
            new_sym->locs[i]=0;
        }

        if(symtbl!=NULL){
            sym* ptr = symtbl;
            while(ptr->ptr_next!=NULL)
            {
                ptr=ptr->ptr_next;
                if(strcmp(ptr->name,id)==0)return;
            }
            size_symbol_table++;
            ptr->ptr_next=new_sym;
        }else{
            size_symbol_table++;
            symtbl=new_sym;   
        }
    }

    void insertQuad(char *op,char * x,char *y,char *z){
        quad* n_quad= (quad*)malloc(sizeof(quad));
        n_quad->position_quad=++instruction_number;
        n_quad->block_number=0;
        n_quad->flag=0;
        n_quad->oper=strdup(op);
        n_quad->x=strdup(x);
        n_quad->y=strdup(y);
        n_quad->z=strdup(z);

        if(list_three_address_code!=NULL){
            quad* pointer = list_three_address_code;
            while(pointer->ptr_next!=NULL)
            {
                pointer=pointer->ptr_next;
            }
            pointer->ptr_next=n_quad;
        }else{
            list_three_address_code=n_quad;
        }
    }

    void set_leader(int x){
        quad*ptr=list_three_address_code;
        while(ptr!=NULL)
        {
            if(ptr->ptr_next!=NULL && ptr->position_quad == x )
            {
                ptr->flag=1;
                return;
            }
            ptr=ptr->ptr_next;
        }
    }


    void set_leaders(){
        quad* ptr=list_three_address_code;
        ptr->flag=1;
        while(ptr!=NULL)
        {
            if(strcmp(ptr->oper,"goto")==0 || strcmp(ptr->oper,"iffalse")==0)
            {
                set_leader(atoi(ptr->z));
                set_leader(ptr->position_quad+1);
            }
            ptr=ptr->ptr_next;
        }
    }

    void create_reg_descriptor(){
        reg_descriptor = (reg*)malloc((num_regs)*sizeof(reg));
    }

    void add_size_reg_descriptor(){
        for(int i=0;i<num_regs;i++){
            strcpy(reg_descriptor[i].vars,"");
            reg_descriptor[i].isFree=1;
            reg_descriptor[i].latestInMemory=0;
        }
    }


    void reset_registers(){
        for(int i=0;i<num_regs;i++){
            strcpy(reg_descriptor[i].vars,"");
            reg_descriptor[i].isFree=1;
            reg_descriptor[i].latestInMemory=0;
        }
    }

%}


%union {
    int nums;
    char * vals_or_identifiers;
}

%token <vals_or_identifiers> IDENTIFIER
%token <vals_or_identifiers> NUMBERS

%token SET
%token WHEN
%token TERMINAL_LOOP
%token WHILE

%token PLUS
%token MINUS
%token MULT
%token DIV
%token MOD

%token LESS_THAN
%token GREATER_THAN
%token LESS_THAN_EQUAL
%token GREATER_THAN_EQUAL
%token ASSIGN_DIVISION
%token VALUE_ASSIGN

%token LPAREN
%token RPAREN

%type LIST
%type ASGN
%type LOOP

%type <vals_or_identifiers> OPER RELN ATOM EXPR BOOL
%type  STMT
%type  COND

%type <nums> M
%start LIST


%%

LIST: STMT
    | STMT LIST
    ;

STMT : ASGN
    | COND
    | LOOP
    ;

ASGN :
    LPAREN SET IDENTIFIER ATOM RPAREN
    {
        insertSymbolTable($3);
        insertQuad("=",$4,"",$3);
    }
    ;

COND :
    LPAREN WHEN BOOL M LIST RPAREN
    {
        backpatching($4-1);
    }
    ;


LOOP:
     LPAREN TERMINAL_LOOP WHILE M BOOL LIST RPAREN
     {
        char *z=(char*)malloc(100*sizeof(char));
        sprintf(z,"%d",$4);
        insertQuad("goto","","",z);
        backpatching($4);
    }
    ;


EXPR :
    LPAREN OPER ATOM ATOM RPAREN
    {
        $$=(char*)malloc(100*sizeof(char));
        sprintf($$,"$%d",++temporary_value);
        insertSymbolTable($$);
        insertQuad($2,$3,$4,$$);
    }
    ;

BOOL :
    LPAREN RELN ATOM ATOM RPAREN
    {
        $$ = (char*)malloc(100*sizeof(char));
        sprintf($$,"%s %s %s",$3,$2,$4);
        insertQuad("iffalse",$$,"","");
    }
    ;

ATOM :
    IDENTIFIER
    {
        $$=strdup($1);
        insertSymbolTable($1);
    }
    | NUMBERS
    {
        $$=strdup($1);
    }
    | EXPR
    {
        $$=strdup($1);
    }
    ;

OPER :
     PLUS
    {
        $$ = strdup("+");
    }
    | MINUS
    {
            $$ = strdup("-");
    }
    | MULT
    {
            $$ = strdup("*");
    }
    | DIV
    {
            $$ = strdup("/");
    }
    | MOD
    {
            $$ = strdup("%");
    }
    ;

RELN :
     LESS_THAN
    {
        $$ = strdup("<");
    }
    | GREATER_THAN
    {
         $$ = strdup(">");
    }
    | LESS_THAN_EQUAL
    {
        $$ = strdup("<=");
    }
    |GREATER_THAN_EQUAL
    {
        $$ = strdup(">=");
    }
    |
     VALUE_ASSIGN
    {
         $$ =strdup("==");
    }
    | ASSIGN_DIVISION
    {
        $$ = strdup("!=");
    }
    ;

M :
    {
        $$=instruction_number+1;
    }
    ;

%%

void yyerror( const char * format, ... )
{
	// preprend `*** Error: ` to the error message
	fprintf( stdout, "//*** Error at line %d: ", yylineno );
	va_list args;
	va_start( args, format );
	vfprintf( stdout, format, args );
	va_end( args );
	fputc( '\n', stdout );
}


