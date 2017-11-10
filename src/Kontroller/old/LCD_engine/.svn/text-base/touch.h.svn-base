#ifndef Oo_TOUCH_H_oO
#define Oo_TOUCH_H_oO
#include "lcd_engine.h"
typedef struct TouchCalibration{

int k_x;
unsigned char m_x;
int k_y;
unsigned char m_y;
unsigned char calibrated; 

}TouchCalibration;

typedef struct Point{

int x;
int y;
unsigned char down_count;
}Point;

void touch_init(void);
void calibrate_touch(LCD_engine *lcd);
int get_touch_x();
int get_touch_y();
Point getTouch();
Point getRawTouch();
void eeprom_write(unsigned int adress,unsigned char data);
unsigned char eeprom_read(unsigned int adress);
void start_adc_check();
void touch_init_advance(void);
void set_x_reading();
void set_y_reading();
void set_calibration(unsigned char diff_x,unsigned char m_x,unsigned char diff_y,unsigned char m_y);
unsigned char TOUCH_STATE;
int ddd;
void start_reading_with_interrupt();
TouchCalibration TC;

#define TOUCH_STATE_PRESSED 	2
#define TOUCH_STATE_WAITING 	1
#define TOUCH_STATE_OFF 		0
#define ACTION_DOWN 			1
#define ACTION_DRAG 			2
#define ACTION_UP	 			3
#define ACTION_NONE 			0
#endif
