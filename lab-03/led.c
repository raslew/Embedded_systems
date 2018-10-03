#include <avr/io.h>
#include "led.h"
#include <stdlib.h>

uint8_t state = 0;
uint8_t duty_cycle = 0;

void LED_init() {
	DDRD |= (1 << DDD6);
}

uint8_t simple_ramp(void) {

        if (duty_cycle == 255 || duty_cycle == 0){
                state = abs(state -1);
        }
        if (state){
                duty_cycle += 1;
                return duty_cycle;
        }
        duty_cycle -= 1;
        return duty_cycle;
}



