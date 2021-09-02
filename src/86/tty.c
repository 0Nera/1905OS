#include "../include/tty.h"

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
 
void term_putuint(int i)
{
    unsigned int n, d = 1000000000;
    char str[255];
    unsigned int dec_index = 0;
    while( ( i/d == 0 ) && ( d >= 10 ) ) d /= 10;
    n = i;
    while(d >= 10)
    {
        str[dec_index++] = ((char)((int)'0' + n/d));
        n = n % d;
        d /= 10;
    }
    str[dec_index++] = ((char)((int)'0' + n));
    str[dec_index] = 0;
    term_print(str);
}

void term_putint(int i)
{
    if(i >= 0)
    {
        term_putuint(i);
    } else {
        term_putc('-');
        term_putuint(-i);
    }
}