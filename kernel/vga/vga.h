#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_COLS    80
#define VGA_ROWS    25

void vga_clear(void);
void vga_print(const char *str);
void vga_println(const char *str);
void vga_putchar(char c);
void vga_putint(int num);
void vga_puthex(uint8_t num);


static void __vga_new_line(void);

#endif
