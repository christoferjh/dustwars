#ifndef Oo_TOUCH_H_oO
#define Oo_TOUCH_H_oO
#include "lcd_engine.h"

//Holding the calibration that takes the raw touch values to screen koordinates 
typedef struct TouchCalibration{
int k_x;
unsigned char m_x;
int k_y;
unsigned char m_y;
unsigned char calibrated; 

}TouchCalibration;

//Touch point, raw or not
typedef struct Point{

int x;
int y;
unsigned char down_count;
}Point;
//Interrupt programming demands global variables
unsigned char TOUCH_STATE;


void touch_init(void);
void calibrate_touch(LCD_engine *lcd,TouchCalibration *TC);
int get_touch_x();
int get_touch_y();
Point getTouch(TouchCalibration *TC);
Point getRawTouch();
void eeprom_write(unsigned int adress,unsigned char data);
unsigned char eeprom_read(unsigned int adress);
void start_adc_check();
void touch_init_advance(TouchCalibration *TC);
void set_x_reading();
void set_y_reading();
void set_calibration(TouchCalibration *TC,unsigned char diff_x,unsigned char m_x,unsigned char diff_y,unsigned char m_y);
void test_touch(LCD_engine *lcd);
void start_reading_with_interrupt();

//CHANGE FROM GLOBAL
//TouchCalibration TC;

//Not using all of these, but i'll like to.
#define TOUCH_STATE_PRESSED 	2
#define TOUCH_STATE_WAITING 	1
#define TOUCH_STATE_OFF 		0
#define ACTION_DOWN 			1
#define ACTION_DRAG 			2
#define ACTION_UP	 			3
#define ACTION_NONE 			0
#endif
