#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "drivning.h"

void init_drivning() {
	DDRD = 0xff;
	DDRC = 0x00;

	ICR1 = 10000;
	TCCR1A |= (1 << COM1A0) | (1 << COM1B0) | (1 << COM1A1) | (1 << COM1B1)
			| (0 << WGM11) | (0 << WGM10);
	TCCR1B |= (1 << CS11) | (1 << WGM13) | (0 << WGM12);

	OCR1A = MOTOR_A_STOP;
	OCR1B = MOTOR_B_STOP;
}
void stop_motors() {
	TCCR1B &= ~(1 << CS11);
}
void start_motors() {
	TCCR1B |= (1 << CS11);
}
void set_motor_A(int value) {
	OCR1A = value;
}
void set_motor_B(int value) {
	OCR1B = value;
}

