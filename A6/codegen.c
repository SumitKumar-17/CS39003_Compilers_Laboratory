#include "y.tab.c"
#include "lex.yy.c"
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

char *findOperation(char *oper)
{
    if (strcmp(oper, "+") == 0)
    {
        return "ADD";
    }
    else if (strcmp(oper, "-") == 0)
    {
        return "SUB";
    }
    else if (strcmp(oper, "*") == 0)
    {
        return "MUL";
    }
    else if (strcmp(oper, "/") == 0)
    {
        return "DIV";
    }
    else if (strcmp(oper, "%") == 0)
    {
        return "REM";
    }
    return "";
}

int is_number(const char *str)
{
    if (*str == '\0')
        return 0;
    if (*str == '-' || *str == '+')
        str++;
    while (*str)
    {
        if (!isdigit((unsigned char)*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

bool isTemporary(char *value)
{
    return strcmp(value, "$") == 0;
}
bool isVariable(char *value)
{
    return !is_number(value) && !isTemporary(value);
}

bool willBeUsedLater(char *value, quad *T_Table_ptr)
{

    quad *T_ptr_copy = T_Table_ptr;
    while (T_ptr_copy != NULL)
    {
        if (T_ptr_copy->flag != 1)
        {
            if (strcmp(T_ptr_copy->x, value) == 0 || strcmp(T_ptr_copy->y, value) == 0 || strcmp(T_ptr_copy->z, value) == 0)
            {
                return true;
            }
            T_ptr_copy = T_ptr_copy->ptr_next;
        }
        else
        {
            break;
        }
    }
    return false;
}

int findLeastScoredRegister()
{
    int minIndex = -1, minScore = 1;
    for (int i = 0; i < num_regs; i++)
    {
        int score = (reg_descriptor[i].latestInMemory == 0) ? 1 : 0;
        if (score < minScore)
        {
            minScore = score;
            minIndex = i;
        }
    }
}

void saveVariableToMemory(int regIndex)
{
    if (reg_descriptor[regIndex].latestInMemory == 0)
    {
        reg_descriptor[regIndex].latestInMemory = 1;
    }
}

int getRegister(char *variable, char *tempT, int ruleCheck, quad *T_Table_ptr)
{
    // return 90000;
    sym *varSym = NULL;
    for (sym *s = symtbl; s != NULL; s = s->ptr_next)
    {
        if (strcmp(s->name, variable) == 0)
        {
            varSym = s;
            break;
        }
    }
    if (ruleCheck >= 1 && varSym != NULL && varSym->reg_or_mem >= 0)
    {
        // printf("here");
        return varSym->reg_or_mem;
    }

    if (ruleCheck >= 2)
    {
        for (int i = 0; i < num_regs; i++)
        {
            if (reg_descriptor[i].isFree)
            {
                strcpy(reg_descriptor[i].vars, variable);
                reg_descriptor[i].isFree = 0;
                reg_descriptor[i].latestInMemory;
                return i;
            }
        }
    }

    if (ruleCheck >= 3)
    {
        for (int i = 0; i < num_regs; i++)
        {
            if (reg_descriptor[i].latestInMemory)
            {
                strcpy(reg_descriptor[i].vars, variable);
                reg_descriptor[i].latestInMemory = 0;
                varSym->reg_or_mem = i;
                return i;
            }
        }
    }

    if (ruleCheck >= 4)
    {
        for (int i = 0; i < num_regs; i++)
        {
            if (strcmp(reg_descriptor[i].vars, tempT) == 0)
            {
                strcpy(reg_descriptor[i].vars, variable);
                reg_descriptor[i].latestInMemory = 0;
                varSym->reg_or_mem = i;
                return i;
            }
        }
    }

    if (ruleCheck >= 5)
    {
        for (int i = 0; i < num_regs; i++)
        {
            if (isTemporary(reg_descriptor[i].vars) && !willBeUsedLater(reg_descriptor[i].vars, T_Table_ptr))
            {
                strcpy(reg_descriptor[i].vars, variable);
                reg_descriptor[i].latestInMemory = 0;
                varSym->reg_or_mem = i;
                return i;
            }
        }
    }

    if (ruleCheck >= 6)
    {
        int leastScoredReg = findLeastScoredRegister();
        saveVariableToMemory(leastScoredReg);
        strcpy(reg_descriptor[leastScoredReg].vars, variable);
        reg_descriptor[leastScoredReg].latestInMemory = 0;
        reg_descriptor[leastScoredReg].isFree = 0;
        varSym->reg_or_mem = leastScoredReg;
        return leastScoredReg;
    }
    return -1;
}

int main()
{

    yyparse();
    quad *Q_Table_ptr = list_three_address_code;
    insertQuad("", "", "", "");
    quad *Q_Table_ptr1 = list_three_address_code;
    set_leaders();
    if (Q_Table_ptr == NULL)
    {
        printf("No code generated\n");
        return 0;
    }
    else
    {
        printf("Generating Intermediate codes\n");
        while (Q_Table_ptr != NULL)
        {
            if (Q_Table_ptr->flag == 1)
            {
                Q_Table_ptr->block_number = ++current_block;
                printf("Block %d:\n", current_block);
            }
            else
            {
                Q_Table_ptr->block_number = current_block;
            }

            if (strcmp(Q_Table_ptr->oper, "iffalse") == 0)
            {
                printf("\t%d   : %s (%s) %s goto %s\n", Q_Table_ptr->position_quad, Q_Table_ptr->oper, Q_Table_ptr->x, Q_Table_ptr->y, Q_Table_ptr->z);
            }
            else if (strcmp(Q_Table_ptr->oper, "=") == 0)
            {
                printf("\t%d   : %s %s %s\n", Q_Table_ptr->position_quad, Q_Table_ptr->z, Q_Table_ptr->oper, Q_Table_ptr->x);
            }
            else if (strcmp(Q_Table_ptr->oper, "goto") == 0)
            {
                printf("\t%d   : %s %s\n", Q_Table_ptr->position_quad, Q_Table_ptr->oper, Q_Table_ptr->z);
            }
            else if (strcmp(Q_Table_ptr->oper, "") == 0)
            {
                printf("\t%d   :", Q_Table_ptr->position_quad);
            }
            else
            {
                printf("\t%d   : %s = %s %s %s\n", Q_Table_ptr->position_quad, Q_Table_ptr->z, Q_Table_ptr->x, Q_Table_ptr->oper, Q_Table_ptr->y);
            }
            Q_Table_ptr = Q_Table_ptr->ptr_next;
        }
    }

    create_reg_descriptor();
    add_size_reg_descriptor();

    quad *T_Table_ptr = Q_Table_ptr1;

    printf("\n\n\n");
    current_block = 0;
    printf("Generating Target codes\n");

    while (T_Table_ptr != NULL)
    {
        if (T_Table_ptr->flag == 1)
        {
            reset_registers();
            T_Table_ptr->block_number = ++current_block;
            printf("Block %d:\n", current_block);
        }
        else
        {
            T_Table_ptr->block_number = current_block;
        }

        if (strcmp(T_Table_ptr->oper, "iffalse") == 0)
        {
            // Generate a conditional jump for "iffalse"
            char var[20], op[3], val[20];
            sscanf(T_Table_ptr->x, "%s %s %s", var, op, val);
            // printf("%s A %s A %s",var,op,val);
            int regA = getRegister(var, var, 6, T_Table_ptr);

            if (is_number(val))
            {
                printf("\t%d    : LDI R%d %s\n", T_Table_ptr->position_quad,regA, val); // Load constant into register
                printf("\t        JEQ R%d %s %s\n", regA, val, T_Table_ptr->z); // Jump to label if false")
            }
            else if (isTemporary(val))
            {
                int regT = getRegister(val, val, 6, T_Table_ptr);
                printf("\t%d    : LD %s %s\n",T_Table_ptr->position_quad, reg_descriptor[regA].vars, reg_descriptor[regT].vars); // Load value from temporary
                printf("\t        JEQ R%d %s %s\n", regA, val, T_Table_ptr->z); // Jump to label if false")
            }
            else
            {   
                printf("\t%d    : LD R%d %s\n",T_Table_ptr->position_quad, regA, var); // Load from memory location
                int regB=getRegister(val, val, 6, T_Table_ptr);
                printf("\t        LD R%d %s\n", regB, val); 
                printf("\t        JEQ R%d R%d %s\n", regA, regB, T_Table_ptr->z); // Jump to label if false")

            }
           
        }
        else if (strcmp(T_Table_ptr->oper, "=") == 0)
        {
            int regA, regT;

            // Case 1: x is a number, z is a number
            if (is_number(T_Table_ptr->x) && is_number(T_Table_ptr->z))
            {
                // No need to load or store, just assign the number directly (not needed in actual code, might not happen in your parser)
            }
            // Case 2: x is a number, z is a variable
            else if (is_number(T_Table_ptr->x) && isVariable(T_Table_ptr->z))
            {
                regA = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : LDI R%d %s\n",T_Table_ptr->position_quad, regA, T_Table_ptr->x);
                printf("\t        ST %s R%d\n", T_Table_ptr->z, regA);
            }
            // Case 3: x is a number, z is a temporary
            else if (is_number(T_Table_ptr->x) && isTemporary(T_Table_ptr->z))
            {
                regA = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : LDI R%d %s\n",T_Table_ptr->position_quad, regA, T_Table_ptr->x);
                printf("\t        ST %s %s\n", reg_descriptor[regA].vars, reg_descriptor[regA].vars);
            }

            // Case 4: x is a variable, z is a number
            else if (isVariable(T_Table_ptr->x) && is_number(T_Table_ptr->z))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                printf("\t%d    : LDI R%d %s\n",T_Table_ptr->position_quad, regA, T_Table_ptr->z);
                printf("\t        ST %s %s\n", T_Table_ptr->x, reg_descriptor[regA].vars);
            }
            // Case 5: x is a variable, z is a variable
            else if (isVariable(T_Table_ptr->x) && isVariable(T_Table_ptr->z))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                
                printf("\t%d    : ST %s R%d\n",T_Table_ptr->position_quad,T_Table_ptr->z, regA);
            }
            // Case 6: x is a variable, z is a temporary
            else if (isVariable(T_Table_ptr->x) && isTemporary(T_Table_ptr->z))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                regT = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : ST %s %s\n",T_Table_ptr->position_quad, reg_descriptor[regT].vars, reg_descriptor[regA].vars);
            }

            // Case 7: x is a temporary, z is a number
            else if (isTemporary(T_Table_ptr->x) && is_number(T_Table_ptr->z))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                printf("\t%d    : LDI R%d %s\n",T_Table_ptr->position_quad, regA, T_Table_ptr->z);
                printf("\t        ST %s %s\n", reg_descriptor[regA].vars, reg_descriptor[regA].vars);
            }
            // Case 8: x is a temporary, z is a variable
            else if (isTemporary(T_Table_ptr->x) && isVariable(T_Table_ptr->z))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                printf("\t%d    : ST %s %s\n",T_Table_ptr->position_quad, T_Table_ptr->z, reg_descriptor[regA].vars);
            }
            // Case 9: x is a temporary, z is a temporary
            else if (isTemporary(T_Table_ptr->x) && isTemporary(T_Table_ptr->z))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                regT = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : ST %s %s\n",T_Table_ptr->position_quad, reg_descriptor[regT].vars, reg_descriptor[regA].vars);
            }
        }

        else if (strcmp(T_Table_ptr->oper, "goto") == 0)
        {
            // Generate an unconditional jump
            printf("\t%d    : JMP %s\n",T_Table_ptr->position_quad, T_Table_ptr->z); // Jump to label
        }
        else if (strcmp(T_Table_ptr->oper, "") == 0)
        {
            // No operation, just skip
        }
        else if (strcmp(T_Table_ptr->oper, "+") == 0 ||
                 strcmp(T_Table_ptr->oper, "-") == 0 ||
                 strcmp(T_Table_ptr->oper, "*") == 0 ||
                 strcmp(T_Table_ptr->oper, "/") == 0 ||
                 strcmp(T_Table_ptr->oper, "%") == 0)
        {
            int regA, regB, regZ;

            // Case 1: x is a number, y is a number
            if (is_number(T_Table_ptr->x) && is_number(T_Table_ptr->y))
            {
                regZ = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : LDI R%d %s\n",T_Table_ptr->position_quad ,regZ, T_Table_ptr->x);
                printf("\t        LDI R%d %s\n", regB, T_Table_ptr->y);
                printf("\t        %s R%d R%d\n", findOperation(T_Table_ptr->oper), regZ, regB); // Apply operator on numbers
            }
            // Case 2: x is a number, y is a variable
            else if (is_number(T_Table_ptr->x) && isVariable(T_Table_ptr->y))
            {
                regA = getRegister(T_Table_ptr->y, T_Table_ptr->y, 6, T_Table_ptr);
                regZ = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : LDI R%d %s\n",T_Table_ptr->position_quad, regZ, T_Table_ptr->x);
                printf("\t        %s R%d R%d\n", findOperation(T_Table_ptr->oper), regZ, regA); // Apply operator between number and variable
            }
            // Case 3: x is a number, y is a temporary
            else if (is_number(T_Table_ptr->x) && isTemporary(T_Table_ptr->y))
            {
                regA = getRegister(T_Table_ptr->y, T_Table_ptr->y, 6, T_Table_ptr);
                regZ = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : LDI R%d %s\n",T_Table_ptr->position_quad, regZ, T_Table_ptr->x);
                printf("\t        %s R%d R%d\n", findOperation(T_Table_ptr->oper), regZ, regA); // Apply operator between number and temporary
            }

            // Case 4: x is a variable, y is a number
            else if (isVariable(T_Table_ptr->x) && is_number(T_Table_ptr->y))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                regZ = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : LDI R%d %s\n",T_Table_ptr->position_quad, regZ, T_Table_ptr->y);
                printf("\t        %s R%d R%d\n", findOperation(T_Table_ptr->oper), regA, regZ); // Apply operator between variable and number
            }
            // Case 5: x is a variable, y is a variable
            else if (isVariable(T_Table_ptr->x) && isVariable(T_Table_ptr->y))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                regB = getRegister(T_Table_ptr->y, T_Table_ptr->y, 6, T_Table_ptr);
                regZ = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : LD R%d %s\n",T_Table_ptr->position_quad,regA,T_Table_ptr->x);
                printf("\t        LD R%d %s\n",regB,T_Table_ptr->y);
                printf("\t        %s R%d R%d\n", findOperation(T_Table_ptr->oper), regA, regB); // Apply operator between two variables
            }
            // Case 6: x is a variable, y is a temporary
            else if (isVariable(T_Table_ptr->x) && isTemporary(T_Table_ptr->y))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                regB = getRegister(T_Table_ptr->y, T_Table_ptr->y, 6, T_Table_ptr);
                regZ = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : LD R%d %s\n",T_Table_ptr->position_quad,regA,T_Table_ptr->x);
                printf("\t        %s R%d R%d\n", findOperation(T_Table_ptr->oper), regA, regB); // Apply operator between variable and temporary
            }

            // Case 7: x is a temporary, y is a number
            else if (isTemporary(T_Table_ptr->x) && is_number(T_Table_ptr->y))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                regZ = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : LDI R%d %s\n",T_Table_ptr->position_quad, regZ, T_Table_ptr->y);
                printf("\t        %s R%d R%d\n", findOperation(T_Table_ptr->oper), regA, regZ); // Apply operator between temporary and number
            }
            // Case 8: x is a temporary, y is a variable
            else if (isTemporary(T_Table_ptr->x) && isVariable(T_Table_ptr->y))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                regB = getRegister(T_Table_ptr->y, T_Table_ptr->y, 6, T_Table_ptr);
                regZ = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : %s R%d R%d\n",T_Table_ptr->position_quad, findOperation(T_Table_ptr->oper), regA, regB); // Apply operator between temporary and variable
            }
            // Case 9: x is a temporary, y is a temporary
            else if (isTemporary(T_Table_ptr->x) && isTemporary(T_Table_ptr->y))
            {
                regA = getRegister(T_Table_ptr->x, T_Table_ptr->x, 6, T_Table_ptr);
                regB = getRegister(T_Table_ptr->y, T_Table_ptr->y, 6, T_Table_ptr);
                regZ = getRegister(T_Table_ptr->z, T_Table_ptr->z, 6, T_Table_ptr);
                printf("\t%d    : %s R%d R%d\n",T_Table_ptr->position_quad, findOperation(T_Table_ptr->oper), regA, regB); // Apply operator between two temporaries
            }
        }

        T_Table_ptr = T_Table_ptr->ptr_next;
    }

    return 0;
}
