.PHONY: spec

all: bin/fn spec

CPP_FLAGS = -std=c++11 -Wno-deprecated-register
INCLUDES = -I.

# Find all .cpp files in src/ and strip the front.
CPP_FILES = `find ./src -name "*.cpp" | sed 's/\.\/src\///g'`
OBJ_FILES = $(CPP_FILES:.cpp=.o)

OBJ_DIR = obj
OBJ_FILENAMES = lex.o parse.o builtins.o runtime.o ast.o machine.o main.o
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(OBJ_FILENAMES))

SPEC_OBJ_FILENAMES = lex.o parse.o builtins.o runtime.o ast.o machine.o cli.o spec.o
SPEC_OBJS = $(patsubst %,$(OBJ_DIR)/%,$(SPEC_OBJ_FILENAMES))

### MAIN ###

bin/fn: $(OBJS)
	g++ $(CPP_FLAGS) -o $@ $(OBJS)

tmp/lex.cpp: src/parser/flex.cpp
	flex -o tmp/lex.cpp --header-file=tmp/lex.h src/parser/flex.cpp

$(OBJ_DIR)/lex.o: tmp/lex.cpp tmp/parse.cpp
	g++ -c -o $@ tmp/lex.cpp $(CPP_FLAGS) $(INCLUDES)

tmp/parse.cpp: src/parser/bison.cpp
	bison --graph -o tmp/parse.cpp --defines=tmp/parse.h src/parser/bison.cpp
	dot -Tpng tmp/parse.dot > tmp/parse.png

$(OBJ_DIR)/parse.o: tmp/parse.cpp tmp/lex.cpp
	g++ -c -o $@ tmp/parse.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/builtins.o: src/interpreter/builtins.cpp
	g++ -c -o $@ src/interpreter/builtins.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/runtime.o: src/interpreter/runtime.cpp
	g++ -c -o $@ src/interpreter/runtime.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/machine.o: src/interpreter/machine.cpp
	g++ -c -o $@ src/interpreter/machine.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/ast.o: src/ast.cpp
	g++ -c -o $@ src/ast.cpp $(CPP_FLAGS) $(INCLUDES)


$(OBJ_DIR)/cli.o: src/cli.cpp
	g++ -c -o $@ src/cli.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/main.o: src/main.cpp
	g++ -c -o $@ src/main.cpp $(CPP_FLAGS) $(INCLUDES)



### SPECS ###

spec: tmp/spec
	./tmp/spec

tmp/spec: $(SPEC_OBJS)
	g++ $(CPP_FLAGS) -o $@ $(SPEC_OBJS)

$(OBJ_DIR)/spec.o:
	g++ -c -o $@ spec/spec.cpp $(CPP_FLAGS) $(INCLUDES)



### DEV TOOLS ###

clean:
	rm -f tmp/* obj/*

watch: # Mac-specific!
	fswatch -or src spec | xargs -I{} make spec
