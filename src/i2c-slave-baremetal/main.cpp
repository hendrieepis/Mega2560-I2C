#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SLAVE_ADDRESS 0x08
#define LED_INDICATOR_PIN PB7
#define F_CPU 16000000UL  // Assuming 16MHz clock, adjust if different

volatile uint8_t receivedData = 0;

void TWI_init(void) {
    // Set slave address
    TWAR = (SLAVE_ADDRESS << 1);
    
    // Enable TWI and TWI interrupt
    TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
}

void IO_init(void) {
    // Set PORTA as output for BarLED
    DDRA = 0xFF;
    
    // Set PB7 as output for LED indicator
    DDRB |= (1 << LED_INDICATOR_PIN);
    // Aktifkan pull-up pada SDA (PD1) dan SCL (PD0)
    PORTD |= (1 << PD1) | (1 << PD0);  // Aktifkan pull-up internal pada pin SDA dan SCL
}

ISR(TWI_vect) {
    uint8_t status = TWSR & 0xF8;

    switch(status) {
        case 0x60: // Own SLA+W has been received, ACK has been returned
        case 0x68: // Arbitration lost in SLA+R/W as Master; Own SLA+W has been received, ACK returned
        case 0x70: // General call address received, ACK returned
        case 0x78: // Arbitration lost in SLA+R/W as Master; General call address received, ACK returned
            TWCR |= (1 << TWEA);  // Send ACK
            break;

        case 0x80: // Previously addressed with own SLA+W; data has been received; ACK has been returned
        case 0x90: // Previously addressed with general call; data has been received; ACK has been returned
            receivedData = TWDR;
            TWCR |= (1 << TWEA);  // Send ACK
            
            // Update BarLED (Common Anode)
            PORTA = ~receivedData;
            
            // Turn on LED indicator
            PORTB |= (1 << LED_INDICATOR_PIN);
            _delay_ms(500);
            PORTB &= ~(1 << LED_INDICATOR_PIN);
            break;

        case 0xA0: // A STOP condition or repeated START condition has been received while still addressed as slave
            TWCR |= (1 << TWEA);  // Send ACK
            break;

        default:
            break;
    }

    // Clear TWI interrupt flag
    TWCR |= (1 << TWINT);
}

int main(void) {
    IO_init();
    TWI_init();
    
    // Enable global interrupts
    sei();

    while(1) {
        // Main loop is empty as all work is done in ISR
    }

    return 0;
}