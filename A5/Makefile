LEX_FILE = expr.l
HEADER_FILE = expr.h
C_FILE = intcodegen.c
EXECUTABLE = icgen
OUTPUT_FILE= output_icg.txt
SAMPLES_FOLDER=samples
TEST_FILE =sample.txt
GENERATE_C_FILE=intcode.c
THREE_ADDRESS_FILE_OUTPUT=final_output

BASENAME = expr
MAINBASENAME = intcodegen

LEX = flex
CC = gcc
CFLAGS = -Wall -g -Wno-unused-function -lm
SAMPLES := $(shell seq 1 100)

all: run icgen

run: icgen
	./icgen < $(TEST_FILE) > $(GENERATE_C_FILE)
	gcc $(GENERATE_C_FILE) -o ic
	./ic > $(THREE_ADDRESS_FILE_OUTPUT).txt

icgen: $(BASENAME).yy.o $(BASENAME).tab.o $(MAINBASENAME).o
	$(CC) $(CFLAGS) $(BASENAME).yy.o $(BASENAME).tab.o $(MAINBASENAME).o -o icgen

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

clean:
	rm -f $(BASENAME).yy.c $(BASENAME).tab.c $(BASENAME).tab.h $(BASENAME).yy.o $(BASENAME).tab.o $(MAINBASENAME).o icgen $(OUTPUT_FILE) $(BASENAME).output  $(GENERATE_C_FILE) ic $(THREE_ADDRESS_FILE_OUTPUT).txt
