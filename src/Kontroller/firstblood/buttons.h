#ifndef Oo_BUTTONS_H_oO
#define Oo_BUTTONS_H_oO
#include "lcd_engine.h"
//This only uses one port for gamecontroller
//The other port can easly be added. But sice I've only made one controller this was not necessary at the time. 
//Init buttons
void button_init(void);
//Debugging
void test_buttons(LCD_engine *lcd);


unsigned char A_isPressed();
unsigned char START_isPressed();
unsigned char B_isPressed();
unsigned char DOWN_isPressed();
unsigned char LEFT_isPressed();
unsigned char RIGHT_isPressed();
unsigned char UP_isPressed();
unsigned char only_combo_isPressed(unsigned char combo);
unsigned char combo_isPressed(unsigned char combo);
unsigned char any_isPressed();

unsigned char RIGHT_isPressedAndRel();
unsigned char LEFT_isPressedAndRel();
unsigned char UP_isPressedAndRel();
unsigned char DOWN_isPressedAndRel();
unsigned char A_isPressedAndRel();
unsigned char B_isPressedAndRel();
unsigned char START_isPressedAndRel();
#endif
