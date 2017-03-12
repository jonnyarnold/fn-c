# Builds the fn language.


### Variables:
# The compilation command.
COMPILE=g++ -std=c++11 -Wno-deprecated-register -I.
COMPILE_BIN=$(COMPILE) -L.
COMPILE_OBJ=$(COMPILE) -c



### Makefile Tasks:
# `all` is the default, and will build in development mode:
all: test
new: clean all

# `test` will compile fn with debugging symbols and build the specs.
test: COMPILE+=-g
test: bin/fn tmp/spec
	./tmp/spec

# TODO: Production compilation

# `clean` removes all build artifacts and temporary files.
clean:
	rm -rf tmp/* obj/* bin/*

# There are two `watch` tasks, depending on the environment.
# (Sorry Windows, nothing for you here.)
watch-mac:
	fswatch -or src spec | xargs -I{} make spec
watch-linux:
	while inotifywait -qq -e close_write -r .; do make; done



# The fn language binary is a Command Line Interface (CLI)
# that executes Fn code.
#
# TODO: Tidy this up! Can't we get some clever partial compilation somewhere?
bin/fn: src/main.cpp tmp/cli.o obj/exec.o obj/parser.o tmp/parse.o obj/codegen.o obj/bytecode.o obj/vm.o tmp/lex.cpp
	$(COMPILE_BIN) -o $@ $^

# The Command Line Interface is built as an object to avoid
# a double-definition of main() in the specs.
tmp/cli.o: src/cli.cpp src/cli.h obj/exec.o
	$(COMPILE_OBJ) -o $@ src/cli.cpp

# To execute Fn code, we do it in three steps:
#
# - parser: convert Fn code to an AST.
# - codegen: convert an AST into VM instructions.
# - vm: execute VM instructions.
obj/exec.o: src/exec.cpp src/exec.h obj/parser.o obj/codegen.o obj/vm.o 
	$(COMPILE_OBJ) -o $@ src/exec.cpp



# The parser uses Flex and Bison to build a parser;
# we tack a C++ interface onto it.
obj/parser.o: src/parser/parser.cpp src/parser/parser.h tmp/parse.o 
	$(COMPILE_OBJ) -o $@ src/parser/parser.cpp

tmp/parse.o: tmp/parse.cpp tmp/parse.h tmp/lex.cpp tmp/lex.h
	$(COMPILE_OBJ) -o $@ tmp/parse.cpp

tmp/lex.cpp tmp/lex.h: src/parser/flex.cpp
	flex -o tmp/lex.cpp --header-file=tmp/lex.h $^

tmp/parse.cpp tmp/parse.h: src/parser/bison.cpp
	bison --report=state -o tmp/parse.cpp --defines=tmp/parse.h $^



# The Code Generator converts the AST
# into VM instructions.
obj/codegen.o: src/codegen/codegen.cpp src/codegen/codegen.h
	$(COMPILE_OBJ) -o $@ src/codegen/codegen.cpp

obj/bytecode.o: src/bytecode.cpp src/bytecode.h
	$(COMPILE_OBJ) -o $@ src/bytecode.cpp


# The VM is homegrown, baby!
obj/vm.o: src/vm/vm.cpp src/vm/vm.h src/number.h
	$(COMPILE_OBJ) -o $@ src/vm/vm.cpp



# The specs are built with Catch,
# a cool C++ testing framework.
tmp/spec: spec/spec.cpp obj/bool.spec.o obj/number.spec.o obj/load.spec.o obj/call.spec.o tmp/cli.o obj/exec.o obj/parser.o tmp/parse.o obj/codegen.o obj/bytecode.o obj/vm.o tmp/lex.cpp
	$(COMPILE) -o $@ $^

obj/bool.spec.o: spec/spec.cpp spec/spec.h spec/vm/bool.spec.cpp
	$(COMPILE_OBJ) -o $@ spec/vm/bool.spec.cpp

obj/number.spec.o: spec/spec.cpp spec/spec.h spec/vm/number.spec.cpp
	$(COMPILE_OBJ) -o $@ spec/vm/number.spec.cpp

obj/load.spec.o: spec/spec.cpp spec/spec.h spec/vm/load.spec.cpp
	$(COMPILE_OBJ) -o $@ spec/vm/load.spec.cpp

obj/call.spec.o: spec/spec.cpp spec/spec.h spec/vm/call.spec.cpp
	$(COMPILE_OBJ) -o $@ spec/vm/call.spec.cpp
