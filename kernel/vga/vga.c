#include "vga.h"
#include "../io/io.h"
#include <stdarg.h>
#include <stdint.h>

static char *vga_buffer = (char *)VGA_ADDRESS;
static uint32_t vga_cursor = 0;
static uint8_t vga_color = 0x0f; // default to white on black

static void __vga_update_cursor(void) {
    uint16_t pos = vga_cursor;
    outb(IO_VGA_INDEX_PORT, IO_VGA_DATA_POS_LOW_BITS);
    outb(IO_VGA_DATA_PORT, (uint8_t)(pos & 0xFF));
    outb(IO_VGA_INDEX_PORT, IO_VGA_DATA_POS_HIGH_BITS);
    outb(IO_VGA_DATA_PORT, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_enable_cursor(void) {
    outb(IO_VGA_INDEX_PORT, 0x0A);
    outb(IO_VGA_DATA_PORT,  0x00);
    outb(IO_VGA_INDEX_PORT, 0x0B);
    outb(IO_VGA_DATA_PORT,  0x0F);
}

void vga_clear() {
    for (uint32_t i = 0; i < 2000 * 2; i += 2) {
        vga_buffer[i]   = ' ';
        vga_buffer[i+1] = 0x00;
    }
    vga_cursor = 0;
    __vga_update_cursor();
}

void vga_cclear(uint8_t color) {
    for (uint32_t i = 0; i < 2000 * 2; i += 2) {
        vga_buffer[i] = ' ';
        vga_buffer[i+1] = (color << 4);
    }
    vga_cursor = 0;
    __vga_update_cursor();
}

static void __vga_new_line(void) {
    uint32_t padding = VGA_COLS - (vga_cursor % VGA_COLS);
    for (uint32_t i = 0; i < padding; ++i) {
        vga_putchar(' ');
    }
    __vga_update_cursor();
}

void vga_print(const char *str) {
    if (!str) {
        return;
    }
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
    vga_buffer[vga_cursor*2+1] = vga_color;
    ++vga_cursor;
    __vga_update_cursor();
}


void vga_putint(int num) {
    if (num >=10) vga_putchar('0' + num / 10);
    vga_putchar('0' + num % 10);
}

void vga_puthex(uint8_t num) {
    char hex[] = "0123456789ABCDEF";
    vga_putchar(hex[num>>4]);
    vga_putchar(hex[num & 0x0f]);
}

void vga_puthex32(uint32_t num) {
    char hex[] = "0123456789ABCDEF";
    vga_print("0x");
    for (int i = 7; i >= 0; --i) {
        vga_putchar(hex[(num >> (i*4)) & 0xF]);
    }
}


void vga_printf(const char *str, ...) {
    if (!str) {
        return;
    }

    va_list args;
    va_start(args, str);
    uint32_t i = 0;

    while(str[i]) {
        char c = str[i];
        if (c == '%') {
            if (str[i+1]) {
                switch(str[i+1]) {
                    case 's':
                        char *s = va_arg(args, char *);
                        vga_print(s);
                        i++;
                        break;
                    case 'd':
                        int n = va_arg(args, int);
                        vga_putint(n);
                        i++;
                        break;
                    case 'x':
                        uint32_t x = va_arg(args, uint32_t);
                        vga_puthex32(x);
                        i++;
                        break;
                    default: break;
                }
            } else {
                vga_putchar(c);
            }
        } else if (c == '\n') {
            __vga_new_line();
        } else {
            vga_putchar(c);
        }
        i++;
    }
    va_end(args);
}

void vga_backspace(void) {
    if (vga_cursor > 0) {
        --vga_cursor;
        vga_putchar(' ');
        --vga_cursor; //counter vga_putchar increment
    }
    __vga_update_cursor();
}

/* only 4 first bits will be used, */
void vga_setforeground_color(uint8_t fg) {
    vga_color = (vga_color & 0xf0) | (fg & 0x0f);
}
/* only 4 first bits will be used */
void vga_setbackground_color(uint8_t bg) {
    vga_color = (vga_color & 0x0f) | (bg << 4);
}
