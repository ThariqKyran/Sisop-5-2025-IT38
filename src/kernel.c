#include "shell.h"
#include "kernel.h"

/*
 * printString()
 *   Mencetak sebuah null‐terminated string ke layar menggunakan BIOS teletype (INT 10h, AH=0x0E).
 */
void printString(char *str) {
    unsigned int i = 0;
    while (str[i] != '\0') {
        int ax = (0x0E << 8) | str[i];  // AH = 0x0E, AL = karakter
        interrupt(0x10, ax, 0, 0, 0);
        i++;
    }
}

/*
 * readString()
 *   Membaca input user karakter per karakter menggunakan BIOS keyboard interrupt (INT 16h, AH=0x00),
 *   menyimpan di buffer buf, sampai Enter (0x0D). Menangani backspace (0x08) dengan menghapus satu karakter.
 *   Setelah Enter, mencetak CR-LF agar cursor berpindah ke baris baru, lalu men‐terminate buf dengan '\0'.
 */
void readString(char *buf) {
    unsigned int idx = 0;
    char c;
    while (1) {
        int ax = interrupt(0x16, 0, 0, 0, 0);     // INT 16h, AH=0x00: wait input keyboard
        c = (char)(ax & 0xFF);                   // ambil AL

        if (c == 0x0D) {  // Enter (CR)
            break;
        } else if (c == 0x08) {  // Backspace
            if (idx > 0) {
                idx--;
                // 1) Keluarkan backspace (ASCII 0x08), 2) spasi, 3) backspace lagi
                interrupt(0x10, (0x0E << 8) | 0x08, 0, 0, 0);
                interrupt(0x10, (0x0E << 8) | ' ',  0, 0, 0);
                interrupt(0x10, (0x0E << 8) | 0x08, 0, 0, 0);
            }
        } else {
            // Simpan karakter dan echo ke layar
            buf[idx++] = c;
            interrupt(0x10, (0x0E << 8) | c, 0, 0, 0);
        }
    }

    buf[idx] = '\0';
    // Setelah Enter, kirim CR-LF agar pindah baris
    interrupt(0x10, (0x0E << 8) | '\r', 0, 0, 0);
    interrupt(0x10, (0x0E << 8) | '\n', 0, 0, 0);
}

/*
 * clearScreen()
 *   Mengosongkan seluruh layar text mode (80×25) dengan cara menggulirkan layar ke atas menggunakan
 *   BIOS INT 10h, AH=0x06, kemudian mengatur kursor ke (0,0).
 */
void clearScreen() {
    // AX = 0x0600 (AH=06h, AL=0 = scroll entire window)
    // BX = 0x0700 (BH=07h = atribut putih, BL diabaikan)
    // CX = 0x0000 (CH=0, CL=0 = baris atas = 0, kolom kiri = 0)
    // DX = 0x184F (DH=24, DL=79 = baris bawah = 24, kolom kanan = 79)
    int ax = (0x06 << 8) | 0;
    int bx = (0x07 << 8) | 0;
    int cx = 0x0000;
    int dx = (24 << 8)    | 79;
    interrupt(0x10, ax, bx, cx, dx);

    // Set kursor ke (0,0): AX = 0x0200 (AH=02h, AL=0 = pindah kursor)
    // DX = 0x0000 (DH=0 baris, DL=0 kolom)
    ax = (0x02 << 8) | 0;
    dx = 0x0000;
    interrupt(0x10, ax, 0, 0, dx);
}

/*
 * main()
 *   Panggil clearScreen() sekali, lalu masuk ke shell().
 */
int main() {
    clearScreen();
    shell();
    return 0;
}
