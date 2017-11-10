#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include "lcd_engine.h"
#include "touch.h"
#include <util/delay.h>
#include "buttons.h"
#include "gui.h"
//Defining for handeling ball against Pads
#define SIDE_LEFT 1
#define SIDE_RIGHT 2
#define SIDE_BOTH 0

//Declare function
void draw_board(LCD_engine *lcd);

//This is the ball
//These doubles do not need to be doubles but can use as well be ints, just multi with 100 to get the desired decimal representation
//but it is used here to demonstrate the ability to use doubles (float values). Missile commander uses the more faster way with ints.
typedef struct Ball {
	double x;
	double y;
	double dx;
	double dy;
	//Used to erase last drawing
	int old_x;
	int old_y;

} Ball;
//This is the definition of a player Pad (or AI if implemented)
typedef struct Pad {
	int x;
	double y;
	double dy;
	int old_y;//For erase of old drawing
	int size; //How big the line is
	char side; //Which side the ball bounches of
} Pad;

//This is a player
typedef struct Player {
	char score;
	//Each player handlers two Pads to make the game more interesting 
	Pad pad;
	Pad pad2;
} Player;

//Draw a ball (or erase), its pretty fast, much faster than using fill_circle 
void draw_ball(LCD_engine *lcd, int x, int y) {
	LCD_drawPixel(lcd, x, y - 2);
	LCD_drawPixel(lcd, x, y - 1);
	LCD_drawPixel(lcd, x, y);
	LCD_drawPixel(lcd, x, y + 1);
	LCD_drawPixel(lcd, x, y + 2);

	LCD_drawPixel(lcd, x - 1, y - 1);
	LCD_drawPixel(lcd, x - 1, y);
	LCD_drawPixel(lcd, x - 2, y);
	LCD_drawPixel(lcd, x - 1, y + 1);

	LCD_drawPixel(lcd, x + 1, y - 1);
	LCD_drawPixel(lcd, x + 1, y);
	LCD_drawPixel(lcd, x + 2, y);
	LCD_drawPixel(lcd, x + 1, y + 1);
}

char handle_ball_hit_pad(Pad *pad, Ball *ball) {//Check if ball hits Pad and bounch if its a bounchy side of the Pad

	if (((pad->side == SIDE_RIGHT && ball->dx < 0 && ((int) ball->x) == pad->x
			+ 1) || (pad->side == SIDE_LEFT && ball->dx > 0 && ((int) ball->x)
			== pad->x - 1)) && ((int) ball->y) >= pad->old_y && ((int) ball->y)
			<= pad->old_y + pad->size) {
		//Never increase this, this make the ball unable to jump over a Pad
		ball->dx = -ball->dx;
		//Smart way to change dy depending on where the ball hit the pad
		int offset = (pad->y + (pad->size) / 2 - ball->y);
		ball->dy += -0.1 * offset;

		return 1; //The ball bounched
	}
	return 0; //No action was made
}

//Draw the pad
void draw_pad(LCD_engine *lcd, Pad *pad) {
	LCD_drawLine(lcd, pad->x, (int) pad->y, pad->x, (int) pad->y + pad->size);
}

//Erase the pad
void erase_pad(LCD_engine *lcd, Pad *pad) {
	LCD_drawLine(lcd, pad->x, pad->old_y, pad->x, pad->old_y + pad->size);
}

//Animate the ball blowing up, pretty cool animation
void lost_ball_animation(LCD_engine *lcd, int x, int y) {
	pixelSetOrClearOrInvert(lcd, INVERT_PIXELS);
	for (int i = 0; i < 30; i++) {
		drawCircle(lcd, x, y, i);

		pushRAM(lcd);
		_delay_ms(50);
	}
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	DOGENABLE;
	draw_board(lcd);
	pushRAM(lcd);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
}
//Draw the board, nothing fancy
void draw_board(LCD_engine *lcd) {
	LCD_drawLine(lcd, 0, 8, 127, 8);
	LCD_drawLine(lcd, 0, 63, 127, 63);
}

