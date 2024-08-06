/*
 * PMU_timer.c
 *
 * Created: 5/15/2024 5:16:05 PM
 *  Author: Admin
 */ 
#include <avr/interrupt.h>
#include "timer.h"

void	timer_start_timer0(void)
{	
	TCCR0B = (1<<CS01)|(1<<CS00);				//DIV = 64
	TIMSK0 |= (1<<OCIE0A);
	sei();
}
void timer_stop_timer0(void)
{
	TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
}
void timer_timer0_init(void)
{
	TCCR0A = (1<<WGM01);
	TCNT0 = 0;
	OCR0A = 125 - 1;
}