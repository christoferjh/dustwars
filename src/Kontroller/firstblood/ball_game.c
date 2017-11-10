#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include "lcd_engine.h"
#include "touch.h"
#include <util/delay.h>
#include "buttons.h"
#include "ball_game.h"
#include "gui.h"
#include <math.h> 
#include "system.h"
#include "zigbeeny.h"
#include "dustwarsIDs.h"
#include "accelerometer.h"
#include <stdlib.h>
#include "killer.h"
#include "settings.h"
#include "debug.h"

#define MAX_S_X 90
#define MAX_S_Y 54
#define MIN_S_X 5
#define MIN_S_Y 10
#define MAX_X 100
#define MAX_Y 63
#define MIN_X 0
#define MIN_Y 9
#define SCORE_DOWN_TIME 1000



typedef struct Ball {
	//Used for deleting last drawing and some calculation so no need to take times 1/100 
	int o_x;
	int o_y;
	//this is multiplied by 100 to get a decimal
	int x;
	int y;
	//speed
	int dx;
	int dy;

} Ball;


typedef struct Food {
	//Used for deleting last drawing and some calculation so no need to take times 1/100 
	char x;
	char y;
	unsigned char timer;
	char respawn;
} Food;

typedef struct Wall {
	//Used for deleting last drawing and some calculation so no need to take times 1/100 
	char x;
	char y;
} Wall;

typedef struct Robot {
	//Used for deleting last drawing and some calculation so no need to take times 1/100 
int dir;
int o_dir;
	//this is multiplied by 100 to get a decimal
	int x;
	int y;

} Robot;

//Calculates the time diff from to one time to another, taking owerflow in the calculation
unsigned int time_diffa(unsigned int from, unsigned int to) {
	if ((to - from) < 0) {

		return to - from + 65536;
	}

	return to - from;
}

