#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include "lcd_engine.h"
#include "touch.h"
#include <util/delay.h>
#include "buttons.h"
#include "gui.h"
#include "system.h"
#include "zigbeeny.h"
#include "dustwarsIDs.h"
#include "debug.h"
#include "ani.h"
int check_debug(LCD_engine *lcd ,TouchCalibration *TC){
if (UP_isPressed() && LEFT_isPressed() && DOWN_isPressed() && RIGHT_isPressed()){
if (A_isPressedAndRel() && UP_isPressed()){
return start_debug(lcd,TC);
}
}
return 0;
}

int start_debug(LCD_engine *lcd ,TouchCalibration *TC){


		//Start message on leds
	sendZigByte(ID_LEDS,ANI_FRIDAY);

LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
LCD_setPage(0);
LCD_setColumn(2);
rprintfStr("DEBUG leds");//rprintfInt(accX);rprintfStr("   ");
unsigned char sends=0;
unsigned char ani=0;

	while (1){
		if (START_isPressedAndRel()){break;}
		LCD_setPage(0);
		LCD_setColumn(2);
		rprintfStr("DEBUG leds");

		LCD_setPage(2);
		LCD_setColumn(2);
		rprintfStr("leds ");rprintfInt(ani);rprintfStr("   ");
LCD_setPage(3);
		LCD_setColumn(2);
		rprintfStr("sends ");rprintfInt(sends);rprintfStr("   ");
	if (UP_isPressedAndRel()){
		ani++;
	}		
	if (DOWN_isPressedAndRel()){
		ani--;
	}		
if (LEFT_isPressedAndRel()){
		ani=0;
	}		


	if (A_isPressedAndRel()){
		sendZigByte(ID_LEDS,ani);
	sends++;
	}

	
		}


return 0;
}


