#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "led.h"

void main(void){

        DDRB = 0x01;
        uart_init();
        const char *str = "Rasmus\n";

        while(1){
                blinkled();
		uart_putstr(str);
                _delay_ms(500);
                uart_putchar('\n');
                _delay_ms(500);
		UDR0 =  '-';
		uart_echo();
        }
}