int distance(int x1,int y1,int x2,int y2){
return (int) sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
}
int distance2(int x1,int y1,int x2,int y2){
return (int) ((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
}

int cosd(int degree){
double rad=(double)degree*0.01745278;
return (int)100*cos(rad);
}

int sind(int degree){
double rad=(double)degree*0.01745278;
return (int)100*sin(rad);
}

char getRandom(){
return (getAccX()&3)|((getAccX()&3)<<2)|((getAccX()&3)<<4);
}



void drawShip(LCD_engine *lcd,int dir,int x,int y){
dir=((dir)/10)*10;

//LCD_drawLine(lcd,x+cosd(dir+120)/18,y+sind(dir+120)/18,x+cosd(dir)/10,y+sind(dir)/10);
//LCD_drawLine(lcd,x+cosd(dir-120)/18,y+sind(dir-120)/18,x+cosd(dir)/10,y+sind(dir)/10);
//LCD_drawLine(lcd,x+cosd(dir-120)/18,y+sind(dir-120)/18,x+cosd(dir+120)/18,y+sind(dir+120)/18);

//LCD_drawLine(lcd,x,y,x+cosd(dir)/10,y+sind(dir)/10);
//LCD_drawLine(lcd,x,y,x+cosd(dir)/10,y+sind(dir)/10);
}

int ball_game_start(LCD_engine *lcd, TouchCalibration *TC) {
//Clear screen
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	DOGENABLE;
	setDubbelbuffer(lcd, 1); //Dubbelbuffring is on
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	DOGENABLE;
unsigned int game_ticks = 0;
int MAX=6;
Ball ball;
ball.x=500;
ball.y=500;
ball.o_x=300;
ball.o_y=500;
ball.dx=0;
ball.dy=0;
Robot robot;
robot.dir=0;
robot.x=20;
robot.y=20;
robot.o_dir=0;

Food food;
food.x=30;
food.y=20;
food.timer=0;
food.respawn=0;

int CalAccX=getAccX();
int	CalAccY=getAccY();
char L_Motor=0;
	char R_Motor=0;
	char last_send=0x00;
	char last_send_id=0x00;
	unsigned int sends=0;
	int score=0;
unsigned char Udistance=230;
unsigned char o_Udistance=Udistance;
unsigned char updateLeds=0;
srand((unsigned char)getRandom());

unsigned char bonus=20;

drawCircle(lcd,117,10,10);
LCD_drawLine(lcd,MAX_X,MIN_Y,MAX_X,0);
LCD_drawLine(lcd,0,MIN_Y,0,0);
int scoreDOWN=SCORE_DOWN_TIME;
Point LastPress;
LastPress.x=-1;
LastPress.y=-1;
char suckOn=0;
sendZigByte(ID_VACUUM,suckOn);

while (1){

if (START_isPressedAndRel()){check_debug(lcd,TC);if (in_meny(lcd,TC)){break;}last_send=0x00;}

if (UP_isPressed() && LEFT_isPressed()){
if (A_isPressedAndRel()){
if (DOWN_isPressed() && RIGHT_isPressed()){
start_killer(lcd,TC);
}
}
}

if (A_isPressedAndRel()){
CalAccX=getAccX();
CalAccY=getAccY();
}
game_ticks = sys_time; //current time



if (TOUCH_STATE == TOUCH_STATE_PRESSED) {
			Point p = getTouch(TC);
			//if (p.down_count > 2) {//Pressing
				LastPress=p;	
			//}
}else if (LastPress.x>0){
//tryckt och sl�ppt

if (suckOn){
suckOn=0;
}else {
suckOn=1;
}
sends++;
sendZigByte(ID_VACUUM,suckOn);
LastPress.x=-1;
LastPress.y=-1;
}


//OBS
Udistance=get_latest_from_zigge_byte();
//Udistance=255;
/*
//DEBUGING
if (UP_isPressed()){
Udistance++;
}
if (DOWN_isPressed()){
Udistance--;
}
if (LEFT_isPressedAndRel()){
Udistance=0;
}
if (RIGHT_isPressedAndRel()){
Udistance=255;
}*/
//inputs
int accX=(getAccX()-CalAccX)/100;
int accY=(getAccY()-CalAccY)/100;

if ((abs(accX)>5 || abs(accY)>5)){

int R=distance(accX,accY,0,0);
accY = MAX * (accY) / R;
	accX = MAX * (accX) / R;
	//kolla vilket håll att snurra runt
//vänster
int RB=distance2(accX,accY,(MAX*cosd(robot.dir))/100,(MAX*sind(robot.dir))/100);
int RL=distance2(accX,accY,(MAX*cosd(robot.dir+80))/100,(MAX*sind(robot.dir+80))/100);
int RR=distance2(accX,accY,(MAX*cosd(robot.dir-80))/100,(MAX*sind(robot.dir-80))/100);
LCD_setPage(0);
LCD_setColumn(2);
if (RB>3){
if (RL>RR){
rprintfStr("RL");
robot.dir+=-2;
//Rotate Right
R_Motor=1;
L_Motor=2;
//robot.dir+=-10;
}else {
rprintfStr("RR");
robot.dir+=2;
R_Motor=2;
L_Motor=1;
//Rotate Left
//robot.dir+=10;
}
ball.dx=0;
ball.dy=0;
}else if (RB<=3 && RB>2){
if (RL>RR){
//rprintfStr("RR");
robot.dir+=-1;

}else {
//rprintfStr("RL");
robot.dir+=1;

}
}else {
rprintfStr("RN");
ball.dx=accX;
ball.dy=accY;
R_Motor=1;
L_Motor=1;
}


}else {
R_Motor=0;
L_Motor=0;
ball.dx=0;
ball.dy=0;
}


if ((abs(ball.o_x-food.x)<4 && abs(ball.o_y-food.y)<4) || food.respawn){
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);

printfStr(lcd,"o",food.x,food.y);
printfStr(lcd,"x",food.x,food.y);

food.x=rand()%(MAX_S_X - MIN_S_X)+MIN_S_X;
food.y=rand()%(MAX_S_Y - MIN_S_Y)+MIN_S_Y;
food.timer=0;

if (food.respawn==2){
bonus=0;

}else{
score++;
score+=bonus;
bonus=20;

}
food.respawn=0;

}


food.timer++;
if (food.timer>20){
food.timer=0;

pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);

printfStr(lcd,"o",food.x,food.y);
printfStr(lcd,"x",food.x,food.y);
char respawn=0;
if (bonus>0){
bonus-=2;
}
if (food.x>60){
food.x--;
}else if(food.x<60){
food.x++;
}else {
respawn=1;
}
if (food.y>30){
food.y--;
}else if (food.y<30) {
food.y++;
}else if (respawn){
food.respawn=1;
}

}

scoreDOWN--;
if (scoreDOWN<0){
if (score>0)
	score--;
scoreDOWN=SCORE_DOWN_TIME;
}


LCD_setPage(0);
LCD_setColumn(20);
rprintfStr("Score:");rprintfInt(score);rprintfStr("+");rprintfInt(bonus+1);rprintfStr(" ");


pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
printfStr(lcd,"o",ball.o_x+o_Udistance*cosd(robot.o_dir)/200,ball.o_y+o_Udistance*sind(robot.o_dir)/200);
printfStr(lcd,"-",120-2,60-3-(30*o_Udistance)/25);



LCD_drawLine(lcd,117,10,117+cosd((robot.o_dir/20)*20)/11,10+sind((robot.o_dir/20)*20)/11);
printfStr(lcd,"*",ball.o_x,ball.o_y);
drawShip(lcd,robot.o_dir,ball.o_x,ball.o_y);
pixelSetOrClearOrInvert(lcd, SET_PIXELS);
drawRect(lcd,MIN_X,MIN_Y,MAX_X-MIN_X,MAX_Y-MIN_Y);

if (ball.y/10+7>MAX_Y){
ball.y+=-100;
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);

printfStr(lcd,"o",food.x,food.y);
printfStr(lcd,"x",food.x,food.y);
food.y+=-10;

}
if (ball.y/10<MIN_Y){
ball.y+=100;
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);

printfStr(lcd,"o",food.x,food.y);
printfStr(lcd,"x",food.x,food.y);
food.y+=+10;
}
if (ball.x/10+5>MAX_X){
ball.x+=-100;
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);

