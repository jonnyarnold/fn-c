.PHONY: spec

all: build spec run

CPPFLAGS = -std=c++11 -Wno-deprecated-register

clean:
	rm -f tmp/*

build: clean parse lex compile

lex:
	flex -o tmp/lex.cpp --header-file=tmp/lex.h src/parser/flex.cpp

parse:
	bison -o tmp/parse.cpp --defines=tmp/parse.h src/parser/bison.cpp

compile:
	g++ -g -Isrc -Itmp -o bin/fn $(CPPFLAGS) tmp/*.cpp src/interpreter/*.cpp src/*.cpp

run:
	./bin/fn < test.fn

spec:
	g++ -o tmp/spec spec/main.cpp && ./tmp/spec
