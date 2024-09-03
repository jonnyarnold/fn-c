#include "vm.h"

Value Word_bitAt(Value word, Value index)
{
    Word unpackedWord = VALUE_UNWRAP_WORD(word);
    I64 unpackedIndex = VALUE_UNWRAP_I64(index);
    return VALUE_WRAP_BOOL((bool)((unpackedWord >> unpackedIndex) & 1));
}

Value File_open(Value fileName, Value mode)
{
    char *unpackedFilename = VALUE_UNWRAP_CHARS(fileName);

    // FIXME: Get Flags
    Word unpackedMode = VALUE_UNWRAP_WORD(mode);
    char *modeChars = "r";

    FILE *file = fopen(unpackedFilename, modeChars);
    return VALUE_WRAP_FILE(file);
}

void File_write(Value file, Value data)
{
    FILE *unpackedFile = VALUE_UNWRAP_FILE(file);
    char *unpackedData = VALUE_UNWRAP_CHARS(data);

    fprintf(unpackedFile, unpackedData);
}
