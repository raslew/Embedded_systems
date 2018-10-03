#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "adc.h"
#include "gpio.h"
#include "i2c.h"
#include "serial.h"
#include "timer.h"
uint8_t status;
uint8_t rw = 0;
uint8_t eep_adr = 0x00;
uint8_t eep_adr2 = 0x0A;
uint8_t eep_adr3 = 0x18;
uint8_t data = 0x09;
uint8_t read_mem = 0xFA;
char name[] = "Rasmus";
char lname[] = "Lewinsso";
char fname[] = "Rasmus Lewinsson";
uint8_t buffer[256];
uint8_t len = 6;
uint8_t len2 = 8;

void main (void) {

	i2c_init();
	uart_init();

	sei();

	/*for(int i=0; i<=5; i++){
                        eeprom_write_byte(eep_adr++, name[i]);
                }
	eep_adr = 0x00;*/

	//eeprom_write_page(eep_adr2, lname, len2);

	eeprom_slicer(eep_adr3, fname, sizeof(fname)-1);

	while (1) {
		_delay_ms(2000);

		/*for(int i=0; i<=5; i++){
			printf_P(PSTR("%c"),eeprom_read_byte(eep_adr++));
		}
		eep_adr = 0x00;*/

		eeprom_sequential_read(buffer, eep_adr3, sizeof(fname)-1);
		for(int i=0; i<sizeof(fname)-1; i++){
			printf_P(PSTR("\n %x = %x"),eep_adr3++, buffer[i]);
		}
	}
}

void eeprom_slicer(uint8_t eep_adr3, uint8_t *fname, uint8_t len_array){

	uint8_t *ptr = fname;
	int counter = 0;
	uint8_t fname_sliced[7];
	uint8_t *ptr2 = fname_sliced;
	for(int i=0; i<len_array; i++){

		*(ptr2++) = *(ptr++);

		if(counter == 7){

			ptr2 = fname_sliced;
			eeprom_write_page(eep_adr3, ptr2, counter + 1);
			eep_adr3 += 8;
			counter = -1;
		}

		else if(i+1 == len_array){

			ptr2 = fname_sliced;
			eeprom_write_page(eep_adr3, ptr2, counter);
			break;
		}
		counter++;
	}
}
