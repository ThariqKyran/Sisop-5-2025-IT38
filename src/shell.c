#include "shell.h"
#include "kernel.h"
#include "std_lib.h"

/* 
 * Variabel global untuk menyimpan username dan suffix Grand Company
 */
char username[64]     = "user";
char promptSuffix[16] = "";

/*
 * Array global untuk jawaban acak pada perintah "yogurt"
 */
char *responses[3] = { 
    "yo", 
    "ts unami gng </3", 
    "sygau" 
};

/* 
 * Fungsi bantu: mencetak prompt, misalnya:
 *   user> 
 *   atau 
 *   Tia@Storm> 
 */
void printPrompt() {
    printString(username);
    printString(promptSuffix);
    printString("> ");
}

/*
 * Fungsi bantu: parsing input "buf" menjadi:
 *   cmd     : kata pertama (sebelum spasi)
 *   arg[0]  : kata kedua (jika ada)
 *   arg[1]  : kata ketiga (jika ada)
 * Sisanya diabaikan (maksimalkan dua argumen).
 */
void parseCommand(char *buf, char *cmd, char arg[2][64]) {
    unsigned int i, j, k;

    // Inisialisasi string cmd dan arg
    cmd[0]    = '\0';
    arg[0][0] = '\0';
    arg[1][0] = '\0';

    // 1) Lewati spasi awal
    i = 0;
    while (buf[i] == ' ') {
        i++;
    }

    // 2) Baca cmd hingga spasi atau '\0'
    j = 0;
    while (buf[i] != '\0' && buf[i] != ' ') {
        cmd[j++] = buf[i++];
    }
    cmd[j] = '\0';

    // Jika sudah akhir string, selesai
    if (buf[i] == '\0') {
        return;
    }

    // 3) Lewati spasi sebelum arg[0]
    while (buf[i] == ' ') {
        i++;
    }

    // 4) Baca arg[0]
    j = 0;
    while (buf[i] != '\0' && buf[i] != ' ' && j < 63) {
        arg[0][j++] = buf[i++];
    }
    arg[0][j] = '\0';

    // Jika akhir, selesai
    if (buf[i] == '\0') {
        return;
    }

    // 5) Lewati spasi sebelum arg[1]
    while (buf[i] == ' ') {
        i++;
    }

    // 6) Baca arg[1]
    k = 0;
    while (buf[i] != '\0' && buf[i] != ' ' && k < 63) {
        arg[1][k++] = buf[i++];
    }
    arg[1][k] = '\0';

    // Sisa setelah arg[1] diabaikan
}

/*
 * Fungsi utama shell: loop membaca input dan mengeksekusi perintah.
 */
void shell() {
    char buf[128];
    char cmd[64];
    char arg[2][64];
    char tempStr[64];
    int num1, num2, result;
    unsigned int tick, idx;

    printString("Welcome to EorzeOS!\n");

    while (1) {
        // Cetak prompt (misalnya "user> " atau "Tia@Storm> ")
        printPrompt();

        // Baca input user (hingga Enter), otomatis echo selama ketik
        readString(buf);

        // Parsing input ke cmd, arg[0], arg[1]
        parseCommand(buf, cmd, arg);

        // --- 1) The Echo: jika cmd bukan valid, akan dijalankan di cabang terakhir ---

        // --- 2) gurt: yo ---
        if (strcmp(cmd, "yo")) {
            printString("gurt\n");
        }
        else if (strcmp(cmd, "gurt")) {
            printString("yo\n");
        }
        // --- 3) user <username> ---
        else if (strcmp(cmd, "user")) {
            if (arg[0][0] != '\0') {
                // Ubah username ke arg[0]
                strcpy(username, arg[0]);
                printString("Username changed to ");
                printString(username);
                printString("\n");
            } else {
                // Tanpa argumen: kembalikan ke "user"
                strcpy(username, "user");
                printString("Username changed to user\n");
            }
        }
        // --- 4) grandcompany <nama> ---
        else if (strcmp(cmd, "grandcompany")) {
            if (strcmp(arg[0], "maelstrom")) {
                clearScreen();
                strcpy(promptSuffix, "@Storm");
                // (Opsional: jika ingin warna merah, perlu menulis ke video memory via putInMemory)
            }
            else if (strcmp(arg[0], "twinadder")) {
                clearScreen();
                strcpy(promptSuffix, "@Serpent");
                // (Opsional: warna kuning)
            }
            else if (strcmp(arg[0], "immortalflames")) {
                clearScreen();
                strcpy(promptSuffix, "@Flame");
                // (Opsional: warna biru)
            }
            else {
                printString("Error: invalid grandcompany\n");
            }
        }
        // Jika command 'clear' → reset Grand Company
        else if (strcmp(cmd, "clear")) {
            clearScreen();
            promptSuffix[0] = '\0';
            // (Opsional: reset warna ke default)
        }
        // --- 5) Kalkulator: add, sub, mul, div ---
        else if (strcmp(cmd, "add")) {
            if (arg[0][0] != '\0' && arg[1][0] != '\0') {
                atoi(arg[0], &num1);
                atoi(arg[1], &num2);
                result = num1 + num2;
                itoa(result, tempStr);
                printString(tempStr);
                printString("\n");
            } else {
                printString("Usage: add <x> <y>\n");
            }
        }
        else if (strcmp(cmd, "sub")) {
            if (arg[0][0] != '\0' && arg[1][0] != '\0') {
                atoi(arg[0], &num1);
                atoi(arg[1], &num2);
                result = num1 - num2;
                itoa(result, tempStr);
                printString(tempStr);
                printString("\n");
            } else {
                printString("Usage: sub <x> <y>\n");
            }
        }
        else if (strcmp(cmd, "mul")) {
            if (arg[0][0] != '\0' && arg[1][0] != '\0') {
                atoi(arg[0], &num1);
                atoi(arg[1], &num2);
                result = num1 * num2;
                itoa(result, tempStr);
                printString(tempStr);
                printString("\n");
            } else {
                printString("Usage: mul <x> <y>\n");
            }
        }
        else if (strcmp(cmd, "div")) {
            if (arg[0][0] != '\0' && arg[1][0] != '\0') {
                atoi(arg[0], &num1);
                atoi(arg[1], &num2);
                if (num2 == 0) {
                    printString("Error: divide by zero\n");
                } else {
                    result = num1 / num2;  // Pembagian integer
                    itoa(result, tempStr);
                    printString(tempStr);
                    printString("\n");
                }
            } else {
                printString("Usage: div <x> <y>\n");
            }
        }
        // --- 6) yogurt (pilih random dari responses[]) ---
        else if (strcmp(cmd, "yogurt")) {
            tick = getBiosTick();
            idx  = mod(tick, 3);
            printString(responses[idx]);
            printString("\n");
        }
        // --- default: The Echo (ulang input) ---
        else {
            printString(buf);
            printString("\n");
        }
    }
}
