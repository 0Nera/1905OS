/*
	'kernel.c' содержит наш основной код ядра. В частности, он содержит код для отображения текста на экране с использованием буфера текстового режима VGA.
*/

// GCC предоставляет эти библиотеки автоматически 
// Они дают нам доступ к таким полезным вещам, как типы фиксированного размера.
#include <stddef.h>
#include <stdint.h>
#include "include/tty.h"
#include "include/CMOS.h"
 
// Во-первых, давайте сделаем несколько базовых проверок, чтобы убедиться, что мы правильно используем кросс-компилятор x86-elf.
#if defined(__linux__)
	#error "This code must be compiled with a cross-compiler"
#elif !defined(__i386__)
	#error "This code must be compiled with an x86-elf compiler"
#endif
 

 
// Это главная функция ядра, она выполняется самой первой
void kernel_main(){
 
	// Инициализация терминала
	term_init();
 
	// Вывод сообщений на экран
	term_print("Hello, World!\n");
	term_print("Welcome to the kernel.\n");
	getnowtime();
}