LEX_FILE = tinyC2_22CS30056_22CS30015.l
HEADER_FILE = tinyC2_22CS30056_22CS30015.h
C_FILE = tinyC2_22CS30056_22CS30015_main.c
C_TEST_FILE =input_22CS30056_22CS30015.c
EXECUTABLE = tincyC2__22CS30056_22CS30015_executable
OUTPUT_FILE= output_22CS30056_22CS30015.txt

BASENAME = tinyC2_22CS30056_22CS30015
MAINBASENAME = tinyC2_22CS30056_22CS30015_main

LEX = flex
CC = gcc
CFLAGS = -Wall -g -Wno-unused-function


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

clean:
	rm -f $(BASENAME).yy.c $(BASENAME).tab.c $(BASENAME).tab.h $(BASENAME).yy.o $(BASENAME).tab.o $(MAINBASENAME).o parser $(OUTPUT_FILE) $(BASENAME).output

.PHONY: all clean run
