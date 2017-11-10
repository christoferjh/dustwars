#include "lcd_engine.h"
#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include <util/delay.h>
#include "touch.h"
#include <avr/interrupt.h> 
#include <stdlib.h>
#include "system.h"
#include "buttons.h"
#include "gui.h"
#include "zigbeeny.h"
#include "accelerometer.h"
#include "dustwarsIDs.h"
#include "game_missile.h"
#include "ball_game.h"
#include "images.h"
#include "settings.h"
#include "ani.h"
#include "debug.h"
//System time
ISR(TIMER0_OVF_vect) {
	sys_time++;

	if (TOUCH_STATE!=TOUCH_STATE_OFF) { //Time for next touch reading
		start_reading_with_interrupt();
	}
}








//Create and launch a new meny
void gui_screen_meny(LCD_engine *lcd, TouchCalibration *TC) {
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	ButtonsMeny BMA;
	BMA.title = "Settings";
	BMA.RootButton = 0;
	MenyButton b[3];
	b[2] = makeButton("Reversed", 1);
	b[1] = makeButton("Calibrate", 2);
	b[0] = makeButton("Dustwars", 3);
	addMenyButton(&BMA, &b[2]);
	addMenyButton(&BMA, &b[1]);
	addMenyButton(&BMA, &b[0]);
	int valde = 0;
	while (1) {
		valde = Buttons_meny(lcd, TC, &BMA, 2);
		if (valde == 1) {
			if (lcd->DISPINVERTED) {
				DisplaySetDispInverted(lcd, 0);
				b[2].text = "Reversed";
			} else {
				DisplaySetDispInverted(lcd, 1);
				b[2].text = "Normal";
			}

		}else if(valde==2){
			calibrate_touch(lcd, TC);
		} else {
			return;
		}
	}
}




//Create and launch a new meny
void leds_slider(LCD_engine *lcd, TouchCalibration *TC) {

LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
int downX = 0;
int leds=0;
int old_leds=1;
int slider_down=0;
int slider_up=0;
char a=0;
while (1){
if (START_isPressedAndRel()){break;}
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
//Scrollers
		//fillRect(lcd, 122, 30 - rotSpeed, 5, 4);
		fillRect(lcd, 0, 58-40, 4+slider_up*8, 5);
		fillRect(lcd, slider_down, 58, 4, 5);
		//Scrollers - end

if (TOUCH_STATE == TOUCH_STATE_PRESSED) {
			Point p = getTouch(TC);
			if (p.down_count > 8) {//Pressing
					if (p.y>32){
					leds=p.x;
					slider_down=p.x;
					}else{
					slider_up=p.x/8;
					leds=(0xFF<<(p.x/(127/8)));
					}
					if (leds!=old_leds){
						old_leds=leds;
						sendZigByte(ID_LEDS,leds);
					}
			}

		} else {
			downX = 0;
		//a++;	
		if (a==0){
		int acc=getAccX();
		if (acc>100){
		int nr_leds=(acc/(5000/8));
		leds=(0xFF<<nr_leds);
		slider_up=nr_leds*2;
		if (leds!=old_leds){
						old_leds=leds;
						sendZigByte(ID_LEDS,leds);
					}
		}
		
		}
			
		}


//Acc test
int accX=getAccX()/100;
int accY=getAccY()/100;
int accZ=getAccZ()/100;

LCD_setPage(3);
LCD_setColumn(0);
rprintfStr("X: ");rprintfInt(accX);rprintfStr("   ");
LCD_setPage(4);
LCD_setColumn(0);
rprintfStr("Y: ");rprintfInt(accY);rprintfStr("   ");
LCD_setPage(5);
LCD_setColumn(0);
rprintfStr("Z: ");rprintfInt(accZ);rprintfStr("   ");
//Acc test - End


pixelSetOrClearOrInvert(lcd, SET_PIXELS);
		//drawRect(lcd, 122, 0, 5, 63);
		drawRect(lcd, 0, 58, 121, 5);
		drawRect(lcd, 0, 58-40, 121, 5);
		fillRect(lcd, slider_down, 58, 4, 5);
		fillRect(lcd, 0, 58-40, 4+slider_up*8, 5);
pushRAM(lcd);

}


}



