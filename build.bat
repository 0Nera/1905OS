::Сценарий сборки операционной системы

::Отключение вывода ненужных данных
@echo off

::Создание папки для выходных фалов
mkdir bin


::Сборка ОС
i686-elf-gcc -O2 -std=gnu99 -Wall -Wextra -ffreestanding -g -c src/86/tty.c -o bin/tty.o
i686-elf-gcc -O2 -std=gnu99 -Wall -Wextra -ffreestanding -g -c src/86/ports.c -o bin/ports.o
i686-elf-gcc -O2 -std=gnu99 -Wall -Wextra -ffreestanding -g -c src/drivers/CMOS.c -o bin/CMOS.o

i686-elf-gcc -O2 -std=gnu99 -Wall -Wextra -ffreestanding -g -c src/start.s -o bin/start.o
i686-elf-gcc -O2 -std=gnu99 -Wall -Wextra -ffreestanding -g -c src/kernel.c -o bin/kernel.o
i686-elf-gcc -ffreestanding -nostdlib -g -T src/linker.ld bin/start.o bin/tty.o bin/ports.o bin/CMOS.o bin/kernel.o  -o bin/mykernel.elf -lgcc


::Остановка выполнения сценария и
::ожидание ввода ENTER
pause