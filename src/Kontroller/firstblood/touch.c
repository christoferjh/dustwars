#include "touch.h"
#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include "system.h" 
#include <avr/interrupt.h> 
#include "buttons.h"

/*

 BOTTOM 		=	PA0 
 LEFT		= 	PA3
 TOP			= 	PA2
 RIGHT		=	PA1

 Rigth -> 700 ohm -> GND

 */

//0 = off
//1 = listen for press
//2 = read y
//3 = read x

unsigned char TOUCH_READING;
//My private touch point. Needed because this is interrupt programming
Point private_touch_point;

//ADC finish interrupt
ISR(ADC_vect)
{

	int value=ADCH; //Read from ADC result
	if (TOUCH_READING) {//Read y
		if (value<=35) {//Stopt pressing
			private_touch_point.y=0;
			private_touch_point.x=0;
			private_touch_point.down_count=0;
			TOUCH_STATE=TOUCH_STATE_WAITING;
			return;
		}

		private_touch_point.y=value;
		set_x_reading();
		TOUCH_READING=0;
		start_reading_with_interrupt();

	} else {// Read x
		private_touch_point.x=value;
		private_touch_point.down_count++;
		if (private_touch_point.down_count>250) {
			private_touch_point.down_count=100;
		}
		TOUCH_READING=1;
		set_y_reading();
		TOUCH_STATE=TOUCH_STATE_PRESSED;
	}

}

//Return the raw touch
Point getRawTouch() {
	Point P;

	P.y = private_touch_point.y;
	P.x = private_touch_point.x;
	P.down_count = private_touch_point.down_count;

	return P;
}

//CHANGE TO NON GLOBAL TC
//Returns the touch point in screen koordinates
Point getTouch(TouchCalibration *TC) {
	Point P;

	P.y = private_touch_point.y * TC->k_y / 100 + TC->m_y;
	P.x = private_touch_point.x * TC->k_x / 100 + TC->m_x;
	P.down_count = private_touch_point.down_count;
	return P;
}
//Set the calibration
void set_calibration(TouchCalibration *TC, unsigned char diff_x,
		unsigned char m_x, unsigned char diff_y, unsigned char m_y) {
	TC->k_x = -3000 / (diff_x);
	TC->k_y = -2400 / (diff_y);
	TC->m_x = m_x;
	TC->m_y = m_y;
}

//Load calibration from persistent eeprom memory
void load_eeprom_touch(TouchCalibration *TC) {
	unsigned char diff_x = eeprom_read(11);
	unsigned char m_x = eeprom_read(12);
	unsigned char diff_y = eeprom_read(13);
	unsigned char m_y = eeprom_read(14);

	set_calibration(TC, diff_x, m_x, diff_y, m_y);

}

//Init that the touch will be handeled with interrupt
void touch_init_advance(TouchCalibration *TC) {
	private_touch_point.y = 0;
	private_touch_point.x = 0;
	ADMUX = 0b00100000; //single ended ADC0

	ADCSRA = 0b10000000; //enable 

	ADCSRA |= (1 << ADIE); // Enable ADC Interrupt
	sei(); // Enable Global Interrupts

	//read from eeprom
	load_eeprom_touch(TC);
	TOUCH_STATE = TOUCH_STATE_WAITING;
	set_y_reading();
	start_reading_with_interrupt();

}
//Init that the touch will be handeled with manually, not good, and not used now
void touch_init(void) {
	DDRA = 0x00;

	ADMUX = 0b00100000; //single ended ADC0

	ADCSRA = 0b10000000; //enable 

	ADCSRA |= (1 << ADIE); // Enable ADC Interrupt
	//sei();   // Enable Global Interrupts, done in main entry


}
//Start next conversion
void start_reading_with_interrupt() {
	ADCSRA |= (1 << ADIE); // Enable ADC Interrupt
	ADCSRA |= 0b01000000; //start next
}
//Set pins to read y value
void set_y_reading() {
	ADCSRA = 0x00;
	DDRA = 0xFF; //Allt ut
	PORTA = 0x00; //GND
	DDRA = 0x00; //Allt in
	PORTA = 0x00; //varför?
	ADMUX = 0b00100001; //single ended ADC1
	ADCSRA = 0b10000000; //enable 
	//Put the correct as in and another as out
	DDRA |= (1 << DDA0) | (1 << DDA2);
	PORTA |= (1 << PINA2);
	PORTA &= ~(1 << PINA0);

}
//Set pins to read x value
void set_x_reading() {
	ADCSRA = 0x00;
	DDRA = 0xFF; //Allt ut
	PORTA = 0x00; //GND
	DDRA = 0x00; //Allt in
	PORTA = 0x00; //varför?
	ADMUX = 0b00100000; //single ended ADC1
	ADCSRA = 0b10000000; //enable 
	//Put the correct as in and another as out
	DDRA |= (1 << DDA1) | (1 << DDA3);
	PORTA |= (1 << PINA3);
	PORTA &= ~(1 << PINA1);
}

//For manuall reading...
int get_touch_x() {
	set_x_reading();
	ADCSRA |= 0b01000000; //start 

	while ((ADCSRA & ADSC) >> ADSC != 0) {
	}

	return ADCH;
}

//For manuall reading...
int get_touch_y() {

	set_y_reading();
	ADCSRA |= 0b01000000; //start 

	while ((ADCSRA & ADSC) >> ADSC != 0) {
	}

	return ADCH;
}

