.PHONY: spec

all: build spec run

CPPFLAGS = -std=c++11 -Wno-deprecated-register
INCLUDES = -I. -Isrc -Itmp

PROGRAM = tmp/*.cpp src/interpreter/*.cpp src/ast.cpp

clean:
	rm -f tmp/*

build: clean parse lex compile

lex:
	flex -o tmp/lex.cpp --header-file=tmp/lex.h src/parser/flex.cpp

parse:
	bison -o tmp/parse.cpp --defines=tmp/parse.h src/parser/bison.cpp

compile:
	g++ -g $(INCLUDES) -o bin/fn $(CPPFLAGS) $(PROGRAM) src/main.cpp

run:
	./bin/fn

spec:
	g++ -o tmp/spec $(INCLUDES) $(CPPFLAGS) $(PROGRAM) spec/main.cpp && ./tmp/spec

watch:
	fswatch -or src spec | xargs -I{} make spec