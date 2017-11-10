#include "ultraljud.h"

//#define F_CPU 1000000

//
//======================================================================================================
//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//
//======================================================================================================
//
/****************************************************************************
 ULTRA MODULE FUNCTIONS

 *****************************************************************************/

int temp1, temp2, distance, ticks, interrupts = -1, interrupt_start = 0,
		distance = 0;
float temp_float, distance_factor = 1.374f;
char newDistance = 0, calulating = 0;
char owerflowed = 0;
//
//======================================================================================================
//
int calc_distance();

ISR(SIG_OVERFLOW0)
{
	owerflowed++;
}

ISR(SIG_INTERRUPT0)
{
	if(interrupts==1) //Negative edge from module
	{
		GICR = 0x00; //Disable local interrupt
		TCCR0 = 0x00; //stop timer
		//if (owerflowed)
		//{
		//    distance=255;
		//}
		//else
		//{
		distance = calc_distance(owerflowed);
		//}
		newDistance=1;
		interrupts=0;
		calulating=0;
	}

	else if (interrupts==0) //Positive edge from module, start timer
	{

		TCNT0=0x00;
		TCCR0 = 0x03; //Set prescaler to 64
		MCUCR=0x02; //falling edge
		interrupts=1; //Change the action taken after next timer/counter capture interrupt
	}

}

void start_ultraljudmatning() {
	//cli();
	if (calulating) {
		return;
	}

	interrupts = -1;
	DDRD |= 0x04;

	PORTD |= 0x04; //Set PD2 to HIGH for 5µs to initiate distance measuring
	_delay_us(5);
	PORTD &= ~(0x04);
	DDRD &= ~(0x04);

	TIMSK |= 0x01;
	GICR = 0x40; //activate INT0
	MCUCR = 0x03; //rising edge

	calulating = 1;
	sei();
	owerflowed = 0;
	interrupts = 0;
}

int calc_distance(int times) {
	ticks = TCNT0;
	ticks += 255 * times;
	temp_float = ticks * distance_factor;
	return (int) temp_float;
}

int get_distance() {
	if (newDistance) {
		newDistance = 0;
		return distance;
	} else {
		return -1;
	}
}
//
//======================================================================================================
//
