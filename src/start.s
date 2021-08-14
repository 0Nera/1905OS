// Мы объявляем метку 'kernel_main' внешней по отношению к этому файлу.
// Это потому, что в файле 'kernel.c' главной функцией является 'kernel_main'.
.extern kernel_main
 
// Мы объявляем метку 'start' глобальной (чтобы она была доступна извне этого файла), поскольку компоновщику нужно будет знать, где он.
// Вскоре мы рассмотрим код, определяющий эту метку.
.global start
 
// Наш загрузчик, GRUB, должен знать некоторую базовую информацию о нашем ядре, прежде чем он сможет его загрузить.
// Мы передаем GRUB эту информацию, используя стандарт, известный как 'Multiboot'.
// Чтобы определить действительный 'Multiboot header', который будет распознаваться GRUB, нам нужно жестко закодировать некоторые
// константы. Следующий код вычисляет эти константы.
.set MB_MAGIC, 0x1BADB002          // Это "магическая" константа, которую GRUB будет использовать для определения местоположения ваших ядер.
.set MB_FLAGS, (1 << 0) | (1 << 1) // This tells GRUB to 1: load modules on page boundaries and 2: provide a memory map (this is useful later in development)

// Наконец, мы вычисляем контрольную сумму, которая включает все предыдущие значения.
.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_FLAGS))
 
// Теперь мы запускаем раздел исполняемого файла, который будет содержать наш заголовок 'Multiboot'.
.section .multiboot
	.align 4 // Убедитесь, что следующие данные выровнены по длине, кратной 4 байтам.
	// Используем ранее вычисленные константы в исполняемом коде
	.long MB_MAGIC
	.long MB_FLAGS
	// Используем контрольную сумму вычисленную раннее
	.long MB_CHECKSUM
 
// Этот раздел содержит данные, заполненные нулями при загрузке ядра.
.section .bss
	// Нашему коду C потребуется стек для запуска. Здесь мы выделяем 4096 байт (или 4 килобайта) для нашего стека.
	// Мы можем расширить его позже, если захотим больший стек. Пока этого будет вполне достаточно.
	.align 16
	stack_bottom:
		.skip 4096 // Резервируем стек размером 4096 байт (4K)
	stack_top:
 
// Этот раздел содержит наш фактический код сборки, который будет запускаться при загрузке ядра.
.section .text
	// Вот метка 'start', о которой мы упоминали ранее. Это первый код, который запускается в нашем ядре.
	start:
		// Перво-наперво: мы хотим настроить среду, готовую для запуска кода C.
		// C очень прост в своих требованиях: все, что нам нужно сделать, это настроить стек.
		// Обратите внимание, что на x86 стек растет ВНИЗ. Вот почему мы начинаем сверху.
		mov $stack_top, %esp // Установим указатель стека в верхнюю часть стека
 
		// Теперь у нас есть среда(достойная для запуска кода на си), готовая для запуска остальной части нашего ядра.
		// На этом этапе мы можем вызвать нашу основную функцию C.
		call kernel_main
 
		// Если по каким-то загадочным обстоятельствам код ядра C когда-либо возвращается, все, что нам нужно сделать, это повесить процессор
		hang:
			cli      // Отключаем прерывания процессора
			hlt      // Останавливаем процессор
			jmp hang // Если это не помогло, повторим попытку и попробуем еще раз.