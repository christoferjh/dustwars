#include "lcd_engine.h"
#include "buttons.h"
#include "touch.h"
#include "system.h"

//Paint CHANGE FILE
//Nice drawing program
void paint(LCD_engine *lcd, TouchCalibration *TC) {

	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	Point p;
	Point old_points[4];

	for (int i = 0; i < 4; i++) {
		old_points[i].x = 0;
		old_points[i].y = 0;
		old_points[i].down_count = 0;
	}

	while (1) {
		if (START_isPressedAndRel()) {
			break;
		}

		if (TOUCH_STATE == TOUCH_STATE_PRESSED) {//Someone is pressing

			p = getTouch(TC);
			if (p.down_count > 10) {//Accurate touch point?

				if (p.down_count != old_points[0].down_count) {
					//save old
					for (int i = 3; i > 0; i--) {
						old_points[i].x = old_points[i - 1].x;
						old_points[i].y = old_points[i - 1].y;
					}
					old_points[0].x = p.x;
					old_points[0].y = p.y;
					old_points[0].down_count = p.down_count;
				}

				if (old_points[3].x != 0 && old_points[3].y != 0) {
					LCD_drawPixel(lcd, (unsigned int) old_points[2].x,
							(unsigned int) old_points[2].y);
					LCD_drawLine(lcd, (unsigned int) old_points[3].x,
							(unsigned int) old_points[3].y,
							(unsigned int) old_points[2].x,
							(unsigned int) old_points[2].y);
				}
			}
		} else {
			for (int i = 0; i < 4; i++) {
				old_points[i].x = 0;
				old_points[i].y = 0;
			}
		}
		//Draw to lcd
		pushRAM(lcd);

	}

}
