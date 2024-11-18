SAMPLES := $(shell seq 1 11)

parser: expr.yy.o expr.tab.o expr_translator.o
	g++ -Wall -I. -std=c++20 expr.yy.o expr.tab.o expr_translator.o -o parser

expr_translator.o: expr_translator.cpp expr_translator.h
	g++ -c -Wall -I. -std=c++20 expr_translator.cpp

expr.yy.o: expr.yy.c expr_translator.h
	g++ -c -Wall -I. -std=c++20 expr.yy.c

expr.tab.o: expr.tab.c expr_translator.h
	g++ -c -Wall -I. -std=c++20 expr.tab.c

expr.yy.c: expr.l expr.tab.h
	flex --outfile=expr.yy.c expr.l

expr.tab.c expr.tab.h: expr.y
	bison -d --debug --report=all expr.y

clean:
	rm parser expr_translator.o expr.yy.* expr.tab.* expr.output


run:parser 
	mkdir -p output
	@echo "Running test Case 1 "
	./parser < tests/test_1.txt > output/output_test_1.txt
	@echo "Completed test Case 1"


testing: parser
	mkdir -p output
	@echo "Running Test Case 1 "
	./parser < tests/test_1.txt > output/output_test_1.txt
	@echo "Completed Test Case 1"

	@echo "Running test cases..."
	@for i in $(SAMPLES); do \
		echo "Running Sample Case $$i"; \
		./parser < tests/sample_$$i.txt > output/output_sample_$$i.txt; \
		echo "Completed Sample Case $$i"; \
	done

	
