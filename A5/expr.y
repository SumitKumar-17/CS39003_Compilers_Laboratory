%{

    #include "expr.h"
    #include <stdio.h>

    #include <stdlib.h>
    #include <string.h>
    #define YYDEBUG 1


%}

%union
{
    int num;
    char *str;
    struct{
        char *type;
        int reg;
    } typed_val;
}

%type <typed_val> EXPR ARG  EXPRSTMT STMT SETSTMT

%token <str> ID
%token <num> NUM
%token SET PLUS MINUS MULT DIV MOD POW
%token LEFT_PARENTHESIS
%token RIGHT_PARENTHESIS

%left PLUS MINUS
%left MULT DIV MOD
%right POW

%start PROGRAM

%%

PROGRAM:
    STMT PROGRAM
    | STMT
    | error PROGRAM { yyerrok;}
    ;

STMT:
    SETSTMT 
    | EXPRSTMT
    ;

SETSTMT:
     LEFT_PARENTHESIS SET ID NUM RIGHT_PARENTHESIS
    {
        int mem_loc=get_memory_loc($3);
        if(mem_loc==-1){
            insert_symbol($3,memory_offset_value++);
            memory[memory_offset_value-1]=$4;
            mem_loc=memory_offset_value-1;
        }else{
            memory[mem_loc]=$4;
        }
            printf("MEM[%d] = %d;\n",mem_loc,$4);
            printf("mprn(MEM,%d);\n",mem_loc);
    }
    | LEFT_PARENTHESIS SET ID ID RIGHT_PARENTHESIS
    {
       int mem_loc_2=get_memory_loc($4);
       if(mem_loc_2!=-1)
       {
           int mem_loc_1=get_memory_loc($3);
           if(mem_loc_1==-1){
               insert_symbol($3,memory_offset_value++);
                memory[memory_offset_value-1]=memory[mem_loc_2];
                mem_loc_1=memory_offset_value-1;
           }else{
                memory[mem_loc_1]=memory[mem_loc_2];
           }
                // bool reg_mem_storage=false;
               //first find a free regsiter then store that value into it anf then generate the three address codes
                // int free_reg=get_first_free_register(&reg_mem_storage);
                if(register_zero_free){
                    printf("R[0] = MEM[%d];\n",mem_loc_2);
                    printf("MEM[%d] = R[0];\n",mem_loc_1);
                } else if(!register_zero_free && register_one_free){
                    printf("R[1] = MEM[%d];\n",mem_loc_2);
                    printf("MEM[%d] = R[1];\n",mem_loc_1);
                } else {
                    //implement the case when both registers are busy
                    // char *temp_var="$"+to_string(temp_var_counter++);
                    // char temp_var  [20];
                    // sprintf(temp_var,"$%d",temp_var_counter++);
                    // insert_symbol(temp_var,memory_offset_value++);
                    // printf("MEM[%d] = R[0];\n",memory_offset_value-1);
                    // printf("R[0] = MEM[%d];\n",mem_loc_2);
                    // printf("MEM[%d] = R[0];\n",mem_loc_1);
                    // printf("here");
                    // printf("R[0] = MEM[%d];\n",memory_offset_value-1);
                     int reg;
                    bool reg_mem_storage;
                    reg=get_first_free_register(&reg_mem_storage);
                    if(!reg_mem_storage){
                        if(reg>=2 && reg<=11){
                            //store the R[0] in a temporary memory  register
                            printf("R[%d] = MEM[%d];\n",reg,mem_loc_2);
                            printf("MEM[%d] = R[%d];\n",mem_loc_1,reg);
                            // printf("//from upper\n");
                        }
                    }
                    else {
                        char temp_var  [20];
                        sprintf(temp_var,"$%d",temp_var_counter++);
                        insert_symbol(temp_var,memory_offset_value++);
                        printf("MEM[%d] = R[0];",memory_offset_value-1);
                        printf("R[0] = MEM[%d];\n",mem_loc_2);
                    }
                    // int reg;
                    // bool reg_mem_storage;
                    // reg=get_first_free_register(&reg_mem_storage);
                    // printf("R[%d] = MEM[%d];\n",reg,mem_loc_2);
                    // printf("MEM[%d] = R[%d];\n",mem_loc_1,reg);
                    // make_register_free(reg);
                } 
                
                // if(reg_mem_storage)
                // {
                //     //for this case working left
                    
                //     printf("R[%d] = MEM[%d];\n",free_reg,mem_loc_1);
                // }
                // else{
                //     printf("R[%d] = MEM[%d];\n",free_reg,mem_loc_2);
                //     printf("MEM[%d] = R[%d];\n",mem_loc_1,free_reg);
                //     make_register_free(free_reg);
                // }
                printf("mprn(MEM,%d);\n",mem_loc_1);
       }else{
              yyerror("Variable %s not declared", $4);
              YYERROR;
    }
    }
    | LEFT_PARENTHESIS SET ID EXPR RIGHT_PARENTHESIS
    {
        int reg = $4.reg;
        int mem_loc = get_memory_loc($3);
        if (mem_loc == -1) {
            insert_symbol($3, memory_offset_value++);
            mem_loc = memory_offset_value - 1;
        }
        printf("MEM[%d] = R[%d];\n", mem_loc, reg);
        printf("mprn(MEM,%d);\n", mem_loc);
        make_register_free(reg);
    }
    ;

