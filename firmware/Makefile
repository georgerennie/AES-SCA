CFLAGS := -mmcu=atmega644p -DF_CPU=12000000 -Wall -Wextra -Os

.PHONY: build
build:
	avr-gcc $(CFLAGS) $(wildcard *.c) -o main.elf
	avr-objcopy -O ihex main.elf main.hex
	avrdude -c avrisp2 -p m644p -U flash:w:main.hex
