#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include "lcd_engine.h"
#include <util/delay.h>
#include "buttons.h"
#define SIDE_LEFT 1
#define SIDE_RIGHT 2
#define SIDE_BOTH 0


void draw_board(LCD_engine *lcd);


typedef struct Ball{
double x;
double y;
double dx;
double dy;
int old_x;
int old_y;

}Ball;

typedef struct Pad{
int x;
double y;
double dy;
int old_y;
int size;
char side;
}Pad;

typedef struct Player{
	char score;
	Pad pad;
	Pad pad2;
	}Player;



void draw_ball(LCD_engine *lcd,int x,int y){
	LCD_drawPixel(lcd,x,y-2);
	LCD_drawPixel(lcd,x,y-1);
	LCD_drawPixel(lcd,x,y);
	LCD_drawPixel(lcd,x,y+1);
	LCD_drawPixel(lcd,x,y+2);
	
	LCD_drawPixel(lcd,x-1,y-1);
	LCD_drawPixel(lcd,x-1,y);
	LCD_drawPixel(lcd,x-2,y);
	LCD_drawPixel(lcd,x-1,y+1);
	
	LCD_drawPixel(lcd,x+1,y-1);
	LCD_drawPixel(lcd,x+1,y);
	LCD_drawPixel(lcd,x+2,y);
	LCD_drawPixel(lcd,x+1,y+1);


}

char handle_ball_hit_pad(Pad *pad,Ball *ball){
	
	if (  ( (pad->side == SIDE_RIGHT && ball->dx<0 && ((int)ball->x)==pad->x+1 ) || (pad->side == SIDE_LEFT && ball->dx>0 && ((int)ball->x)==pad->x-1 ) ) && ((int)ball->y)>=pad->old_y && ((int)ball->y)<=pad->old_y+pad->size){
		ball->dx=-ball->dx;
		//Smart way to change dy depending on where the ball hit the pad
		int offset=(pad->y + (pad->size)/2 - ball->y);
		ball->dy+= -0.1 * offset;
		
		
		return 1;
		}
	return 0;
	}

void draw_pad(LCD_engine *lcd, Pad *pad){
	LCD_drawLine(lcd,pad->x,(int)pad->y,pad->x,(int)pad->y+pad->size);
}

void erase_pad(LCD_engine *lcd, Pad *pad){
	LCD_drawLine(lcd,pad->x,pad->old_y,pad->x,pad->old_y+pad->size);
}

void lost_ball_animation(LCD_engine *lcd, int x,int y){
	pixelSetOrClearOrInvert(lcd,INVERT_PIXELS);
	for (int i = 0; i < 30; i++)
	{
		drawCircle(lcd,x, y, i);
		
		pushRAM(lcd);
		_delay_ms(50);
	}
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	DOGENABLE;
	draw_board(lcd);
	pushRAM(lcd);
	pixelSetOrClearOrInvert(lcd,SET_PIXELS);
	}
void draw_board(LCD_engine *lcd){
	LCD_drawLine(lcd,0,8,127,8);
	LCD_drawLine(lcd,0,63,127,63);
	}