EXPRSTMT:
    EXPR
    {
        $$.type=strdup("EXPR");
        $$.reg=$1.reg;
        printf("eprn(R,%d);\n",$1.reg);
        make_register_free($1.reg);
    }
    ;

EXPR:
    LEFT_PARENTHESIS PLUS ARG ARG RIGHT_PARENTHESIS
    {
        int reg;
        bool reg_mem_storage;
        if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("check-1");
            printf("R[%d] = R[%d] +  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "NUM") == 0) {
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = %d + %d;\n", reg, $3.reg, $4.reg);
            $$.reg=reg;
        } 
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-2");
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = R[%d] +  R[%d];\n", reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "EXPR") == 0) {
            printf("R[%d] = %d + R[%d];\n", $4.reg, $3.reg, $4.reg);
            // make_register_free($4.reg); 
            $$.reg=$4.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "NUM") == 0) {
            printf("R[%d] = R[%d] + %d;\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg); 
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-3");
            printf("R[%d] = R[%d] +  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("cehck-5");
            printf("R[%d] = R[%d] +  R[%d];\n", $4.reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            // make_register_free($4.reg);
            $$.reg=$4.reg;
        }else if(strcmp($3.type,"NUM")==0 && strcmp($4.type,"ID")==0){
             reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = %d + R[%d];\n",reg,$3.reg,$4.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if(strcmp($3.type,"ID")==0 && strcmp($4.type,"NUM")==0){
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = R[%d] + %d;\n",reg,$3.reg,$4.reg);
            make_register_free($3.reg);
            $$.reg=reg;
        }
        $$.type = strdup("EXPR");
        // printf("coming here");
        // $$.reg = reg;

    }
    | LEFT_PARENTHESIS MINUS ARG ARG RIGHT_PARENTHESIS
    {
        int reg;
        bool reg_mem_storage;
        if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("check-1");
            printf("R[%d] = R[%d] -  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "NUM") == 0) {
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = %d - %d;\n", reg, $3.reg, $4.reg);
            $$.reg=reg;
        } 
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-2");
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = R[%d] -  R[%d];\n", reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "EXPR") == 0) {
            printf("R[%d] = %d - R[%d];\n", $4.reg, $3.reg, $4.reg);
            // make_register_free($4.reg); 
            $$.reg=$4.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "NUM") == 0) {
            printf("R[%d] = R[%d] - %d;\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg); 
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-3");
            printf("R[%d] = R[%d] -  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("cehck-5");
            printf("R[%d] = R[%d] -  R[%d];\n", $4.reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            // make_register_free($4.reg);
            $$.reg=$4.reg;
        }else if(strcmp($3.type,"NUM")==0 && strcmp($4.type,"ID")==0){
             reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = %d - R[%d];\n",reg,$3.reg,$4.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if(strcmp($3.type,"ID")==0 && strcmp($4.type,"NUM")==0){
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = R[%d] - %d;\n",reg,$3.reg,$4.reg);
            make_register_free($3.reg);
            $$.reg=reg;
        }
        $$.type=strdup("EXPR");
        // $$.reg=reg;
    }    
    | LEFT_PARENTHESIS MULT ARG ARG RIGHT_PARENTHESIS
    {
        int reg;
        bool reg_mem_storage;
        if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("check-1");
            printf("R[%d] = R[%d] *  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "NUM") == 0) {
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = %d * %d;\n", reg, $3.reg, $4.reg);
            $$.reg=reg;
        } 
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-2");
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = R[%d] *  R[%d];\n", reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "EXPR") == 0) {
            printf("R[%d] = %d * R[%d];\n", $4.reg, $3.reg, $4.reg);
            // make_register_free($4.reg); 
            $$.reg=$4.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "NUM") == 0) {
            printf("R[%d] = R[%d] * %d;\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg); 
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-3");
            printf("R[%d] = R[%d] *  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("cehck-5");
            printf("R[%d] = R[%d] *  R[%d];\n", $4.reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            // make_register_free($4.reg);
            $$.reg=$4.reg;
        }else if(strcmp($3.type,"NUM")==0 && strcmp($4.type,"ID")==0){
             reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = %d * R[%d];\n",reg,$3.reg,$4.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if(strcmp($3.type,"ID")==0 && strcmp($4.type,"NUM")==0){
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = R[%d] * %d;\n",reg,$3.reg,$4.reg);
            make_register_free($3.reg);
            $$.reg=reg;
        }
        $$.type=strdup("EXPR");
        // $$.reg=reg;
    }
    | LEFT_PARENTHESIS DIV ARG ARG RIGHT_PARENTHESIS
    {
        int reg;
        bool reg_mem_storage;
        if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("check-1");
            printf("R[%d] = R[%d] /  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "NUM") == 0) {
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = %d / %d;\n", reg, $3.reg, $4.reg);
            $$.reg=reg;
        } 
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-2");
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = R[%d] /  R[%d];\n", reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "EXPR") == 0) {
            printf("R[%d] = %d / R[%d];\n", $4.reg, $3.reg, $4.reg);
            // make_register_free($4.reg); 
            $$.reg=$4.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "NUM") == 0) {
            printf("R[%d] = R[%d] / %d;\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg); 
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-3");
            printf("R[%d] = R[%d] /  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("cehck-5");
            printf("R[%d] = R[%d] /  R[%d];\n", $4.reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            // make_register_free($4.reg);
            $$.reg=$4.reg;
        }else if(strcmp($3.type,"NUM")==0 && strcmp($4.type,"ID")==0){
             reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = %d / R[%d];\n",reg,$3.reg,$4.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if(strcmp($3.type,"ID")==0 && strcmp($4.type,"NUM")==0){
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = R[%d] / %d;\n",reg,$3.reg,$4.reg);
            make_register_free($3.reg);
            $$.reg=reg;
        }
        $$.type=strdup("EXPR");
        // $$.reg=reg;
    }
    | LEFT_PARENTHESIS MOD ARG ARG RIGHT_PARENTHESIS
    {
        int reg;
        bool reg_mem_storage;
        if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("check-1");
            printf("R[%d] = R[%d] %%  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "NUM") == 0) {
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = %d %% %d;\n", reg, $3.reg, $4.reg);
            $$.reg=reg;
        } 
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-2");
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = R[%d] %%  R[%d];\n", reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "EXPR") == 0) {
            printf("R[%d] = %d %% R[%d];\n", $4.reg, $3.reg, $4.reg);
            // make_register_free($4.reg); 
            $$.reg=$4.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "NUM") == 0) {
            printf("R[%d] = R[%d] %% %d;\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg); 
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-3");
            printf("R[%d] = R[%d] %%  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("cehck-5");
            printf("R[%d] = R[%d] %%  R[%d];\n", $4.reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            // make_register_free($4.reg);
            $$.reg=$4.reg;
        }else if(strcmp($3.type,"NUM")==0 && strcmp($4.type,"ID")==0){
             reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = %d %% R[%d];\n",reg,$3.reg,$4.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if(strcmp($3.type,"ID")==0 && strcmp($4.type,"NUM")==0){
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = R[%d] %% %d;\n",reg,$3.reg,$4.reg);
            make_register_free($3.reg);
            $$.reg=reg;
        }
        $$.type=strdup("EXPR");
        // $$.reg=reg;
    }

    | LEFT_PARENTHESIS POW ARG ARG RIGHT_PARENTHESIS
    {
        int reg;
        bool reg_mem_storage;
        if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("check-1");
            printf("R[%d] = pwr(R[%d],  R[%d];\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "NUM") == 0) {
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = pwr(%d, %d;\n", reg, $3.reg, $4.reg);
            $$.reg=reg;
        } 
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-2");
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = pwr(R[%d],  R[%d]);\n", reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if (strcmp($3.type, "NUM") == 0 && strcmp($4.type, "EXPR") == 0) {
            printf("R[%d] = pwr(%d, R[%d]);\n", $4.reg, $3.reg, $4.reg);
            // make_register_free($4.reg); 
            $$.reg=$4.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "NUM") == 0) {
            printf("R[%d] = pwr(R[%d], %d);\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg); 
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "EXPR") == 0 && strcmp($4.type, "ID") == 0) {
            // printf("check-3");
            printf("R[%d] = pwr(R[%d],  R[%d]);\n", $3.reg, $3.reg, $4.reg);
            // make_register_free($3.reg);
            make_register_free($4.reg);
            $$.reg=$3.reg;
        }
        else if (strcmp($3.type, "ID") == 0 && strcmp($4.type, "EXPR") == 0) {
            // printf("cehck-5");
            printf("R[%d] = pwr(R[%d],  R[%d]);\n", $4.reg, $3.reg, $4.reg);
            make_register_free($3.reg);
            // make_register_free($4.reg);
            $$.reg=$4.reg;
        }else if(strcmp($3.type,"NUM")==0 && strcmp($4.type,"ID")==0){
             reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = pwr(%d, R[%d]);\n",reg,$3.reg,$4.reg);
            make_register_free($4.reg);
            $$.reg=reg;
        }
        else if(strcmp($3.type,"ID")==0 && strcmp($4.type,"NUM")==0){
            reg = get_first_free_register(&reg_mem_storage);
            printf("R[%d] = pwr(R[%d], %d);\n",reg,$3.reg,$4.reg);
            make_register_free($3.reg);
            $$.reg=reg;
        }
        $$.type=strdup("EXPR");
        // $$.reg=reg;
    }
    ;

ARG:
    ID
    {
        $$.type=strdup("ID");
        if(register_zero_free){
            printf("R[0] = MEM[%d];\n",get_memory_loc($1));
            register_zero_free=0;
            $$.reg=0;
        } else if(!register_zero_free && register_one_free){
            printf("R[1] = MEM[%d];\n",get_memory_loc($1));
            register_one_free=0;
            $$.reg=1;
        } else {
            //implement the case when both registers are busy
            // we wil store the reg[0] value in th symbol table and then free the value th reg[0]
            // char temp_var  [20];
            // sprintf(temp_var,"$%d",temp_var_counter++);
            // insert_symbol(temp_var,memory_offset_value++);
            // printf("MEM[%d] = R[0];\n",memory_offset_value-1);
            // printf("R[0] = MEM[%d];\n",get_memory_loc($1));
            // register_zero_free=0;
            // $$.reg=0;
            int reg;
            bool reg_mem_storage;
            reg=get_first_free_register(&reg_mem_storage);
            if(!reg_mem_storage){
                if(reg>=2 && reg<=11){
                    //store the R[0] in a temporary memory  register
                    printf("R[%d] = MEM[%d];\n",reg,get_memory_loc($1));
                    $$.reg=reg;
                    // printf("//from below\n");
                }
            } else {
                char temp_var  [20];
                sprintf(temp_var,"$%d",temp_var_counter++);
                insert_symbol(temp_var,memory_offset_value++);
                printf("MEM[%d] = R[0];",memory_offset_value-1);
                printf("R[0] = MEM[%d];\n",get_memory_loc($1));

            }
            
        }
        // int reg;
        // bool reg_mem_storage;
        // reg=get_first_free_register(&reg_mem_storage);
        // printf("R[%d] = MEM[%d];\n",reg,get_memory_loc($1));
        // $$.reg=reg;
    }
    | NUM
    {
        $$.type=strdup("NUM");
        $$.reg=$1;
    }
    | EXPR
    {   
        $$.type=strdup("EXPR");
        $$.reg=$1.reg;
    }
    ;

%%
