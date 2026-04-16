#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_ADDRESS 0xB8000

void vga_clear(void);
void vga_print(const char *str);
void vga_putchar(char c);
void vga_putint(int num);

#endif