printfStr(lcd,"o",food.x,food.y);
printfStr(lcd,"x",food.x,food.y);
food.x+=-10;
}
if (ball.x/10<MIN_X){
ball.x+=100;
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);

printfStr(lcd,"o",food.x,food.y);
printfStr(lcd,"x",food.x,food.y);
food.x+=10;
}

if (food.x>MAX_S_X || food.x<MIN_S_X || food.y>MAX_S_Y || food.y<MIN_S_Y){

food.respawn=2;
}

ball.o_y=ball.y/10;
ball.o_x=ball.x/10;

pixelSetOrClearOrInvert(lcd, SET_PIXELS);

if (suckOn){
printfStr(lcd,"s",117-2,10-4);
}else {
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
printfStr(lcd,"s",117-2,10-4);
}
pixelSetOrClearOrInvert(lcd, SET_PIXELS);
printfStr(lcd,"*",ball.o_x,ball.o_y);
drawShip(lcd,robot.dir,ball.o_x,ball.o_y);
LCD_drawLine(lcd,117,10,117+cosd((robot.dir/20)*20)/11,10+sind((robot.dir/20)*20)/11);
//wall
o_Udistance=Udistance/10;
if (Udistance<250){

printfStr(lcd,"o",ball.o_x+o_Udistance*cosd(robot.dir)/200,ball.o_y+o_Udistance*sind(robot.dir)/200);
	}
	//Ultraljud
LCD_drawLine(lcd,120,30,120,60);
printfStr(lcd,"-",120-2,60-3-(30*o_Udistance)/25);
printfStr(lcd,"x",120-2,60-3-(30*10)/25);
if (o_Udistance<10){
pixelSetOrClearOrInvert(lcd, SET_PIXELS);
if (score>0)
	score--;
}else {
pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
}
printfStr(lcd,"W",110-2,25);
printfStr(lcd,"A",110-2,33);
printfStr(lcd,"L",110-2,41);
printfStr(lcd,"L",110-2,49);



pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	

if (!food.respawn){
printfStr(lcd,"o",food.x,food.y);
printfStr(lcd,"x",food.x,food.y);
}
robot.o_dir=robot.dir;


if (robot.dir>360)
robot.dir=robot.dir-360;

if (robot.dir<0)
robot.dir=360+robot.dir;
ball.x+=ball.dx;
ball.y+=ball.dy;

pushRAM(lcd);
//ROBOT
if (last_send!=((R_Motor)<<4 | (L_Motor)) || last_send_id!=ID_DRIVE){
sends++;
sendZigByte(ID_DRIVE,(R_Motor)<<4 | (L_Motor));
last_send=(R_Motor)<<4 | (L_Motor);
last_send_id=ID_DRIVE;

}

//LEDS

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



		int diff = time_diffa(game_ticks, sys_time);
		while (diff < 25) {
			diff = time_diffa(game_ticks, sys_time);
			LCD_setColumn(10); //Time consuming, wierd erros without, cannot in the world tell you why
		}

}

return 0;
}



