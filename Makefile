# Adapted from https://dev.to/erhant/a-makefile-template-for-small-projects-1akc

# Compilation commands
COMPILE=gcc -std=c11 -Isrc -c -Wall -Wextra
LINK=gcc -Isrc -Lobj -Wall -Wextra

MAIN_SRCS=$(shell find src -type f -name *.c -not -name *test.c)
MAIN_OBJS=$(patsubst src/%,obj/%,$(MAIN_SRCS:.c=.o))

TEST_SRCS=$(shell find src -type f -name *.c -not -name main.c)
TEST_OBJS=$(patsubst src/%,obj/%,$(TEST_SRCS:.c=.o))

ALL_SRCS=$(shell find src -type f -name *.c)
ALL_OBJS=$(patsubst src/%,obj/%,$(ALL_SRCS:.c=.o))

.PHONY: run
run: bin/fn
	./bin/fn

bin/fn: $(MAIN_OBJS)
	$(LINK) -o $@ $^

.PHONY: test
test: bin/test-fn
	time ./bin/test-fn

bin/test-fn: $(TEST_OBJS)
	$(LINK) -o $@ $^

$(ALL_OBJS): obj/%.o : src/%.c
	$(COMPILE) -o $@ $^

.PHONY: clean
clean:
	rm -rf bin/* obj/*
