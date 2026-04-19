#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>

void fa_init(void);

void *fa_alloc(void);
void fa_free(uint32_t addr);

#endif //FRAME_H
