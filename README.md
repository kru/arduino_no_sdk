## Run Program

- Board to tested the program is Arduino Uno R3
- User needs to adjust file name (`lm35_read_temp.*` to filename user wants to upload) the in the `Makefile` everytime (we'll adjust this later)

```
elf_compile:
	avr-gcc -Wall -mmcu=atmega328p -Os -o lm35_read_temp.elf ./lm35_read_temp.c

hex_compile:
	avr-objcopy -O ihex -R .eeprom lm35_read_temp.elf lm35_read_temp.hex

upload:
	avrdude -p atmega328p -c stk500v1 -P /dev/ttyACM0 -b 115200 -v -V -F -U flash:w:lm35_read_temp.hex
```
