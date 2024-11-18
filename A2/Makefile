
EXECUTABLE = evalexpr

LEX_FILE = list.l
CPP_FILE = evalexpr.cpp

LEX_C_FILE = lex.yy.c

LIST_INPUT = list.txt
BIGLIST_INPUT = biglist.txt

OUTPUT_FILE = output.txt

all: $(EXECUTABLE)

$(LEX_C_FILE): $(LEX_FILE)
	flex $(LEX_FILE)

$(EXECUTABLE): $(LEX_C_FILE) $(CPP_FILE)
	g++ -o $(EXECUTABLE) $(CPP_FILE) 

run_listexample: $(EXECUTABLE)
	./$(EXECUTABLE) < $(LIST_INPUT) > $(OUTPUT_FILE)

run_biglistexample: $(EXECUTABLE)
	./$(EXECUTABLE) < $(BIGLIST_INPUT) > $(OUTPUT_FILE)

clean:
	rm -f $(EXECUTABLE) $(LEX_C_FILE) $(OUTPUT_FILE)

flex_compile: $(LEX_FILE)
	flex $(LEX_FILE)
