#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <util/delay.h>

#include "led.h"
#include "serial.h"
#include "timer.h"

volatile uint8_t adc_value = 0;
volatile uint8_t b_state = 1;
uint8_t val = 1;
uint8_t counter = 0;

void adc_init(void){
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS1) | (1 << ADPS0);
	ADMUX |= (1 << REFS1) | (1 << ADLAR);
}

void button_state(){

	if ((PIND & (1 << PIND2)) == 4 && b_state == 1){
		b_state = 0;
	}
	else if ((PIND & (1 << PIND2)) == 0 &&  b_state == 0){
		b_state = 1;
		if(val == 4){
			val = 1;
		}
		else{
			val++;
		}
	}
}

void s_case(){
         switch (val){

                case 1:
                        OCR0A = simple_ramp();
                        break;
                case 2:
			ADCSRA |= (1 << ADSC);
                        break;
                case 3:
                        if(counter == 50){
				OCR0A = 0;
                        }
                        else if(counter >= 100){
                                OCR0A = 255;
                                counter = 0;
                        }
			counter++;
                        break;
                case 4:
			OCR0A = 0;
                        break;
                }
}

ISR(TIMER2_COMPA_vect){
	button_state();
	s_case();
}

ISR(ADC_vect){
	adc_value = ADCH;
	OCR0A = adc_value;
}

void main (void) {

	LED_init();
	uart_init();
	timer_init();
	adc_init();

	DDRD |= (0 << DDD2);
	sei();

	while (1) {
		;
	}
}

