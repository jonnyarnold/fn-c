#include "test.h"
#include "vm.h"

int test_Value_size()
{
    ASSERT("sizeof(Value) == 16", sizeof(Value) == 16);
    ASSERT("sizeof(ValueData) == 8", sizeof(ValueData) == 8);
    return 0;
}

int test_Word_bitAt()
{
    // test_extracts_bit
    Value word = VALUE_WRAP_WORD((Word)2);

    bool bit0 = VALUE_UNWRAP_BOOL(Word_bitAt(word, VALUE_WRAP_I64(0)));
    ASSERT("Word_bitAt(2, 0) == 0", bit0 == 0);

    bool bit1 = VALUE_UNWRAP_BOOL(Word_bitAt(word, VALUE_WRAP_I64(1)));
    ASSERT("Word_bitAt(2, 1) == 1", bit1 == 1);

    return 0;
}

// FIXME: Break up this file
int main(int argc, char **argv)
{
    return (test_Value_size() || test_Word_bitAt());
}
