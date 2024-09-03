#include <stdio.h>

#include "builtins.h"

int main(int argc, char **argv)
{
   File_write(STDOUT, VALUE_WRAP_CHARS("Hello, world!"));
   return 0;
}
