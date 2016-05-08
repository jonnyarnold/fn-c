all: build run

LLVMCONFIG = /usr/local/opt/llvm/bin/llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11 -Wno-deprecated-register
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic
LIBS = `$(LLVMCONFIG) --libs` 

clean:
	rm -f tmp/*

build: clean parse lex compile

lex:
	flex -o tmp/lex.cpp --header-file=tmp/lex.h src/fn.flex.cpp

parse:
	bison -o tmp/parse.cpp --defines=tmp/parse.h src/fn.bison.cpp

compile:
	g++ -g -o bin/fn $(CPPFLAGS) $(LIBS) $(LDFLAGS) tmp/*.cpp src/fn.ast.cpp src/fn.runtime.cpp src/main.cpp

run:
	./bin/fn
