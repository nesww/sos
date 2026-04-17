#ifndef KCORE_STRING
#define KCORE_STRING
#include <stdint.h>

static inline uint32_t kstr_len (const char *str) {
    uint32_t len = 0;
    while (str[len] != '\0') len++;
    return len;
}
#endif // KCORE_STRING
