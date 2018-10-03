#include <avr/io.h>
#include <stdlib.h>
#include "timer.h"

int state = 0;
int x = 255;
uint8_t duty_c;

void timer_fast_PWM(void){

	TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
	TCCR0B |= (1 << CS01) | (1 << CS00);
	TCNT0 = 0;
	OCR0A = 0;
}

void timer_init(void) {

        TCCR0A |= (1 << WGM01);
        OCR0A = 77;
        TCNT0 = 0;
        TCCR0B |= (1 << CS02) | (1 << CS00);

}

void timer_CTC(void){

	TCCR2A |= (1 << WGM21);
        OCR2A = 124;
        TCNT2 = 0;
        TCCR2B |= (1 << CS02) | (1<<CS01) | (1 << CS00);

}

uint8_t simple_ramp(uint8_t duty_cycle){

	while(!(TIFR2 & (1 << OCF2A)))
		;
	if (duty_cycle == 255 || duty_cycle == 0){
		state = abs(state -1);
	}
	TIFR2 |= (1 << OCF2A);
	if (state){
		duty_cycle += 1;
		return duty_cycle;
	}
	duty_cycle -= 1;
	return duty_cycle;
}

void led_pulse_one_clock(){

                while (!(TIFR0 & (1 << OCF0B)))
                        ;
                TIFR0 |= (1 << OCF0B);
                while (!(TIFR0 & (1 << OCF0A)))
                        ;
                TIFR0 |= (1 << OCF0A);
                PORTD ^= (1 << PD6);
                x -= 1;
                OCR0A = abs(x);
                if (x <= -255) x = 255;
}

void led_pulse_two_clock(void){

	while (!(TIFR0 & (1 <<OCF0B)))
        ;
	TIFR0 |=  (1 << OCF0B);
	PORTD ^= (1 << PD6);
	duty_c = OCR0A;
	OCR0A = simple_ramp(duty_c);
}