void controll_motors(LCD_engine *lcd, TouchCalibration *TC) {
LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();


	ButtonsMeny BMA;
	BMA.title = "Screen";
	BMA.RootButton = 0;
	MenyButton b[6];
	b[0] = makeButton("Forward", 0);
	b[1] = makeButton("Still", 1);
	b[2] = makeButton("Back", 2);
	b[3] = makeButton("Forward", 3);
	b[4] = makeButton("Still", 4);
	b[5] = makeButton("Back", 5);
	addMenyButton(&BMA, &b[5]);
	addMenyButton(&BMA, &b[2]);
	addMenyButton(&BMA, &b[4]);
	addMenyButton(&BMA, &b[1]);
	addMenyButton(&BMA, &b[3]);
	addMenyButton(&BMA, &b[0]);
	int valde = 0;
	char L_Motor=0;
	char R_Motor=0;
	while (1) {
		valde = Buttons_meny(lcd, TC, &BMA, 3);

		if (valde<=2){
		b[0].text="Forward";
		b[1].text="Still";
		b[2].text="Back";
		}else {
		b[3].text="Forward";
		b[4].text="Still";
		b[5].text="Back";
		}

		if (valde == 0) {//Left Forward
			//sendZigByte(0x00);
			b[0].text="*Forward*";
			L_Motor=1;

		}else if (valde == 1) {
			//sendZigByte(0xFF);
			b[1].text="*Still*";
			L_Motor=0;
		}else if (valde == 2) {
			//sendZigByte(0xFF);
			b[2].text="*Back*";
			L_Motor=3;
		}else if (valde == 3) {//R Forward
			//sendZigByte(0x00);
			b[3].text="*Forward*";
			R_Motor=1;

		}else if (valde == 4) {
			//sendZigByte(0xFF);
			b[4].text="*Still*";
			R_Motor=0;
		}else if (valde == 5) {
			//sendZigByte(0xFF);
			b[5].text="*Back*";
			R_Motor=2;
		}
		if (valde<0)
			break;

		sendZigByte(ID_DRIVE,(R_Motor)<<4 | (L_Motor));
		
	}


}



void motors_buttons(LCD_engine *lcd, TouchCalibration *TC){

LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);


drawCircle(lcd,62,40,20);
fillRect(lcd,34,35,6,10);
fillRect(lcd,84,35,6,10);

