.PHONY: spec

all: build spec run

CPPFLAGS = -std=c++11 -Wno-deprecated-register

clean:
	rm -f tmp/*

build: clean parse lex compile

lex:
	flex -o tmp/lex.cpp --header-file=tmp/lex.h src/fn.flex.cpp

parse:
	bison -o tmp/parse.cpp --defines=tmp/parse.h src/fn.bison.cpp

compile:
	g++ -g -o bin/fn $(CPPFLAGS) tmp/*.cpp src/fn.ast.cpp src/fn.runtime.cpp src/main.cpp

run:
	./bin/fn < test.fn

spec:
	g++ -o tmp/spec spec/main.cpp && ./tmp/spec
