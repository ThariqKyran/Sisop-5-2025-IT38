#include "std_lib.h"

/*
 * div(int a, int b)
 *   Menghitung pembagian a / b (integer division),
 *   mendukung angka negatif.
 *   Jika b == 0, hasilnya 0.
 */
int div(int a, int b)
{
    int sign;
    int dividend;
    int divisor;
    int result;

    sign = 1;
    dividend = a;
    divisor = b;
    result = 0;

    if (b == 0) {
        /* Pembagian oleh nol tidak terdefinisi; kembalikan 0 */
        return 0;
    }

    /* Tentukan tanda hasil */
    if (dividend < 0) {
        sign = -sign;
        dividend = -dividend;
    }
    if (divisor < 0) {
        sign = -sign;
        divisor = -divisor;
    }

    /* Lakukan pembagian dengan pengurangan berulang */
    while (dividend >= divisor) {
        dividend = dividend - divisor;
        result = result + 1;
    }

    return sign * result;
}

/*
 * mod(int a, int b)
 *   Menghitung sisa a % b, mendukung angka negatif.
 *   a % b = a - (a/b)*b. Jika b == 0, kembalikan 0.
 */
int mod(int a, int b)
{
    int quotient;

    if (b == 0) {
        return 0;
    }

    quotient = div(a, b);
    return a - (quotient * b);
}

/*
 * strcmp(char *str1, char *str2)
 *   Mengembalikan true jika str1 sama persis dengan str2, false kalau beda.
 */
bool strcmp(char *str1, char *str2)
{
    unsigned int i;

    i = 0;
    while (str1[i] != '\0' || str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return false;
        }
        i = i + 1;
    }
    return true;
}

/*
 * strcpy(char *dst, char *src)
 *   Menyalin isi src (termasuk '\0') ke dst.
 *   Asumsikan dst sudah dialokasikan cukup besar.
 */
void strcpy(char *dst, char *src)
{
    unsigned int i;

    i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i = i + 1;
    }
    dst[i] = '\0';
}

/*
 * clear(byte *buf, unsigned int size)
 *   Mengisi buf[0..size-1] dengan 0x00.
 */
void clear(byte *buf, unsigned int size)
{
    unsigned int i;

    i = 0;
    while (i < size) {
        buf[i] = 0x00;
        i = i + 1;
    }
}

/*
 * atoi(char *str, int *num)
 *   Mengonversi string desimal (bisa diawali '-' atau '+') menjadi integer.
 *   Contoh: "123" -> 123, "-42" -> -42. Hentikan konversi saat bertemu karakter non-digit.
 */
void atoi(char *str, int *num)
{
    unsigned int i;
    int sign;
    int result;

    i = 0;
    sign = 1;
    result = 0;

    /* Periksa tanda '+' atau '-' */
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    } else if (str[0] == '+') {
        i = 1;
    }

    /* Proses tiap digit */
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            result = (result * 10) + (str[i] - '0');
        } else {
            /* Jika bertemu karakter bukan digit, hentikan */
            break;
        }
        i = i + 1;
    }

    *num = sign * result;
}

/*
 * itoa(int num, char *str)
 *   Mengonversi integer (bisa negatif) menjadi string null-terminated.
 *   Contoh: 456 -> "456", -78 -> "-78", 0 -> "0".
 *   Pastikan str cukup besar (maks ~12 karakter untuk int 32-bit).
 */
void itoa(int num, char *str)
{
    unsigned int i;
    int n;
    int isNegative;
    char buf[12];
    unsigned int j;
    int digit;

    /* Deklarasi variabel di atas agar BCC tidak error */

    i = 0;
    n = num;
    isNegative = 0;
    j = 0;

    /* Kasus special: num == 0 */
    if (n == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    /* Periksa tanda negatif */
    if (n < 0) {
        isNegative = 1;
        n = -n;
    }

    /* Konversi setiap digit ke char, simpan terbalik di buf[] */
    while (n > 0) {
        digit = mod(n, 10);
        buf[j] = (char)('0' + digit);
        n = div(n, 10);
        j = j + 1;
    }

    /* Tambahkan tanda minus bila perlu */
    if (isNegative != 0) {
        str[i] = '-';
        i = i + 1;
    }

    /* Balikkan urutan digit dari buf[] ke str[] */
    while (j > 0) {
        j = j - 1;
        str[i] = buf[j];
        i = i + 1;
    }

    str[i] = '\0';
}