char L_Motor=0;
char R_Motor=0;
char last_send=0x00;
char last_send_id=0x00;
char useAcc=0;
unsigned int sends=0;
int CalAccX=0;
int CalAccY=0;
char suckOn=0;
Point LastPress;
LastPress.x=-1;
LastPress.y=-1;
unsigned char Udistance=230;
unsigned char o_Udistance=Udistance;
unsigned char updateLeds=0;
unsigned char timer=0;
while (1){
Udistance=get_latest_from_zigge_byte();
pixelSetOrClearOrInvert(lcd, SET_PIXELS);
if (START_isPressedAndRel()){check_debug(lcd,TC);if (in_meny(lcd,TC)){break;}}
if (B_isPressedAndRel()){
if (useAcc){
	useAcc=0;
	}else {
	
	useAcc=1;
	//Kalibrerar
	CalAccX=getAccX()/100;
	CalAccY=getAccY()/100;
	}
}

//timer++;
if (timer>100){
last_send_id=-1;
updateLeds=-1;
timer=0;
}
if (DOWN_isPressed()){
L_Motor=2;
R_Motor=2;
}else if (UP_isPressed()){
L_Motor=1;
R_Motor=1;
}else if (RIGHT_isPressed()){
L_Motor=1;
R_Motor=2;
}else if (LEFT_isPressed()){
L_Motor=2;
R_Motor=1;
}else if (A_isPressed()){
L_Motor=0;
R_Motor=0;
}


if (TOUCH_STATE == TOUCH_STATE_PRESSED) {
			Point p = getTouch(TC);
			//if (p.down_count > 2) {//Pressing
				LastPress=p;	
			//}
}else if (LastPress.x>0){
//tryckt och släppt

if (LastPress.y>35){

if (suckOn){
suckOn=0;
}else {
suckOn=1;
}
sends++;
sendZigByte(ID_VACUUM,suckOn);


}else {
if (useAcc){
	useAcc=0;
	}else {
	
	useAcc=1;
	//Kalibrerar
	CalAccX=getAccX()/100;
	CalAccY=getAccY()/100;
	}
}
LastPress.x=-1;
LastPress.y=-1;
}

if (useAcc){

		int accX=(getAccX()/100)-CalAccX;
		int accY=(getAccY()/100)-CalAccY;
		
		
		//LCD_setPage(2);
		//LCD_setColumn(0);
		//rprintfInt((get_latest_from_zigge_id()<<8)|get_latest_from_zigge_byte());rprintfStr("   ");
			//Svänga
			if (accX>0){
			if (accX>20){
				L_Motor=1;
				R_Motor=2;
			}	
			}else {
			accX=accX*-1;
			if (accX>20){
				L_Motor=2;
				R_Motor=1;
			}
			}
			//Framåt
			if (accY<0){
			accY=accY*-1;
			if (accY>20){
				L_Motor=1;
				R_Motor=1;
			}	
			}else {
			
			if (accY>20){
				L_Motor=2;
				R_Motor=2;
			}
			}
		

}


		LCD_setPage(0);
		LCD_setColumn(10);
		rprintfStr("L ");rprintfInt(L_Motor);rprintfStr("  ");
		LCD_setPage(0);
		LCD_setColumn(80);
		rprintfStr("R ");rprintfInt(R_Motor);rprintfStr("  ");
		LCD_setPage(1);
		LCD_setColumn(50);
		if (useAcc){
			rprintfStr("AcOn ");
			}else {
			rprintfStr("AcOff");
		}

		LCD_setPage(4);
		LCD_setColumn(50);
		if (!suckOn){
			rprintfStr("sOn ");
			}else {
			rprintfStr("sOff");
		}
		//LCD_setPage(1);
		//LCD_setColumn(50);
		//rprintfStr("B: ");rprintfBin(last_send);rprintfStr("         ");

		LCD_setPage(7);
		LCD_setColumn(0);
		rprintfInt(sends);rprintfStr(".");



	//Ultraljud
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
//LCD_drawLine(lcd,2+ball.o_x+o_Udistance*cosd(robot.dir-20)/300,3+ball.o_y+o_Udistance*sind(robot.dir-20)/300,2+ball.o_x+o_Udistance*cosd(robot.dir+20)/300,3+ball.o_y+o_Udistance*sind(robot.dir+20)/300);
printfStr(lcd,"-",120-2,60-3-(30*o_Udistance)/25);
o_Udistance=Udistance/10;
pixelSetOrClearOrInvert(lcd, SET_PIXELS);
LCD_drawLine(lcd,120,30,120,60);
printfStr(lcd,"-",120-2,60-3-(30*o_Udistance)/25);
printfStr(lcd,"x",120-2,60-3-(30*10)/25);
if (o_Udistance<10){
pixelSetOrClearOrInvert(lcd, SET_PIXELS);

}else {

pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
}
printfStr(lcd,"W",110-2,25);
printfStr(lcd,"A",110-2,33);
printfStr(lcd,"L",110-2,41);
printfStr(lcd,"L",110-2,49);


pixelSetOrClearOrInvert(lcd, SET_PIXELS);
if (last_send!=((R_Motor)<<4 | (L_Motor)) || last_send_id!=ID_DRIVE){
sends++;
sendZigByte(ID_DRIVE,(R_Motor)<<4 | (L_Motor));
last_send=(R_Motor)<<4 | (L_Motor);
last_send_id=ID_DRIVE;

fillRect(lcd,34,35,7,10);
fillRect(lcd,84,35,7,10);
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
if (L_Motor==2){
pixelSetOrClearOrInvert(lcd, SET_PIXELS);
LCD_drawLine(lcd,0+35,0+32+16,2+35,2+32+16);
LCD_drawLine(lcd,2+35,2+32+16,4+35,0+32+16);
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
fillRect(lcd,36,36,3,4);
LCD_drawLine(lcd,0+35,0+32,2+35,-2+32);
LCD_drawLine(lcd,2+35,-2+32,4+35,0+32);
}else if (L_Motor==1){
pixelSetOrClearOrInvert(lcd, SET_PIXELS);
LCD_drawLine(lcd,0+35,0+32,2+35,-2+32);
LCD_drawLine(lcd,2+35,-2+32,4+35,0+32);
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
fillRect(lcd,36,36+4,3,4);
LCD_drawLine(lcd,0+35,0+32+16,2+35,2+32+16);
LCD_drawLine(lcd,2+35,2+32+16,4+35,0+32+16);
}

if (R_Motor==2){
pixelSetOrClearOrInvert(lcd, SET_PIXELS);
LCD_drawLine(lcd,0+85,0+32+16,2+85,2+32+16);
LCD_drawLine(lcd,2+85,2+32+16,4+85,0+32+16);
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
fillRect(lcd,85+1,36,3,4);
LCD_drawLine(lcd,0+85,0+32,2+85,-2+32);
LCD_drawLine(lcd,2+85,-2+32,4+85,0+32);
}else if (R_Motor==1){
pixelSetOrClearOrInvert(lcd, SET_PIXELS);
LCD_drawLine(lcd,0+85,0+32,2+85,-2+32);
LCD_drawLine(lcd,2+85,-2+32,4+85,0+32);
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
fillRect(lcd,85+1,36+4,3,4);
LCD_drawLine(lcd,0+85,0+32+16,2+85,2+32+16);
LCD_drawLine(lcd,2+85,2+32+16,4+85,0+32+16);
}
//fillRect(lcd,84,35,6,10);




}
pushRAM(lcd);


if (o_Udistance<10){
if (updateLeds!=ANI_WALL){
updateLeds=ANI_WALL;

	sendZigByte(ID_LEDS,ANI_WALL);
}
}else {


if (R_Motor==1 && L_Motor==1 && updateLeds!=ANI_FORWARD){
		//Start message on leds
	sendZigByte(ID_LEDS,ANI_FORWARD);
updateLeds=ANI_FORWARD;
}else if (R_Motor==2 && L_Motor==2 && updateLeds!=ANI_BACKWARD){
		//Start message on leds
	sendZigByte(ID_LEDS,ANI_BACKWARD);
updateLeds=ANI_BACKWARD;
}else if (R_Motor==2 && L_Motor==1 && updateLeds!=ANI_RIGHT){
		//Start message on leds
	sendZigByte(ID_LEDS,ANI_RIGHT);
updateLeds=ANI_RIGHT;
}else if (R_Motor==1 && L_Motor==2 && updateLeds!=ANI_LEFT){
		//Start message on leds
	sendZigByte(ID_LEDS,ANI_LEFT);
updateLeds=ANI_LEFT;
}

}

}

}

