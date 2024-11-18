CXX = g++
CXXFLAGS = -Wall -g -I. -std=c++20
FLEX = flex
BISON = bison
LEX_FILE = tinyC3_22CS30056_22CS30015.l
YACC_FILE = tinyC3_22CS30056_22CS30015.y
PARSER = parser
OBJ = tinyC3_22CS30056_22CS30015.yy.o tinyC3_22CS30056_22CS30015.tab.o tinyC3_22CS30056_22CS30015_translator.o
TEST_DIR = input_tests
OUTPUT_DIR = output_tests

$(PARSER): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(PARSER)

tinyC3_22CS30056_22CS30015_translator.o: tinyC3_22CS30056_22CS30015_translator.cxx tinyC3_22CS30056_22CS30015_translator.h
	$(CXX) -c $(CXXFLAGS) tinyC3_22CS30056_22CS30015_translator.cxx

tinyC3_22CS30056_22CS30015.yy.o: tinyC3_22CS30056_22CS30015.yy.c tinyC3_22CS30056_22CS30015_translator.h
	$(CXX) -c $(CXXFLAGS) tinyC3_22CS30056_22CS30015.yy.c

tinyC3_22CS30056_22CS30015.tab.o: tinyC3_22CS30056_22CS30015.tab.c tinyC3_22CS30056_22CS30015_translator.h
	$(CXX) -c $(CXXFLAGS) tinyC3_22CS30056_22CS30015.tab.c

tinyC3_22CS30056_22CS30015.yy.c: $(LEX_FILE) tinyC3_22CS30056_22CS30015.tab.h
	$(FLEX) --outfile=tinyC3_22CS30056_22CS30015.yy.c $(LEX_FILE)

tinyC3_22CS30056_22CS30015.tab.c tinyC3_22CS30056_22CS30015.tab.h: $(YACC_FILE)
	$(BISON) -d --debug --report=all $(YACC_FILE)

clean:
	rm -f $(PARSER) $(OBJ) tinyC3_22CS30056_22CS30015.yy.* tinyC3_22CS30056_22CS30015.tab.* tinyC3_22CS30056_22CS30015.output

test: $(PARSER)
	@echo "Generating the ouput directory if not present. All the quads outputs are stored in this folder."
	mkdir -p $(OUTPUT_DIR)
	
	@echo "Running all the test cases."

	@echo "Running test 1."
	./$(PARSER) < $(TEST_DIR)/tinyC3_22CS30056_22CS30015_test1.c > $(OUTPUT_DIR)/tinyC3_22CS30056_22CS30015_quads1.txt
	@echo "Test 1 complete."

	@echo "Running test 2."
	./$(PARSER) < $(TEST_DIR)/tinyC3_22CS30056_22CS30015_test2.c > $(OUTPUT_DIR)/tinyC3_22CS30056_22CS30015_quads2.txt
	@echo "Test 2 complete."

	@echo "Running test 3."
	./$(PARSER) < $(TEST_DIR)/tinyC3_22CS30056_22CS30015_test3.c > $(OUTPUT_DIR)/tinyC3_22CS30056_22CS30015_quads3.txt
	@echo "Test 3 complete."

	@echo "Running test 4."
	./$(PARSER) < $(TEST_DIR)/tinyC3_22CS30056_22CS30015_test4.c > $(OUTPUT_DIR)/tinyC3_22CS30056_22CS30015_quads4.txt
	@echo "Test 4 complete."

	@echo "Running test 5."
	./$(PARSER) < $(TEST_DIR)/tinyC3_22CS30056_22CS30015_test5.c > $(OUTPUT_DIR)/tinyC3_22CS30056_22CS30015_quads5.txt
	@echo "Test 5 complete."

	@echo "Running test 6."
	./$(PARSER) < $(TEST_DIR)/tinyC3_22CS30056_22CS30015_test6.c > $(OUTPUT_DIR)/tinyC3_22CS30056_22CS30015_quads6.txt
	@echo "Test 6 complete."

	@echo "Running test 7."
	./$(PARSER) < $(TEST_DIR)/tinyC3_22CS30056_22CS30015_test7.c > $(OUTPUT_DIR)/tinyC3_22CS30056_22CS30015_quads7.txt
	@echo "Test 7 complete."

	@echo "Running test 8."
	./$(PARSER) < $(TEST_DIR)/tinyC3_22CS30056_22CS30015_test8.c > $(OUTPUT_DIR)/tinyC3_22CS30056_22CS30015_quads8.txt
	@echo "Test 8 complete."
