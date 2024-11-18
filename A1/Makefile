# Compiler and flags
CC = gcc
LEX = flex
LDFLAGS = -ll

# Files
LEX_FILE = latex.l
C_FILE = procltx.c
OUTPUT_FILE = latex_parser
INPUT_FILE = A1.tex
OUTPUT_TXT = hello.txt

all: $(OUTPUT_FILE) run

# Generate the scanner
lex.yy.c: $(LEX_FILE)
	$(LEX) $(LEX_FILE)

# Compile the program
$(OUTPUT_FILE): lex.yy.c $(C_FILE)
	$(CC) $(C_FILE) -o $(OUTPUT_FILE) $(LDFLAGS)

# Run the program
run: $(OUTPUT_FILE)
	./$(OUTPUT_FILE) < $(INPUT_FILE) > $(OUTPUT_TXT)

# Clean generated files
clean:
	rm -f lex.yy.c $(OUTPUT_FILE) $(OUTPUT_TXT)

.PHONY: all run clean