//Create and launch a new meny
void gui_leds_meny(LCD_engine *lcd, TouchCalibration *TC) {
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	ButtonsMeny BMA;
	BMA.title = "Dustwars";
	BMA.RootButton = 0;
	MenyButton b[4];
	//b[0] = makeButton("All on", 1);
	b[0] = makeButton("Buttons", 1);
	
	b[1] = makeButton("Missiles", 2);
	b[2] = makeButton("Controll", 3);
	b[3] = makeButton("Ball", 4);

	addMenyButton(&BMA, &b[3]);
	addMenyButton(&BMA, &b[2]);
	addMenyButton(&BMA, &b[1]);
	addMenyButton(&BMA, &b[0]);
	int valde = 0;
	while (1) {
		valde = Buttons_meny(lcd, TC, &BMA, 2);
		if (valde == 1) {
			//sendZigByte(0x00);
			motors_buttons(lcd,TC);

		}else if (valde == 2) {
			//sendZigByte(ID_DRIVE,0xFF);

			game_missile(lcd ,TC);

		}else if (valde==3){
			controll_motors(lcd,TC);
		} else if (valde==4){
			ball_game_start(lcd, TC);
		}else {
			gui_screen_meny(lcd,TC);
			//break;
		}
	}
}


void test_acc(LCD_engine *lcd){
LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	char a=0;
	while (1){
	a++;
		LCD_setPage(0);
		LCD_setColumn(10);
		rprintfStr("Hello acc! ");rprintfInt(a);
		LCD_setPage(3);
		LCD_setColumn(10);
		if (a==0){
		rprintfInt(getAccX());
		resetConnection();rprintfStr("       ");	
		}
	}
}

void drawStartImage(LCD_engine *lcd){
LCD_CLR();
LCD_RAM_CLR(lcd);
resetConnection();
setDubbelbuffer(lcd,1);
pixelSetOrClearOrInvert(lcd,SET_PIXELS);
resetConnection();


LCD_drawImageToRam(lcd,Darth_im);
pushRAM(lcd);

while (1){if (START_isPressedAndRel() || A_isPressedAndRel()){break;}};

}

//Main entry point
int main(void) {

	zigbee_init();
	init_acc();

	LCD_engine *lcd = init_lcd(); //Get a LCD buffer and init LCD
	button_init();

	//test_acc(lcd);

	//test_buttons(lcd);
	//The calibration
	TouchCalibration TC;
	//Init the sys_time

	TCCR0A&=~((1<<COM0A1) | (1<<COM0A0) | (1<<COM0B1) | (1<<COM0B0));
	TCCR0B = 0b00000011; //64 prescaler
	TIMSK0 = 0x01;
	sei(); //Init interrupts


	touch_init_advance(&TC);//Init the interrupt handeled touch

	//PCMSK2=0x00;

	unsigned char starts = eeprom_read(10);
	if (starts == 255) {//just flashed...
		set_calibration(&TC, 37, 164, 47, 86); //Sets a pretty good calibration
		calibrate_touch(lcd, &TC);
	}
	if (starts != 254) {
		eeprom_write(10, starts + 1);
	}


	//calibrate_touch(lcd, &TC);

	


	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	LCD_setPage(0);
	LCD_setColumn(10);
	rprintfStr("Hello world, again!");

	//Start message on leds
	sendZigByte(ID_LEDS,ANI_HELLO);

	//test_buttons(lcd);
//test_touch(lcd);
DisplaySetDispInverted(lcd, 1);
drawStartImage(lcd);
DisplaySetDispInverted(lcd, 0);
gui_screen_meny(lcd,&TC);


while (1) {
gui_leds_meny(lcd,&TC);
	
	}

LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	LCD_setPage(0);
	LCD_setColumn(10);
	rprintfStr("Hello world, again!");
	//Just in case...
	while (1) {

	
	}

}


