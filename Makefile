all: elf_compile hex_compile upload clean

elf_compile:
	avr-gcc -Wall -mmcu=atmega328p -Os -o lm35_read_temp.elf ./lm35_read_temp.c

hex_compile:
	avr-objcopy -O ihex -R .eeprom lm35_read_temp.elf lm35_read_temp.hex

upload:
	avrdude -p atmega328p -c stk500v1 -P /dev/ttyACM0 -b 115200 -v -V -F -U flash:w:lm35_read_temp.hex

clean:
	rm -r *.elf *.hex
