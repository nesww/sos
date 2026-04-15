void clear_screen(void) {
    char *video = (char *)0xB8000;
    for (int i = 0; i < 2000 * 2; i += 2) {
        video[i]     = ' ';
        video[i + 1] = 0x07;
    }
}

void print(const char *str) {
    char *video = (char *)0xB8000;
    int i = 0;
    while (str[i] != '\0') {
        video[i * 2]     = str[i];
        video[i * 2 + 1] = 0x04;
        i++;
    }
}

void kernel_main(void) {
    clear_screen();
    print("hello");
    while(1);
}
