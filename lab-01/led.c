#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

void blinkled(void){

        DDRB = 0x01;
                PORTB = 0x01;
                _delay_ms(500);
                PORTB = 0x00;
                _delay_ms(500);
}


