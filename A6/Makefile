 run: all
		./program < sample.txt > finalOutput.txt

all:
		yacc -d prog.y
		flex prog.l
		gcc -o program codegen.c


clean:
		rm -f lex.yy.c y.tab.c y.tab.h a.out  program
