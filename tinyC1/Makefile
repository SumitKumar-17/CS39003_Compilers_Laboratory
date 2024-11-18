LEX_FILE = ass3_22CS30056_22CS30015.l
HEADER_FILE = ass3_22CS30056_22CS30015.h
C_FILE = ass3_22CS30056_22CS30015.c
C_TEST_FILE =ass3_22CS30056_22CS30015_test.c
EXECUTABLE = ass3_22CS30056_22CS30015_executable
OUTPUT_FILE= result.txt

LEX = flex
CC = gcc
CFLAGS = -Wall -g -Wno-unused-function

all:  $(EXECUTABLE)

lex.yy.c: $(LEX_FILE)
	$(LEX) $(LEX_FILE)	

$(EXECUTABLE): lex.yy.c $(C_FILE) $(HEADER_FILE)
	$(CC) $(CFLAGS)  $(C_FILE) -o $(EXECUTABLE)

clean:
	rm -f lex.yy.c $(EXECUTABLE)
	rm -rf $(OUTPUT_FILE)

run: $(EXECUTABLE)
	./$(EXECUTABLE) < $(C_TEST_FILE) > $(OUTPUT_FILE)

open:
	open $(OUTPUT_FILE)

run_and_open_file:$(EXECUTABLE)
	./$(EXECUTABLE) < $(C_TEST_FILE) > $(OUTPUT_FILE)
	open $(OUTPUT_FILE)

.PHONY: all clean run
