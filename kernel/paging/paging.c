#include "paging.h"
#include "../frame/frame.h"
#include "../mem/mem.h"
#include <stdint.h>

static page_directory *kernel_pd;

page_directory *paging_init(void) {
    kernel_pd = (page_directory*)fa_alloc();
    for (uint32_t i = 0; i < 1024; ++i) {
        kernel_pd->entries[i] = 0;
    }

    for (uint32_t i = 0; i < (uint32_t)mem_heap_get_start() + mem_heap_get_size(); i+=4096) {
        uint32_t pd_index = i >> 22; /*keeping only the 10 last bits (high) */
        if (kernel_pd->entries[pd_index] == 0) {
            kernel_pd->entries[pd_index] = (uint32_t)fa_alloc() | PAGING_PD_ENTRY_FLAGS_KERNEL_ONLY;
        }
        uint32_t pt_index = (i >> 12) & 0x3FF; /*get the 10 bits after the 10 higher bits, removing the last 12 bits*/
        page_table *pt = (page_table *)(kernel_pd->entries[pd_index] & 0xFFFFF000);
        pt->entries[pt_index] = i | PAGING_PT_ENTRY_FLAGS_KERNEL_ONLY;
    }

    PAGING_LOAD_CR3(kernel_pd);
    PAGING_ENABLE();
}
