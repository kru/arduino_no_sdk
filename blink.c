#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= (1 << DDB5); // Set PB5 (pin 13) as output
    while (1) {
        PORTB |= (1 << PORTB5); // LED on for 10s
        _delay_ms(10000);
        PORTB &= ~(1 << PORTB5); // LED off for 10s
        _delay_ms(10000);
    }
    return 0;
}
