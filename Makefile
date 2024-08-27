# Adapted from https://dev.to/erhant/a-makefile-template-for-small-projects-1akc

# Compilation commands
COMPILE=gcc -std=c11 -Isrc -c -Wall -Wextra
LINK=gcc -Isrc -Lobj -Wall -Wextra

SRC_FILES=$(shell find src -type f -name *.c)
OBJ_FILES=$(patsubst src/%,obj/%,$(SRC_FILES:.c=.o))

.PHONY: run
run: bin/fn
	./bin/fn

bin/fn: $(OBJ_FILES)
	$(LINK) -o $@ $^

$(OBJ_FILES): obj/%.o : src/%.c
	$(COMPILE) -o $@ $^

.PHONY: clean
clean:
	rm -rf bin/* obj/*
