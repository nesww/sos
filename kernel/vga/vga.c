#include "vga.h"

static char *vga_buffer = (char *)VGA_ADDRESS;
static uint32_t vga_cursor = 0;

void vga_clear() {
    for (uint32_t i = 0; i < 2000 * 2; i += 2) {
        vga_buffer[i]   = ' ';
        vga_buffer[i+1] = 0x07;
    }
    vga_cursor = 0;
}

static void __vga_new_line(void) {
    uint32_t padding = VGA_COLS - (vga_cursor % VGA_COLS);
    for (uint32_t i = 0; i < padding; ++i) {
        vga_putchar(' ');
    }
}

void vga_print(const char *str) {
    uint32_t i = 0;
    while(str[i] != '\0') {
        vga_putchar(str[i]);
        i++;
    }
}

void vga_println(const char *str) {
    vga_print(str);
    __vga_new_line();
}

void vga_putchar(char c) {
    vga_buffer[vga_cursor*2] = c;
    vga_buffer[vga_cursor*2+1] = 0x04;
    ++vga_cursor;
}

void vga_putint(int num) {
    if (num >=10) vga_putchar('0' + num / 10);
    vga_putchar('0' + num % 10);
}

void vga_puthex(uint8_t num) {
    char hex[] = "0123456789ABCDEFGH";
    vga_putchar(hex[num>>4]);
    vga_putchar(hex[num & 0x0f]);
}
