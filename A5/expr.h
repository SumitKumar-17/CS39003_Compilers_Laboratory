#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_
#include <stdbool.h>

#define MAX_REGISTERS 12
#define MAX_MEMORY 65536
#define MAX_SYMBOLS 100
#define REG_TEMPORARY_VALUE_INITIALIZATION -999999999


int yyparse();
int yylex();
void yyerror( const char * format, ... );
void yyinfo( const char * format, ... );



//for the symbol table
typedef struct SymbolTable{
    char *name;
    int offset;
    struct SymbolTable *next;
} SymbolTable;

//offset refers to the index present in the memory array
void init_symbol_table();
void insert_symbol(char *name, int offset);
int get_memory_loc(char *name);
void print_symbol_table();


typedef struct RegisterArray{
    int registers[MAX_REGISTERS];
    int free_reg_value[MAX_REGISTERS];
}RegisterArray;

void initialize_register_array();
int  get_first_free_register(bool *where_is_stored);
bool check_given_register_is_free(int reg);
void make_register_free(int reg_index);

extern int memory[MAX_MEMORY];
extern int memory_offset_value;
extern RegisterArray *register_array;
extern SymbolTable *symbol_table;
extern int register_zero_free ;
extern int register_one_free ;
extern int temp_var_counter;




extern char *yytext;
extern int yylineno;


#endif
