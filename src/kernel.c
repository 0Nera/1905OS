/*
	'kernel.c' содержит наш основной код ядра. В частности, он содержит код для отображения текста на экране с использованием буфера текстового режима VGA.
*/

// GCC предоставляет эти библиотеки автоматически 
// Они дают нам доступ к таким полезным вещам, как типы фиксированного размера.
#include <stddef.h>
#include <stdint.h>
 
// Во-первых, давайте сделаем несколько базовых проверок, чтобы убедиться, что мы правильно используем кросс-компилятор x86-elf.
#if defined(__linux__)
	#error "This code must be compiled with a cross-compiler"
#elif !defined(__i386__)
	#error "This code must be compiled with an x86-elf compiler"
#endif
 
// Это буфер текстового режима VGA x86. Для отображения текста мы записываем данные в эту ячейку памяти.
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;

// По умолчанию буфер текстового режима VGA имеет размер 80x25 символов.
const int VGA_COLS = 80;
const int VGA_ROWS = 25;
 
// Мы начинаем отображать текст в верхнем левом углу экрана (столбец = 0, строка = 0)
int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F; // Черный фон, белый текст
 
// Эта функция инициализирует терминал очищая его
void term_init()
{
	// Очищаем текстовый буффер
	for (int col = 0; col < VGA_COLS; col ++)
	{
		for (int row = 0; row < VGA_ROWS; row ++)
		{
			// Буфер текстового режима VGA имеет размер (VGA_COLS * VGA_ROWS). Это 2000 символов.
			// Учитывая это, мы находим индекс в буфере для нашего персонажа.
			const size_t index = (VGA_COLS * row) + col;
			// Записи в буфере VGA принимают двоичную форму BBBBFFFFCCCCCCCC, где:
			// - B цвет фона
			// - F цвет переднего плана
			// - C симврл ASCII
			vga_buffer[index] = ((uint16_t)term_color << 8) | ' '; // Устанавливает пустой символ (пробел)
		}
	}
}
 
// Эта функция выводит один символ на экран
void term_putc(char c)
{
	// Запомните: мы не должны выводить все символы на экран
	switch (c){
	case '\n': { // Символ новой строки
			term_col = 0;
			term_row ++;
			break;
		}
 
	default: { // Обычные символы выводятся в новой ячейке
			const size_t index = (VGA_COLS * term_row) + term_col; // Считает индекс буффера
			vga_buffer[index] = ((uint16_t)term_color << 8) | c;
			term_col ++;
			break;
		}
	}
 
	// Что произойдет, если мы пройдем последний столбец? Нам нужно сбросить столбец до 0 и увеличить строку, чтобы перейти на новую строку.
	if (term_col >= VGA_COLS){
		term_col = 0;
		term_row ++;
	}
 
	// Что произойдет, если мы пройдем последний ряд? Нам нужно сбросить столбец и строку до 0, чтобы вернуться к верхней части экрана.
	if (term_row >= VGA_ROWS){
		term_col = 0;
		term_row = 0;
	}
}
 
// Эта функция выводит текст на экран
void term_print(const char* str){
	for (size_t i = 0; str[i] != '\0'; i ++){// Вывод текста до символа '\0' обозначающего конец строки
		term_putc(str[i]);
	} 
}
 
 
 
// Это главная функция ядра, она выполняется самой первой
void kernel_main(){
 
	// Инициализация терминала
	term_init();
 
	// Вывод сообщений на экран
	term_print("Hello, World!\n");
	term_print("Welcome to the kernel.\n");
}