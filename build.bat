::Сценарий сборки операционной системы

::Отключение вывода ненужных данных
@echo off

::Создание папки для выходных фалов
mkdir bin


::Сборка ОС
i686-elf-gcc -O2 -std=gnu99 -Wall -Wextra -ffreestanding -g -c src/start.s -o bin/start.o
i686-elf-gcc -O2 -std=gnu99 -Wall -Wextra -ffreestanding -g -c src/kernel.c -o bin/kernel.o
i686-elf-gcc -ffreestanding -nostdlib -g -T src/linker.ld bin/start.o bin/kernel.o -o bin/mykernel.elf -lgcc


::Остановка выполнения сценария и
::ожидание ввода ENTER
pause