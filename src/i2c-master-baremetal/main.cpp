#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define F_CPU 16000000UL  // Frekuensi CPU, sesuaikan dengan clock Anda
#define SCL_FREQ 100000L  // Frekuensi I2C yang diinginkan (100 kHz)
#define SLAVE_ADDRESS 0x08 // Alamat slave

void i2c_init(void) {
    // Set SCL frequency
    TWSR = 0x00;  // Prescaler value = 1
    TWBR = ((F_CPU / SCL_FREQ) - 16) / 2;

    // Enable TWI
    TWCR = (1 << TWEN);
}

uint8_t i2c_start(uint8_t address) {
    // Send START condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    // Wait for TWINT flag set
    while (!(TWCR & (1 << TWINT)));
    
    // Check if START was sent successfully
    if ((TWSR & 0xF8) != TW_START && (TWSR & 0xF8) != TW_REP_START)
        return 1;

    // Send slave address with Write flag
    TWDR = address;
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    // Wait for TWINT flag set
    while (!(TWCR & (1 << TWINT)));
    
    // Check if SLA+W was sent and ACK received
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
        return 2;

    return 0;
}

void i2c_stop(void) {
    // Send STOP condition
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    
    // Wait for STOP to be executed
    while (TWCR & (1 << TWSTO));
}

uint8_t i2c_write(uint8_t data) {
    // Load data into TWDR register
    TWDR = data;
    
    // Start transmission
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    // Wait for TWINT flag set
    while (!(TWCR & (1 << TWINT)));
    
    // Check if data was sent and ACK received
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
        return 1;

    return 0;
}

int main(void) {
    uint8_t dataToSend = 0;

    i2c_init();

    while (1) {
        if (i2c_start((SLAVE_ADDRESS << 1) | TW_WRITE) == 0) {
            if (i2c_write(dataToSend) == 0) {
                // Data sent successfully
            } else {
                // Error in sending data
            }
            i2c_stop();
        } else {
            // Error in starting communication
        }

        // Update data to send (0 to 255 then back to 0)
        dataToSend++;
        if (dataToSend > 255) {
            dataToSend = 0;
        }

        _delay_ms(1000);  // Wait for 1 second before next transmission
    }

    return 0;
}