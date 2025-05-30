#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// LCD pin definition
#define E (1<<PD3)
#define RS (1<<PD2)
#define D4 (1<<PD4)
#define D5 (1<<PD5)
#define D6 (1<<PD6)
#define D7 (1<<PD7)
#define A0 (1<<PC0)


void adc_init() {
    // set REFS1 = 0 | REFS0 = 1 (Vref as AVCC pin) | ADLAR = 0 (right adjusted) | MUX4 to MUX0 is 0000 for ADC0
    ADMUX = 0b01000000;
    // enable ADC module, set prescalar of 128 which give CLK/128
    ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

int adc_read(unsigned char channel) {
    // set input channel to read
    ADMUX = 0x40 | (channel & 0x07); // 0100 0000 | (channel & 0000 0100)
    // wait until end of conversion by polling ADC interrupt flag
    while (!(ADCSRA & (1<<ADIF)));
    // clear interrupt flag
    ADCSRA |= (1<<ADIF);
    _delay_ms(1);
    // return ADC word
    return ADCW;
}

void lcd_init() {
    // initialize LCD
    DDRD |= E | RS | D4 | D5 | D6 | D7;
    // send pulse to latch the data
    latch();
    _delay_ms(2); // delay for stable power
    // command to setup the LCD
    lcd_cmd(0x33);
    _delay_us(100);
    lcd_cmd(0x32);
    _delay_us(100);
    // 2 lines 5x7 matrix dot
    lcd_cmd(0x28);
    _delay_us(100);
    // display ON, Cursor ON
    // lcd_cmd(0x0E);
     lcd_cmd(0x0C);
     _delay_us(100);
     //clear LCD
     lcd_cmd(0x01);
     _delay_ms(20);
     // shift cursor to right
     lcd_cmd(0x06);
     _delay_ms(10);
}

void latch(void) {
    // send high
    PORTD |= E;
    // wait
    _delay_us(500);
    // send low
    PORTD &= ~E;
    // wait
    _delay_us(500);
}

void lcd_cmd(uint8_t cmd) {
    // send high nibble
    PORTD = (PORTD & 0x0F) | (cmd & 0xF0);
    // send 0 to select command register
    PORTD &= ~RS;
    // latch the data
    latch();
    // send low nibble
    PORTD = (PORTD & 0x0F) | (cmd<<4);
    latch();
}

void lcd_char(uint8_t data) {
    // send hight nibble
    PORTD = (PORTD & 0x0F) | (data & 0xF0);
    // send 1 to select data register
    PORTD |= RS;
    latch();
    // send high nibble
    PORTD = (PORTD & 0x0F) | (data<<4);

    latch();
}

void lcd_print(char *str) {
    uint8_t k=0;
    while(str[k] != 0) {
        lcd_char(str[k]);
        k += 1;
    }
}

void lcd_set_cursor(uint8_t x, uint8_t y) {
    uint8_t first_char_addr[] = {0x80, 0xC0, 0x94, 0xD4};
    lcd_cmd(first_char_addr[y-1] + x-1);
    _delay_us(1000);
}

int main() {
    lcd_init();
    adc_init();

    char temp[10];
    float celcius;

    lcd_set_cursor(1,1);
    lcd_print("Temperature:");

    while(1) {
        celcius = (adc_read(A0) * 4.88);
        celcius = (celcius/10.00);
        dtostrf(celcius, 6, 2, temp);

        lcd_set_cursor(1,2);
        lcd_print(temp);
        lcd_char(0xDF);
        lcd_print("C");
    }

    return 0;
}
