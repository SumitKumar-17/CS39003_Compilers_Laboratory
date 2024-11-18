
#include "expr.h"
#include "expr.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

int memory[MAX_MEMORY];
int register_zero_free = 1;
int register_one_free = 1;
int memory_offset_value = 0;
RegisterArray *register_array = NULL;
SymbolTable *symbol_table = NULL;
int temp_var_counter = 0;

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

void yyinfo( const char * format, ... )
{
	// preprend `*** Info: ` to the info message
	fputs( "*** Info: ", stdout );
	va_list args;
	va_start( args, format );
	vfprintf( stdout, format, args );
	va_end( args );
	fputc( '\n', stdout );
}

void init_symbol_table()
{
    symbol_table = (SymbolTable *)malloc(sizeof(SymbolTable));
    symbol_table->name = "start";
    symbol_table->offset = 0;
    symbol_table->next = NULL;
}

void insert_symbol(char *name, int offset)
{
    SymbolTable *new_symbol = (SymbolTable *)malloc(sizeof(SymbolTable));
    // printf("Inserting symbol %s at offset %d\n", name, offset);
    new_symbol->name = strdup(name);
    new_symbol->offset = offset;
    new_symbol->next = symbol_table->next;
    symbol_table->next = new_symbol;
}

int get_memory_loc(char *name)
{
    if (symbol_table == NULL)
    {
        printf("Symbol Table is NULL\n");
        return -1;
    }
    SymbolTable *current = symbol_table->next;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current->offset;
        }
        current = current->next;
    }
    return -1;
}

void print_symbol_table()
{
    SymbolTable *current = symbol_table->next;
    while (current != NULL)
    {
        printf("Name: %s, Offset: %d\n", current->name, current->offset);
        current = current->next;
    }
}

void initialize_register_array()
{
    register_array = (RegisterArray *)malloc(sizeof(RegisterArray));
    for (int i = 0; i < MAX_REGISTERS; i++)
    {
        register_array->registers[i] = REG_TEMPORARY_VALUE_INITIALIZATION;
    }
    for (int i = 0; i < MAX_REGISTERS; i++)
    {
        register_array->free_reg_value[i] = 1;
        // 1 denotes the register is free to use
    }
}

int get_first_free_register(bool *where_is_stored)
{
    // the bool variable decides whether the index returned is coming
    // from a register or from the memory
    for (int i = 2; i < MAX_REGISTERS; i++)
    {
        if (register_array->free_reg_value[i] == 1)
        {
            register_array->free_reg_value[i] = 0;
            *where_is_stored = false; // 0 denotes value is stored in the register
            return i;
        }
    }
    // return 1;

    // if any of the regsiter is not free then  we store the last value in the memory
    //  we alrealdy know that none of the registers are free so we can directly store the value in the memory
    //  and return the memory location
    // memory[memory_offset_value++] = register_array->registers[0];
    // register_array->free_reg_value[0] = 1;
    *where_is_stored = true; // 1 denotes value is stored in the memory
    return memory_offset_value ;
    // return -10;
}

bool check_given_register_is_free(int reg)
{
    if (register_array->free_reg_value[reg] == 1)
    {
        return true;
    }
    return false;
}

void make_register_free(int reg_index)
{   
    if(reg_index==0) register_zero_free=1;
    else if(reg_index==1) register_one_free=1;
    else{
        register_array->free_reg_value[reg_index] = 1;
    }
}

int main()
{
    //  yydebug = 1;
    // FILE *out;
    // out = fopen("intcode.c", "w");
    // if (!out)
    // {
        // printf("Failed to open output file\n");
        // return 1;
    // }

    printf( "#include <stdio.h>\n#include <stdlib.h> \n#include \"aux.c\"\n\nint main() {\n");
    printf( "int R[12];\nint MEM[65536];\n");

    init_symbol_table();
    if (symbol_table == NULL)
    {
        printf("Symbol Table is NULL\n");
    }
    initialize_register_array();
    // printf("Starting....\n");

    // FILE *original_stdout = stdout;
    // stdout = out;
    if (!yyparse())
    {
        // printf("Parsing done\n");
    }
    else
    {
        printf("Parsing failed\n");
    }

    // stdout=original_stdout;
    // printf("Ending....\n");
    printf("\n");
    printf("//This line is for the printing of the symbol table");
    printf("\n//Printing the symbol table\n");
    printf("/*\n");
    print_symbol_table();
    printf("*/\n");
    printf("\n");

    printf("exit (0);\n}\n");
    // fclose(out);

    return 0;
}
