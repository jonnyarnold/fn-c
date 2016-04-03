all: build run

clean:
	rm -f tmp/*

build: clean parse lex compile

lex:
	flex -o tmp/lex.cpp --header-file=tmp/lex.h src/fn.flex.cpp

parse:
	bison -o tmp/parse.cpp --defines=tmp/parse.h src/fn.bison.cpp

compile:
	g++ tmp/parse.cpp tmp/lex.cpp -ll -o bin/fn

run:
	./bin/fn