//Check if user would like to quit game
int in_game_meny(LCD_engine *lcd, TouchCalibration *TC) {//return 1 if exit game
//This is really cool, it starts a new lcd buffer so the old one is intact. To use for ingame menys etc, for example a pause meny
	//never did any fancy with it like a rest score or invert second pad choice etc, no time now. But it works as a charm :)
	LCD_CLR();
	DOGENABLE;
	LCD_setPage(0);
	LCD_setColumn(0);
	rprintfStr("Pressed START");
	LCD_engine * newLCD = get_new_ram(); //The cool stuff, allocates a new buffer
	setDubbelbuffer(newLCD, 1);
	LCD_RAM_CLR(newLCD);
	draw_ball(newLCD, 20, 20);
	pushRAM(newLCD);

	ButtonsMeny BMA;
	BMA.title = "Paused";
	BMA.RootButton = 0;
	MenyButton b[2];
	b[0] = makeButton("QUIT", 1);
	b[1] = makeButton("Return", 0);
	addMenyButton(&BMA, &b[1]);
	addMenyButton(&BMA, &b[0]);
	int valde = Buttons_meny(newLCD, TC, &BMA, 3);
	trash_ram(newLCD); //Trashes the new buffer since we are done with it
	LCD_CLR();
	DOGENABLE;
	redrawRAM(lcd); //Then just like nothing have happend, the game countinues, the old buffer is redrawn.

	return valde;
}

