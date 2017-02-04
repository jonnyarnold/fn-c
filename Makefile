# TODO: This is getting a little unruly.
# Time to move to something a little more automated!

.PHONY: spec

all: bin/fn spec

CPP_FLAGS = -g -std=c++11 -Wno-deprecated-register
INCLUDES = -I.

# Find all .cpp files in src/ and strip the front.
CPP_FILES = `find ./src -name "*.cpp" | sed 's/\.\/src\///g'`
OBJ_FILES = $(CPP_FILES:.cpp=.o)

OBJ_DIR = obj
OBJ_FILENAMES = lex.o parse.o parser.o value.o def.o list.o world.o ast.o machine.o number.o string.o exec.o
MAIN_OBJ_FILENAMES = $(OBJ_FILENAMES) main.o
MAIN_OBJS = $(patsubst %,$(OBJ_DIR)/%,$(MAIN_OBJ_FILENAMES))

SPEC_OBJ_FILENAMES = $(OBJ_FILENAMES) spec.o language.spec.o world.spec.o bool.spec.o def.spec.o list.spec.o number.spec.o block.spec.o string.spec.o value.spec.o
SPEC_OBJS = $(patsubst %,$(OBJ_DIR)/%,$(SPEC_OBJ_FILENAMES))

### MAIN ###

bin/fn: $(MAIN_OBJS)
	g++ $(CPP_FLAGS) -o $@ $(MAIN_OBJS)

tmp/lex.cpp: src/parser/flex.cpp
	flex -o tmp/lex.cpp --header-file=tmp/lex.h src/parser/flex.cpp

$(OBJ_DIR)/lex.o: tmp/lex.cpp tmp/parse.cpp
	g++ -c -o $@ tmp/lex.cpp $(CPP_FLAGS) $(INCLUDES)

tmp/parse.cpp: src/parser/bison.cpp
	bison --report=state -o tmp/parse.cpp --defines=tmp/parse.h src/parser/bison.cpp

$(OBJ_DIR)/parse.o: tmp/parse.cpp tmp/lex.cpp
	g++ -c -o $@ tmp/parse.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/parser.o: src/parser.cpp
	g++ -c -o $@ src/parser.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/value.o: src/interpreter/objects/value.cpp
	g++ -c -o $@ src/interpreter/objects/value.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/block.o: src/interpreter/objects/block.cpp
	g++ -c -o $@ src/interpreter/objects/block.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/def.o: src/interpreter/objects/def.cpp
	g++ -c -o $@ src/interpreter/objects/def.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/list.o: src/interpreter/objects/list.cpp
	g++ -c -o $@ src/interpreter/objects/list.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/world.o: src/interpreter/world.cpp
	g++ -c -o $@ src/interpreter/world.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/number.o: src/interpreter/objects/number.cpp
	g++ -c -o $@ src/interpreter/objects/number.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/string.o: src/interpreter/objects/string.cpp
	g++ -c -o $@ src/interpreter/objects/string.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/machine.o: src/interpreter/machine.cpp
	g++ -c -o $@ src/interpreter/machine.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/ast.o: src/ast.cpp
	g++ -c -o $@ src/ast.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/cli.o: src/cli.cpp
	g++ -c -o $@ src/cli.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/main.o: src/main.cpp
	g++ -c -o $@ src/main.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/exec.o: src/exec.cpp
	g++ -c -o $@ src/exec.cpp $(CPP_FLAGS) $(INCLUDES)



### SPECS ###

spec: tmp/spec
	./tmp/spec

tmp/spec: $(SPEC_OBJS)
	g++ $(CPP_FLAGS) -o $@ $(SPEC_OBJS)

$(OBJ_DIR)/spec.o: spec/spec.cpp
	g++ -c -o $@ spec/spec.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/language.spec.o: spec/e2e/language.spec.cpp
	g++ -c -o $@ spec/e2e/language.spec.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/world.spec.o: spec/e2e/world.spec.cpp
	g++ -c -o $@ spec/e2e/world.spec.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/bool.spec.o: spec/e2e/bool.spec.cpp
	g++ -c -o $@ spec/e2e/bool.spec.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/def.spec.o: spec/e2e/def.spec.cpp
	g++ -c -o $@ spec/e2e/def.spec.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/list.spec.o: spec/e2e/list.spec.cpp
	g++ -c -o $@ spec/e2e/list.spec.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/number.spec.o: spec/e2e/number.spec.cpp
	g++ -c -o $@ spec/e2e/number.spec.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/block.spec.o: spec/e2e/block.spec.cpp
	g++ -c -o $@ spec/e2e/block.spec.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/string.spec.o: spec/e2e/string.spec.cpp
	g++ -c -o $@ spec/e2e/string.spec.cpp $(CPP_FLAGS) $(INCLUDES)

$(OBJ_DIR)/value.spec.o: spec/e2e/value.spec.cpp
	g++ -c -o $@ spec/e2e/value.spec.cpp $(CPP_FLAGS) $(INCLUDES)



### DEV TOOLS ###

clean:
	rm -rf tmp/* obj/* bin/*

watch-mac: # Mac-specific!
	fswatch -or src spec | xargs -I{} make spec

watch-linux:
	while inotifywait -qq -e close_write -r .; do make; done



### VM STUFF ###

obj/vm.o: src/vm/vm.cpp src/vm/vm.h
	g++ -c -o $@ src/vm/vm.cpp $(CPP_FLAGS) $(INCLUDES)

obj/bool.spec.o: spec/spec.cpp spec/spec.h spec/vm/bool.spec.cpp
	g++ -c -o $@ spec/vm/bool.spec.cpp $(CPP_FLAGS) $(INCLUDES)

obj/number.spec.o: spec/spec.cpp spec/spec.h spec/vm/number.spec.cpp
	g++ -c -o $@ spec/vm/number.spec.cpp $(CPP_FLAGS) $(INCLUDES)

tmp/vm_spec: obj/vm.o obj/bool.spec.o obj/number.spec.o obj/spec.o
	g++ $(CPP_FLAGS) -o $@ $^

vm: tmp/vm_spec
	./tmp/vm_spec
