ASM = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

CFLAGS = -ffreestanding -nostdlib -mgeneral-regs-only -I/usr/lib/gcc/i686-elf/15.2.0/include
LDFLAGS = -T kernel/kernel.ld --oformat binary

#for calculating automatically value for AL in bootloader/boot.asm for loading all sectors for the kernel
KERNEL_SECTORS=$(shell expr $$(wc -c < kernel/kernel.bin) / 512 + 2)

BUILDS=build

all: $(BUILDS)/disk.img

#bootloader targets
bootloader/bootloader.bin: bootloader/boot.asm kernel/kernel.bin
	$(ASM) -f bin -dKERNEL_SECTORS=$(KERNEL_SECTORS) $< -o $@

#kernel related targets
kernel/kernel_entry.o: kernel/kernel_entry.asm
	$(ASM) -f elf32 $< -o $@

kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/kernel.bin: kernel/kernel_entry.o kernel/kernel.o kernel/idt/idt.o kernel/vga/vga.o
	$(LD) $(LDFLAGS) $^ -o $@

#kernel internal libs & utils targets
kernel/idt/idt.o: kernel/idt/idt.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/vga/vga.o: kernel/vga/vga.c
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILDS)/disk.img: kernel/kernel.bin bootloader/bootloader.bin
	dd if=/dev/zero of=$@ bs=512 count=20
	dd if=bootloader/bootloader.bin of=$@ conv=notrunc
	dd if=kernel/kernel.bin of=$@ bs=512 seek=1 conv=notrunc

clean:
	rm -f bootloader/bootloader.bin kernel/kernel_entry.o kernel/kernel.o kernel/kernel.bin kernel/idt/idt.o kernel/vga/vga.o $(BUILDS)/disk.img

run: $(BUILDS)/disk.img
	qemu-system-i386 -drive format=raw,file=$< -display sdl

run-debug-int: $(BUILDS)/disk.img
	qemu-system-i386 -drive format=raw,file=$< -display sdl -d int
