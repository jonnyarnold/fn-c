#include <stdio.h>

#include "vm.h"

int main(int argc, char **argv)
{
   Value word = VALUE_WRAP_WORD((Word)3);
   bool secondBit = VALUE_UNWRAP_BOOL(Word_bitAt(word, VALUE_WRAP_I64(1)));
   printf("Hello, %i!\n", secondBit);
   return 0;
}
