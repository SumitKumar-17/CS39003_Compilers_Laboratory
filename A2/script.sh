# flex list.l
# gcc lex.yy.c -o lexer -lfl
# ./lexer < biglist.txt > tokens.txt
# g++ -o evalexpr evalexpr.cpp -lfl
# ./evalexpr < tokens.txt >output.txt

# lex list.l
# gcc lex.yy.c -o lexer -lfl
# ./lexer <biglist.txt > tokens.txt
# g++ evalexpr.cpp -o evalexpr
# ./evalexpr


g++ -o evalexpr evalexpr.cpp 
./evalexpr < example.txt > output.txt