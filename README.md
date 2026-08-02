<div align="center">

# rtjn-kernel

### [CODEBERG](https://codeberg.org/retrojan/rtjn-kernel/)

**Simple x86 kernel**

<br>

<img width="732" height="410" alt="image" src="https://github.com/user-attachments/assets/baec0dcd-4025-47fc-90ef-82e0cbe74ac9" />

<br><br>

**BIOS/Legacy boot only (Multiboot)**  
For UEFI systems: enable CSM

</div>


## PACKAGES

U need:
- nasm
- gcc
- ld
- grub
- xorriso
- qemu

Arch Linux
```bash
sudo pacman -S nasm gcc grub xorriso qemu
```

## BUILD

Clone repository
```bash
git clone https://github.com/retrojan/rtjn-kernel.git && cd rtjn-kernel
```
Build
```bash
make
```
Run qemu
```bash
make run
```

