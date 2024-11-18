%{

#define OB 1
#define CB 2
#define NUM 3
#define ID 4
#define OP 5

%}

space          [ \t\n]
spaces         {space}*
digit          [0-9]
num         [-]?{digit}+
id             [a-zA-Z_][a-zA-Z0-9]*
              
%%

{spaces}    {}
"("        {return (OB);}
")"        {return (CB);}
{num}   {return (NUM);}
{id}       {return (ID);}
"+"        {return (OP);}
"-"        {return (OP);}
"/"        {return (OP);}
"*"        {return (OP);}
"%"        {return (OP);}
"&&"       {return (OP);}
"||"       {return (OP);}
"!"        {return (OP);}
"=="       {return (OP);}
"!="       {return (OP);}
%%