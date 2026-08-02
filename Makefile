ISO_NAME = retrojan.iso

all:
	nasm -f elf32 boot.asm -o ks.o
	gcc -m32 -c kernel.c -o kernel.o
	ld -melf_i386 -T linker.ld -o kernel.bin ks.o kernel.o
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	echo 'set timeout=3' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Retrojan" {' >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '    boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_NAME) iso/

clean:
	rm -f *.o *.bin
	rm -rf iso
	rm -f $(ISO_NAME)

run:
	qemu-system-i386 -cdrom $(ISO_NAME)
