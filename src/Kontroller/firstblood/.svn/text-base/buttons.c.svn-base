#include <avr/io.h>
#include "connections.h"
#include "lcd_engine.h"
#include "touch.h"
//This only uses one port for gamecontroller
//The other port can easly be added. But sice I've only made one controller this was not necessary at the time. 


void button_init(void) {
	DD_BUTTONS =0x00;
	P_BUTTONS = 0xFF;
}
unsigned char only_combo_isPressed(unsigned char combo) {
	return R_BUTTONS == (~combo);
}
unsigned char combo_isPressed(unsigned char combo) {
	return ~R_BUTTONS & (combo);
}

unsigned char A_isPressed() {
	return ~R_BUTTONS & (1 << B_A);
}
unsigned char START_isPressed() {
	return ~R_BUTTONS & (1 << B_START);
}
unsigned char B_isPressed() {
	return ~R_BUTTONS & (1 << B_B);
}
unsigned char DOWN_isPressed() {
	return ~R_BUTTONS & (1 << B_DOWN);
}
unsigned char LEFT_isPressed() {
	return ~R_BUTTONS & (1 << B_LEFT);
}
unsigned char RIGHT_isPressed() {
	return ~R_BUTTONS & (1 << B_RIGHT);
}
unsigned char UP_isPressed() {
	return ~R_BUTTONS & (1 << B_UP);
}

unsigned char UP_isPressedAndRel() {
	if (!UP_isPressed())
		return 0;
	while (UP_isPressed()) {
	};
	return 1;
}

unsigned char DOWN_isPressedAndRel() {
	if (!DOWN_isPressed())
		return 0;
	while (DOWN_isPressed()) {
	};
	return 1;
}

unsigned char LEFT_isPressedAndRel() {
	if (!LEFT_isPressed())
		return 0;
	while (LEFT_isPressed()) {
	};
	return 1;
}

unsigned char RIGHT_isPressedAndRel() {
	if (!RIGHT_isPressed())
		return 0;
	while (RIGHT_isPressed()) {
	};
	return 1;
}

unsigned char A_isPressedAndRel() {
	if (!A_isPressed())
		return 0;
	while (A_isPressed()) {
	};
	return 1;
}

unsigned char B_isPressedAndRel() {
	if (!B_isPressed())
		return 0;
	while (B_isPressed()) {
	};
	return 1;
}

unsigned char START_isPressedAndRel() {
	if (!START_isPressed())
		return 0;
	while (START_isPressed()) {
	};
	return 1;
}

unsigned char any_isPressed() {
	return A_isPressed() || B_isPressed() || DOWN_isPressed()
			|| LEFT_isPressed() || RIGHT_isPressed() || UP_isPressed();
}

//Test the buttons
void test_buttons(LCD_engine *lcd) {

	LCD_CLR();
	resetConnection();
	LCD_setPage(0);
	LCD_setColumn(10);
	rprintfStr("Test Buttons ");
	LCD_setPage(1);
	LCD_setColumn(10);
	rprintfStr("START and touch to exit");
	LCD_setPage(3);
	LCD_setColumn(10);

	while (1) {
		LCD_setColumn(10);
		rprintfBin( R_BUTTONS);
		rprintfStr("        ");
		if (TOUCH_STATE == TOUCH_STATE_PRESSED && START_isPressedAndRel()) {
			return;
		}

	}
}
