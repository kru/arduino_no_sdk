#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define BUTTON_PIN  PD2
#define LED_PIN     PB5

int main(void) {
    // configure LED pin (PB5) as output
    DDRB |= (1 << LED_PIN);

    // configure button poin (PD2) as input with pull up resistor (internal)
    DDRD &= ~(1 << BUTTON_PIN); // input
    PORTD |= (1 << BUTTON_PIN); // Enable pull-up

    // Variables for button state and LED state
    uint8_t last_button_state = 1;
    uint8_t led_state = 0;

    while (1) {
        // Read current button state (0 = pressed, 1 = not pressed)
        uint8_t current_button_state = (PIND >> BUTTON_PIN) & 1;

        // Detect button press (HIGH to LOW transition)
        if (current_button_state == 0 && last_button_state == 1) {
            led_state = !led_state;

            if (led_state) {
                PORTB |= (1 << LED_PIN); // LED off
            } else {
                PORTB &= ~(1 << LED_PIN); // LED off
            }

            _delay_ms(500);
        }

        last_button_state = current_button_state;
    }

}
