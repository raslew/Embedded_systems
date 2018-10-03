#ifndef _TIMER_H_
#define _TIMER_H_

void timer_fast_PWM(void);

void timer_init(void);

void timer_CTC(void);

uint8_t simple_ramp(uint8_t duty_cycle);

void led_pulse_one_clock();

void led_pulse_two_clock();
#endif // _TIMER_H_
