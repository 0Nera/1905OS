#ifndef TTY_H
#define TTY_H

#include <stddef.h>
#include <stdint.h>

void term_init();
void term_putc(char c);
void term_print(const char* str);
void term_putint(int i);

#endif