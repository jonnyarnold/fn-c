// Adapted from minunit:
// https://jera.com/techinfo/jtns/jtn002

#include <stdio.h>
#include <assert.h>

// TODO: Replace with <assert.h>?
#define ASSERT(message, test)         \
    if (!(test))                      \
    {                                 \
        printf("❌ %s\n", (message)); \
        return 1;                     \
    }                                 \
    else                              \
    {                                 \
        printf("✅ %s\n", (message)); \
    }
