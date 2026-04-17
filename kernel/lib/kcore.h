#ifndef KCORE_H
#define KCORE_H

#define TRUE 1
#define FALSE

#define KMIN(a,b) ((a) < (b) ? (a) : (b))
#define KMAX(a,b) ((a) > (b) ? (a) : (b))

#define KB(x) ((x) * 1024)
#define MB(x) ((x) * 1024 * 1024)

#define TODO(str)                           \
    do {                                    \
        if (str)                            \
            vga_printf("TODO: " #str "\n"); \
    } while(0)                              \

#endif //K_CORE_H