//The entry point of the game
int game_ball(LCD_engine *lcd, TouchCalibration *TC) {

	Ball ball;
	Player player_1;
	Player player_2;

	Point touchpoint;

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
	setDubbelbuffer(lcd, 1);

	DOGENABLE;
	_delay_ms(10);
	draw_board(lcd);

	//Init ball
	ball.x = 20;
	ball.y = 30;
	ball.old_x = 20;
	ball.old_y = 30;
	ball.dx = 1;
	ball.dy = 1;

	////Player one, pad on left side
	player_1.score = 0;
	player_1.pad.x = 2;
	player_1.pad.y = 30;
	player_1.pad.dy = 0.5;
	player_1.pad.old_y = 30;
	player_1.pad.size = 10;
	player_1.pad.side = SIDE_RIGHT;

	player_1.pad2.x = 96;
	player_1.pad2.y = 30;
	player_1.pad2.dy = 0.5;
	player_1.pad2.old_y = 30;
	player_1.pad2.size = 10;
	player_1.pad2.side = SIDE_RIGHT;

	//Player 2 on the rigth side
	player_2.score = 0;
	player_2.pad.x = 126;
	player_2.pad.y = 30;
	player_2.pad.dy = 0.5;
	player_2.pad.old_y = 30;
	player_2.pad.size = 10;
	player_2.pad.side = SIDE_LEFT;

	player_2.pad2.x = 32;
	player_2.pad2.y = 30;
	player_2.pad2.dy = 0.5;
	player_2.pad2.old_y = 30;
	player_2.pad2.size = 10;
	player_2.pad2.side = SIDE_LEFT;

	char speed = 0;
	while (1) {//game loop


		if (START_isPressedAndRel()) {
			if (in_game_meny(lcd, TC))
				return 5; //Just exit


		}

		//Print score, bypas dubblebuffer by using raw print
		LCD_setPage(0);
		LCD_setColumn(0);
		rprintfStr("Score:");
		rprintfInt(player_1.score);
		LCD_setColumn(60);
		rprintfStr("ö ");//A hart
		rprintfStr("Score:");
		rprintfInt(player_2.score);
		if (LEFT_isPressed()) {
			speed += 1;
			if (speed > 50)
				speed = 0;
		}

		//Player 1
		if (UP_isPressed()) {
			player_1.pad.y += -player_1.pad.dy;
			if (player_1.pad.y < 9)
				player_1.pad.y = 9;
		}

		if (DOWN_isPressed()) {
			player_1.pad.y += player_1.pad.dy;
			if (player_1.pad.y + player_1.pad.size > 62)
				player_1.pad.y = 62 - player_1.pad.size;
		}
		//Second pad just folows the first, would be fun to invert
		player_1.pad2.y = player_1.pad.y;

		//Player 2, touch controlled also
		touchpoint = getTouch(TC); //Get the touch
		if (A_isPressed() || (TOUCH_STATE == TOUCH_STATE_PRESSED
				&& touchpoint.down_count > 3 && touchpoint.y < 32)) {
			player_2.pad.y += -player_2.pad.dy;
			if (player_2.pad.y < 9)
				player_2.pad.y = 9;
		}
		if (B_isPressed() || (TOUCH_STATE == TOUCH_STATE_PRESSED
				&& touchpoint.down_count > 3 && touchpoint.y > 32)) {
			player_2.pad.y += player_2.pad.dy;
			if (player_2.pad.y + player_2.pad.size > 62)
				player_2.pad.y = 62 - player_2.pad.size;
		}
		//Second pad just folows the first, would be fun to invert
		player_2.pad2.y = player_2.pad.y;

		//Erase last frame
		pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
		draw_ball(lcd, ball.old_x, ball.old_y);
		erase_pad(lcd, &player_1.pad);
		erase_pad(lcd, &player_1.pad2);

		erase_pad(lcd, &player_2.pad);
		erase_pad(lcd, &player_2.pad2);

		//Draw next frame
		pixelSetOrClearOrInvert(lcd, SET_PIXELS);
		draw_pad(lcd, &player_1.pad);
		draw_pad(lcd, &player_1.pad2);

		draw_pad(lcd, &player_2.pad);
		draw_pad(lcd, &player_2.pad2);

		//Draw_pad(lcd, &pad2);
		draw_ball(lcd, (int) ball.x, (int) ball.y);

		//Save the drawed ball pos
		ball.old_x = (int) ball.x;
		ball.old_y = (int) ball.y;

		//Save player 1 state
		player_1.pad.old_y = (int) player_1.pad.y;
		player_1.pad2.old_y = player_1.pad.old_y;
		//Save player 2 state
		player_2.pad.old_y = (int) player_2.pad.y;
		player_2.pad2.old_y = player_2.pad.old_y;

		//move the ball
		ball.x += ball.dx;
		ball.y += ball.dy;

		//Upper and lower boundarys check
		if (((int) ball.y) >= 61 || ((int) ball.y) <= 10) {
			ball.dy = -ball.dy;
			ball.y += ball.dy;
		}

		//Returns 1 on hit and 0 on no hit, not used, can be used for some cool animation
		//The order is of no importance
		handle_ball_hit_pad(&(player_1.pad), &ball);
		handle_ball_hit_pad(&(player_1.pad2), &ball);
		handle_ball_hit_pad(&(player_2.pad), &ball);
		handle_ball_hit_pad(&(player_2.pad2), &ball);

		//This could be a function since it is basicly copy paste.
		if (((int) ball.x) >= 126) {//Player 2 missed the ball
			lost_ball_animation(lcd, ball.x, ball.y); //Play the cool animation
			ball.x = 65;
			ball.y = 30;
			ball.dx = -ball.dx; //Respawn ball at the other direction
			ball.dy = 0;
			player_1.score += 1; //Increase player 1 score

		}

		if (((int) ball.x) <= 1) {//Player 1 missed the ball!
			lost_ball_animation(lcd, ball.x, ball.y); //Play the cool animation
			ball.x = 65;
			ball.dx = -ball.dx;//Respawn ball at the other direction
			ball.dy = 0;
			ball.y = 30;
			player_2.score += 1;//Increase player 2 score
		}

		pushRAM(lcd); //Push the framebuffer to LCD
		_delay_ms(1);//Delay game
	}
	return 1; //Return error, this should never happen

}
