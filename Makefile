OBJECTS = loader.o kmain.o io.o timer.o serial.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c -g
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -g -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf

	genisoimage -R \
				-b boot/grub/stage2_eltorito \
				-no-emul-boot \
				-boot-load-size 4 \
				-A os \
				-input-charset utf8 \
				-quiet \
				-boot-info-table \
				-o os.iso \
				iso

debug: os.iso
	qemu-system-i386 -s -S -boot d -cdrom os.iso -m 16

run: os.iso
	qemu-system-i386 -boot d -cdrom os.iso -m 16

serial: os.iso
	qemu-system-i386 -boot d -cdrom os.iso -m 16 -serial stdio

gdb: 
	gdb -ex "target remote localhost:1234" kernel.elf

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso