/* Функции для работы с портами */
#ifndef PORTS_H
#define PORTS_H

#include <stdint.h> //Для использования специальных типов

#define PORT_COM1 0x3f8

uint8_t inb(uint16_t port); //Читает байт с порта
void outb(uint16_t port, uint8_t  val); //Для написания числа в порт
void outw(uint16_t port, uint16_t val); //Для записи символа в порт

#endif