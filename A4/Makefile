LEX_FILE = poly.l
HEADER_FILE = poly.h
C_FILE = polyutils.c
C_TEST_FILE =sample.txt
EXECUTABLE = poly_executable
OUTPUT_FILE= output_poly.txt

BASENAME = poly
MAINBASENAME = polyutils

LEX = flex
CC = gcc
CFLAGS = -Wall -g -Wno-unused-function -lm
SAMPLES := $(shell seq 1 100)


$(OUTPUT_FILE):parser $(C_TEST_FILE)
	./parser < $(C_TEST_FILE) > $(OUTPUT_FILE)

parser: $(BASENAME).yy.o $(BASENAME).tab.o $(MAINBASENAME).o
	$(CC) $(CFLAGS) $(BASENAME).yy.o $(BASENAME).tab.o $(MAINBASENAME).o -o parser

$(MAINBASENAME).o: $(C_FILE)
	$(CC) -c $(CFLAGS) $(C_FILE) 

$(BASENAME).yy.o: $(BASENAME).yy.c 
	$(CC) -c $(CFLAGS) $(BASENAME).yy.c

$(BASENAME).tab.o: $(BASENAME).tab.c
	$(CC) -c $(CFLAGS) $(BASENAME).tab.c

$(BASENAME).yy.c: $(LEX_FILE) $(BASENAME).tab.h
	$(LEX) --outfile=$(BASENAME).yy.c $(LEX_FILE)

$(BASENAME).tab.c $(BASENAME).tab.h: $(BASENAME).y
	bison -d --debug --report=all $(BASENAME).y

testing: parser
	cd testinputs && python generate.py
	cd ..
	mkdir -p testoutputs

	@echo "Running test cases..."
	@for i in $(SAMPLES); do \
		echo "Running Sample Case $$i"; \
		./parser < testinputs/test$$i.txt > testoutputs/output$$i.txt; \
		echo "Completed Sample Case $$i"; \
	done

testing_clean:
	rm -rf testoutputs
	@echo "Deleting test cases..."
	@for i in $(SAMPLES); do \
		echo "Deleting Sample Case $$i"; \
		rm testinputs/test$$i.txt; \
		echo "Completed Deleting Sample Case $$i"; \
	done

clean:
	rm -f $(BASENAME).yy.c $(BASENAME).tab.c $(BASENAME).tab.h $(BASENAME).yy.o $(BASENAME).tab.o $(MAINBASENAME).o parser $(OUTPUT_FILE) $(BASENAME).output

.PHONY: all clean run
