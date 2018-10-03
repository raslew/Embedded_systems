#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <string.h>

#include "i2c.h"

void i2c_init(void) {

	TWBR |= 72;
	TWSR |= (0 << TWPS1 | 0 << TWPS0);
	TWCR |= (1 << TWEN);
}

void i2c_meaningful_status(uint8_t status) {
	switch (status) {
		case 0x08: // START transmitted, proceed to load SLA+W/R
			printf_P(PSTR("START\n"));
			break;
		case 0x10: // repeated START transmitted, proceed to load SLA+W/R
			printf_P(PSTR("RESTART\n"));
			break;
		case 0x38: // NAK or DATA ARBITRATION LOST
			printf_P(PSTR("NOARB/NAK\n"));
			break;
		// MASTER TRANSMIT
		case 0x18: // SLA+W transmitted, ACK received
			printf_P(PSTR("MT SLA+W, ACK\n"));
			break;
		case 0x20: // SLA+W transmitted, NAK received
			printf_P(PSTR("MT SLA+W, NAK\n"));
				break;
		case 0x28: // DATA transmitted, ACK received
			printf_P(PSTR("MT DATA+W, ACK\n"));
			break;
		case 0x30: // DATA transmitted, NAK received
			printf_P(PSTR("MT DATA+W, NAK\n"));
			break;
		// MASTER RECEIVE
		case 0x40: // SLA+R transmitted, ACK received
			printf_P(PSTR("MR SLA+R, ACK\n"));
			break;
		case 0x48: // SLA+R transmitted, NAK received
			printf_P(PSTR("MR SLA+R, NAK\n"));
			break;
		case 0x50: // DATA received, ACK sent
			printf_P(PSTR("MR DATA+R, ACK\n"));
			break;
		case 0x58: // DATA received, NAK sent
			printf_P(PSTR("MR DATA+R, NAK\n"));
			break;
		default:
			printf_P(PSTR("N/A %02X\n"), status);
			break;
	}
}

inline void i2c_start(void) {

	TWCR |= (1 << TWINT | 1 << TWSTA);
	while (! (TWCR & (1 << TWINT)))
		;
}

inline void i2c_stop(void) {
	TWCR |= (1 << TWINT | 1 << TWEN | 1 << TWSTO);
}

inline uint8_t i2c_get_status(void) {
	return  TWSR & 0xF8;
}

inline void i2c_xmit_addr(uint8_t address, uint8_t rw) {

	TWDR = address + rw;
	TWCR &= ~(1 << TWSTA);
	TWCR |= (1 << TWINT);
	while (! (TWCR & (1 << TWINT)))
		;
}

inline void i2c_xmit_byte(uint8_t data) {

	TWDR = data;
	TWCR |= (1 << TWINT);
	while (! (TWCR & (1 << TWINT)))
                ;
}

inline uint8_t i2c_read_ACK(void) {

	TWCR |= (1 << TWINT | 1 << TWEA);
	while (! (TWCR & (1 << TWINT)))
		;
	return TWDR;
}

inline uint8_t i2c_read_NAK(void) {

	TWCR |= (1 << TWINT | 0 << TWEA);
	while (! (TWCR & (1 << TWINT)))
		;
	return TWDR;
}

inline void eeprom_wait_until_write_complete() {

	do {
	i2c_start();
	i2c_xmit_addr(CONTROL_BYTE, I2C_W);
	}
	while(0x18 != i2c_get_status());
}

uint8_t eeprom_read_byte(uint8_t addr) {

	uint8_t eeprom_val;

	i2c_start();
	i2c_xmit_addr(CONTROL_BYTE, I2C_W);
	i2c_xmit_byte(addr);

	i2c_start();
	i2c_xmit_addr(CONTROL_BYTE, I2C_R);

	eeprom_val = i2c_read_NAK();
	i2c_stop();

	return eeprom_val;
}

void eeprom_write_byte(uint8_t addr, uint8_t data) {

	i2c_start();
	i2c_xmit_addr(CONTROL_BYTE, I2C_W);
	i2c_xmit_byte(addr);
	i2c_xmit_byte(data);
	i2c_stop();
	eeprom_wait_until_write_complete();
}

void eeprom_write_page(uint8_t addr, uint8_t *data, uint8_t len) {

	uint8_t *ptr = data;

	i2c_start();
	i2c_xmit_addr(CONTROL_BYTE, I2C_W);
	i2c_xmit_byte(addr);
	for(int i=0; i<len; i++){
		printf_P(PSTR("%c"), *ptr);
		i2c_xmit_byte(*(ptr++));
	}
	i2c_stop();
	eeprom_wait_until_write_complete();
}

void eeprom_sequential_read(uint8_t *buf, uint8_t start_addr, uint8_t len) {

	i2c_start();
        i2c_xmit_addr(CONTROL_BYTE, I2C_W);
        i2c_xmit_byte(start_addr);

        i2c_start();
        i2c_xmit_addr(CONTROL_BYTE, I2C_R);

	for(int i=0; i<len; i++){

		if(i+1 == len){
			*buf = i2c_read_NAK();
		}
		else{
			*buf++ = i2c_read_ACK();
		}
	}
        i2c_stop();
}
