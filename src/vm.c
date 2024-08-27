#include "vm.h"

Value Word_bitAt(Value word, Value index)
{
    Word unpackedWord = VALUE_UNWRAP_WORD(word);
    I64 unpackedIndex = VALUE_UNWRAP_I64(index);
    return VALUE_WRAP_BOOL((bool)((unpackedWord >> unpackedIndex) & 1));
}
