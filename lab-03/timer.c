#include <avr/io.h>
#include <stdlib.h>
#include "timer.h"


void timer_init() {
	//set PWN
	TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1);
	TCCR0B |= (1 << CS01) | (1 << CS00);

	//set CTC
	TCCR2A |= (1 << WGM21);
	OCR2A = 77;
	TCCR2B |= (1 << CS22) | (1<<CS21) | (1 << CS20);
	TIMSK2 |= (1 << OCIE2A);
}