//Calibrate
void calibrate_touch(LCD_engine *lcd, TouchCalibration *TC) {
	//Fix screen
	LCD_CLR();
	setDubbelbuffer(lcd, 1);
	LCD_RAM_CLR(lcd);
	DOGENABLE;

	/*
	 Calculations
	 x:  x=k_x*r_x+m_x
	 tryck 55 få x_0
	 tryck 5 få x_1
	 tryck 110 få x_2
	 k_x=(55-5)/(x_0-x_1);
	 m_x=55-k_x;
	 //This is a wery simple variant.
	 */

	//Looks pretty messy, and it is...
	Point p;
	int k_x = 0;
	unsigned char m_x = 0;
	int x_0 = 0;
	int X, Y;
	int old_X = 0;
	int old_Y = 0;
	int k_y = 0;
	unsigned char m_y = 0;
	unsigned char diff_x = 0;
	unsigned char diff_y = 0;
	int y_0 = 0;
	char count = 0;
	char down = 0;
	LCD_setPage(2);
	LCD_setColumn(22);
	rprintfStr("x");
	ADCSRA |= 0b01000000; //start 
	while (1) {

		if (START_isPressedAndRel()) {//Exit
			break;
		}
		p = getRawTouch();
		Y = (p.y * k_y) / 100 + m_y;
		X = (p.x * k_x) / 100 + m_x;

		LCD_setPage(0);
		LCD_setColumn(0);
		rprintfInt(p.down_count);
		LCD_setColumn(90);
		rprintfStr("c:");
		rprintfInt(count);
		rprintfStr("  ");
		if (p.down_count > 0) {//PRESSED
			if (down == 5) {
				if (count == 0) {//save value and get next
					x_0 = p.x;
					y_0 = p.y;
					LCD_setPage(2);
					LCD_setColumn(22);
					rprintfStr(" ");
					LCD_setPage(5);
					LCD_setColumn(52);
					rprintfStr("x");

				}
				if (count == 1) {//Finish
					diff_x = x_0 - p.x;
					diff_y = y_0 - p.y;
					k_x = -3000 / (diff_x);
					m_x = 25 - (k_x * x_0) / 100;

					k_y = -2400 / (diff_y);
					m_y = 21 - (k_y * y_0) / 100;
					//Write to EEPROM
					eeprom_write(11, diff_x);//
					eeprom_write(12, m_x);//
					eeprom_write(13, diff_y);//
					eeprom_write(14, m_y);//
					//CHANGE FROM GLOBAL
					TC->k_y = k_y;
					TC->k_x = k_x;
					TC->m_x = m_x;
					TC->m_y = m_y;

				}

				count++;

			}

			down++;
			if (down > 5 && k_x != 0) {//Finish, let user test the calibration by displaying a horizontial and vertical line intersecting the touched point
				pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
				LCD_drawLine(lcd, 0, old_Y, 127, old_Y);
				LCD_drawLine(lcd, old_X, 0, old_X, 63);
				pixelSetOrClearOrInvert(lcd, SET_PIXELS);
				LCD_drawLine(lcd, 0, Y, 127, Y);
				LCD_drawLine(lcd, X, 0, X, 63);
				old_Y = Y;
				old_X = X;
			}

			if (down > 10)
				down = 10;
		} else {

			if (k_x != 0) {
				pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
				LCD_drawLine(lcd, 0, old_Y, 127, old_Y);
				LCD_drawLine(lcd, old_X, 0, old_X, 63);
			}
			down = 0;
		}

		if (A_isPressed()) {//Debugging only
			LCD_setPage(2);
			LCD_setColumn(0);
			rprintfStr("diff_x:");
			rprintfInt(diff_x);
			rprintfStr(" m_x:");
			rprintfInt(TC->m_x);
			LCD_setPage(3);
			LCD_setColumn(0);
			rprintfStr("diff_y:");
			rprintfInt(diff_y);
			rprintfStr(" m_y:");
			rprintfInt(TC->m_y);

		}
		pushRAM(lcd);

	}

}
//Read from eeprom
unsigned char eeprom_read(unsigned int adress) {
	while (EECR & (1 << EEPE)) {
	} //Wait if already writing
	EEAR = adress;
	EECR |= (1 << EERE);
	return EEDR;
}
//Write to eeprom
void eeprom_write(unsigned int adress, unsigned char data) {
	while (EECR & (1 << EEPE)) {
	} //Wait if already writing
	EEAR = adress;
	EEDR = data;
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);

}

//Debugging
void test_touch(LCD_engine *lcd) {

	LCD_CLR();
	DOGENABLE;
	LCD_setPage(0);
	LCD_setColumn(10);
	rprintfStr("Hello TOUCH-test!ö");
	while (1) {
		if (START_isPressed()) {
			break; //Exit
		}
		LCD_setPage(1);
		LCD_setColumn(10);
		rprintfStr("t:");
		rprintfInt( sys_time);
		rprintfStr("      ");
		LCD_setPage(2);
		LCD_setColumn(10);
		if (TOUCH_STATE == TOUCH_STATE_PRESSED) {
			Point p = getRawTouch();

			rprintfStr("P ");
			rprintfInt(p.x);
			rprintfStr(",");
			rprintfInt(p.y);
			rprintfStr("      ");
		}

		LCD_setPage(4);
		LCD_setColumn(10);
		rprintfInt( ADCH);
		rprintfStr("  ");
	}
}
