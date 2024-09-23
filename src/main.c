#include <stdio.h>
#include <string.h>

static int repl() {
    // The line buffer
    char line[1024];

    // initEnv();

    printf("Fn repl: Ctrl+C to quit\n");
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        // interpret(line);
    }

    // freeEnv();
    return 0;
}

static void printHelp() {
    printf("Fn: A fun-ctional programming language\n");
    printf("fn help   Print this message\n");
    printf("fn repl   Enter Fn code on the command line\n");
}

int main(int argc, const char* argv[]) {
    if (argc == 1) {
        printHelp();
        return 0;
    }

    const char* command = argv[1];
    if (strcmp(command, "repl") == 0) {
        return repl();
    }
    if (strcmp(command, "scan") == 0) {
        return scanRepl();
    }

    printHelp();
    return 0;
}
