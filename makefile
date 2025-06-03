# Nama direktori keluaran dan image
BIN_DIR = bin
IMG     = $(BIN_DIR)/floppy.img

.PHONY: build prepare bootloader stdlib kernel shell link clean

# Target utama: jalankan semua langkah build
build: prepare bootloader stdlib kernel shell link
	@echo "Build selesai. Jalankan: bochs -f bochsrc.txt"

# 1) Buat folder bin/ dan floppy.img kosong (1.44 MB)
prepare:
	mkdir -p $(BIN_DIR)
	dd if=/dev/zero of=$(IMG) bs=512 count=2880

# 2) Compile bootloader.asm → bootloader.bin, lalu tulis ke sektor pertama image
bootloader: prepare
	nasm -f bin src/bootloader.asm -o $(BIN_DIR)/bootloader.bin
	dd if=$(BIN_DIR)/bootloader.bin of=$(IMG) bs=512 count=1 conv=notrunc

# 3) Compile pustaka std_lib.c → std_lib.o
stdlib:
	bcc -ansi -Iinclude -c src/std_lib.c -o $(BIN_DIR)/std_lib.o

# 4) Compile kernel.asm → kernel_asm.o, kemudian kernel.c → kernel.o
kernel: stdlib
	nasm -f as86 src/kernel.asm -o $(BIN_DIR)/kernel_asm.o
	bcc -ansi -Iinclude -c src/kernel.c    -o $(BIN_DIR)/kernel.o

# 5) Compile shell.c → shell.o
shell:
	bcc -ansi -Iinclude -c src/shell.c     -o $(BIN_DIR)/shell.o

# 6) Link semua object → kernel.bin, lalu tulis ke image mulai sektor 1
link: kernel shell
	ld86 -o $(BIN_DIR)/kernel.bin -d \
		$(BIN_DIR)/kernel.o \
		$(BIN_DIR)/kernel_asm.o \
		$(BIN_DIR)/std_lib.o \
		$(BIN_DIR)/shell.o
	dd if=$(BIN_DIR)/kernel.bin of=$(IMG) bs=512 seek=1 conv=notrunc

# 7) Bersihkan file hasil build
clean:
	rm -f $(BIN_DIR)/*.o
	rm -f $(BIN_DIR)/bootloader.bin
	rm -f $(BIN_DIR)/kernel.bin
	rm -f $(IMG)
