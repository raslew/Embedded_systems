#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "led.h"
#include "serial.h"
#include "timer.h"

void main (void) {

        uart_init();
        DDRD |= (1<<PD6);
	timer_fast_PWM();
	timer_CTC();
	while (1) {
		led_pulse_two_clock();
	}
}
