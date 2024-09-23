FLAGS := -std=c11 -Wall -Wextra

# Files.
HEADERS := $(wildcard src/*.h)
SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix build/, $(notdir $(SOURCES:.c=.o)))

# Compile object files
build/%.o: src/%.c $(HEADERS)
	@ printf "%s\n" "$(CC) -c $(FLAGS) -o $@ $<"
	@ mkdir -p build
	@ $(CC) -c $(FLAGS) -o $@ $<

# Link executable
build/fn: $(OBJECTS)
	@ printf "%s\n" "$(CC) -c $(FLAGS) -o $@ $<"
	@ mkdir -p build
	@ $(CC) $(FLAGS) $^ -o $@

all: build

.PHONY: build
build: build/fn

.PHONY: run
run: build/fn
	./build/fn

.PHONY: clean
clean: 
	rm -rf build

.PHONY: rebuild
rebuild: clean build