int game_ball(LCD_engine *lcd){
	
	//Variabler
	Ball ball;
	Player player_1;
	Player player_2;
	//Pad pad1;
	//Pad pad2;
	//char counter=0;
	
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	DOGENABLE;
	LCD_setPage(0);
	LCD_setColumn(10);
	rprintfStr("Hello Pong ENGINE2!!!! :D öö");
	
	_delay_ms(4000);
	LCD_setPage(0);
	LCD_setColumn(10);
	rprintfStr("                                  ");
	setDubbelbuffer(lcd,1);
	
	DOGENABLE;
	_delay_ms(10);
	//draw board
	draw_board(lcd);
	
	
	ball.x=20;
	ball.y=30;
	ball.old_x=20;
	ball.old_y=30;
	ball.dx=1;
	ball.dy=1;
	
	////Player one, pad on left side
	//pad1.x=2;
	//pad1.y=30;
	//pad1.dy=0.1;
	//pad1.old_y=30;
	//pad1.size=10;
	//pad1.side=SIDE_RIGHT;
	//
	////Player one, pad in middle
	//pad2.x=50;
	//pad2.y=30;
	//pad2.dy=0.1;
	//pad2.old_y=30;
	//pad2.size=10;
	//pad2.side=SIDE_LEFT;
	//player_1.pad=&pad1;
	player_1.score=0;
	player_1.pad.x=2;
	player_1.pad.y=30;
	player_1.pad.dy=0.5;
	player_1.pad.old_y=30;
	player_1.pad.size=10;
	player_1.pad.side=SIDE_RIGHT;
	
	player_1.pad2.x=96;
	player_1.pad2.y=30;
	player_1.pad2.dy=0.5;
	player_1.pad2.old_y=30;
	player_1.pad2.size=10;
	player_1.pad2.side=SIDE_RIGHT;
	
	
	//Player 2 on the rigth side
	player_2.score=0;
	player_2.pad.x=126;
	player_2.pad.y=30;
	player_2.pad.dy=0.5;
	player_2.pad.old_y=30;
	player_2.pad.size=10;
	player_2.pad.side=SIDE_LEFT;
	
	player_2.pad2.x=32;
	player_2.pad2.y=30;
	player_2.pad2.dy=0.5;
	player_2.pad2.old_y=30;
	player_2.pad2.size=10;
	player_2.pad2.side=SIDE_LEFT;
	
	
	


	//int lifes=4;
	char speed=0;
	while (1){//game loop

	
	
	if (START_isPressedAndRel()){
		break;
		LCD_CLR();
		//LCD_RAM_CLR(lcd);
		DOGENABLE;
		LCD_setPage(0);
		LCD_setColumn(0);
		rprintfStr("Pressed START");
		LCD_engine * newLCD=get_new_ram();
		LCD_RAM_CLR(newLCD);
		draw_ball(newLCD,20,20);
		pushRAM(newLCD);
		while (1){
			if (A_isPressed()){
				break;
			}
			}
			trash_ram(newLCD);
		LCD_CLR();
		//LCD_RAM_CLR(lcd);
		DOGENABLE;
		redrawRAM(lcd);

	}

	
	
	LCD_setPage(0);
	LCD_setColumn(0);
	rprintfStr("Score:");rprintfInt(player_1.score);
	
	LCD_setColumn(60);rprintfStr("ö ");
	
	//LCD_setColumn(100);
	rprintfStr("Score:");rprintfInt(player_2.score);
	//rprintfStr("L:");for(int i=0;i<lifes;i++){rprintfStr("ö");}rprintfStr("  ");
	//rprintfStr("s:");rprintfInt(speed);rprintfStr(" ");
	//rprintfStr("p:");rprintfInt((int)((pad1.dy)*100));rprintfStr(" ");
	//see keys
	//if (R_BUTTONS&(1<<B_LEFT)){
	if (LEFT_isPressed()){
	speed+=1;
	if (speed>50)
		speed=0;
	}
	//if (R_BUTTONS&(1<<B_RIGHT)){
	if (RIGHT_isPressed()){
	//pad1.dy+=0.01;
	
	//if (pad1.dy>1)
	//pad1.dy=0.01;
	}
	
	//Player 1
	//if (R_BUTTONS&(1<<B_B)){
	if (B_isPressed()){
	player_1.pad.y+=-player_1.pad.dy;
	if (player_1.pad.y<9)
		player_1.pad.y=9;
	}
	
	//if (R_BUTTONS&(1<<B_A)){
	if (A_isPressed()){
	player_1.pad.y+=player_1.pad.dy;
	if (player_1.pad.y+player_1.pad.size>62)
		player_1.pad.y=62-player_1.pad.size;
	}
	//Second pad
	player_1.pad2.y=player_1.pad.y;
	
	
	//Player 1
	//if (R_BUTTONS&(1<<B_DOWN)){
	if (UP_isPressed()){
	player_2.pad.y+=-player_2.pad.dy;
	if (player_2.pad.y<9)
		player_2.pad.y=9;
	}
	//if (R_BUTTONS&(1<<B_UP)){
	if (DOWN_isPressed()){
	player_2.pad.y+=player_2.pad.dy;
	if (player_2.pad.y+player_2.pad.size>62)
		player_2.pad.y=62-player_2.pad.size;
	}
	//Second pad
	player_2.pad2.y=player_2.pad.y;
	
	
	//Erase last frame
	pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
	draw_ball(lcd,ball.old_x,ball.old_y);
	erase_pad(lcd, &player_1.pad);
	erase_pad(lcd, &player_1.pad2);
	
	erase_pad(lcd, &player_2.pad);
	erase_pad(lcd, &player_2.pad2);
	
	//erase_pad(lcd, &pad2);
	//Draw next frame
	pixelSetOrClearOrInvert(lcd,SET_PIXELS);
	draw_pad(lcd, &player_1.pad);
	draw_pad(lcd, &player_1.pad2);
	
	draw_pad(lcd, &player_2.pad);
	draw_pad(lcd, &player_2.pad2);
	
	//draw_pad(lcd, &pad2);
	draw_ball(lcd,(int)ball.x,(int)ball.y);
	
	
	
	ball.old_x=(int)ball.x;
	ball.old_y=(int)ball.y;
	
	
	//Save player 1 state
	player_1.pad.old_y=(int)player_1.pad.y;
	player_1.pad2.old_y=player_1.pad.old_y;
	//Save player 2 state
	player_2.pad.old_y=(int)player_2.pad.y;
	player_2.pad2.old_y=player_2.pad.old_y;
	
	
	//pad2.old_y=(int)pad2.y;
	
	ball.x+=ball.dx;
	ball.y+=ball.dy;
	
	//Upper and lower boundarys
	if (((int)ball.y)>=61 || ((int)ball.y)<=10 ){
	ball.dy=-ball.dy;
	ball.y+=ball.dy;
	}
	
	
	if (handle_ball_hit_pad(&(player_1.pad),&ball)){//returns 1 on hit and 0 on no hit
	}
	handle_ball_hit_pad(&(player_1.pad2),&ball);
	
	
	handle_ball_hit_pad(&(player_2.pad),&ball);
	handle_ball_hit_pad(&(player_2.pad2),&ball);
	
	//if (handle_ball_hit_pad(&pad2,&ball)){//returns 1 on hit and 0 on no hit
	//}
	
	if (((int)ball.x)>=126){//Player 2 missed the ball
	lost_ball_animation(lcd,ball.x,ball.y);
		ball.x=65;
		ball.y=30;
		ball.dx=-ball.dx;
		ball.dy=0;
		//if (lifes<0)
		//lifes=4;
		player_1.score+=1;
	
	}
	
	if (((int)ball.x)<=1){//Player 1 missed the ball!
	//lifes--;
	lost_ball_animation(lcd,ball.x,ball.y);
	ball.x=65;
	ball.dx=-ball.dx;
	ball.dy=0;
	ball.y=30;
	//if (lifes<0)
	//lifes=4;
	player_2.score+=1;
	
	
	}
	
	
	
	pushRAM(lcd);
	_delay_ms(1);
	DOGENABLE;
	}
	return 1;

}
