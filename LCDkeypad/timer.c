/*
 * timer.c
 *
 * Created: 3.3.2021 10:21:59
 *  Author: Jakub
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

#define NOP() asm volatile ("nop")

volatile uint16_t timer1overflowCount = 0;

ISR(TIMER1_OVF_vect)
{
	timer1overflowCount++;//Obsluha preruseni
}

uint32_t getTime()
{
	return (((uint32_t)timer1overflowCount<<16) + TCNT1);
}

void timerInit()
{
	TCCR1B = (1<<CS11);//preddelicka 8
	TIMSK1 = (1<<TOIE1);//povoleni preruseni
}

void busyDelay(uint32_t us)
{
	us -= 2;
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	for(;us != 0; --us)
	{
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
	}
}

void delay(uint32_t us)
{
	us *= 2;
	us += getTime();
	while (us > getTime())
	{}
}
