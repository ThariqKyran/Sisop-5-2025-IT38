#ifndef __SHELL_H__
#define __SHELL_H__

#include "std_type.h"

void shell();
void parseCommand(char *buf, char *cmd, char arg[2][64]);

// Tambahkan fungsi bila perlu

#endif // __SHELL_H__
