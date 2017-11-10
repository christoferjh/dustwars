#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include "lcd_engine.h"
#include "touch.h"
#include <util/delay.h>
#include "buttons.h"
#include "game_missile.h"
#include "gui.h"
#include <math.h> 
#include "system.h"

//Defining constants that will change game behavior
//Speed of players missiles 
#define M_DEF_SPEED 50
//Speed of incoming missiles
#define M_SPEED 40
//Max missiles at the same time
#define MAXMISSILES 20
//Number of start Bases
#define MAXBASES 4
//The number of levels that exist, can be done in another way, ex by returning a level with negative numbers of waves
#define NUMBEROFLEVELS 7
//The max level delay length
#define MAX_LEVEL_DELAYS 15

//This is how a level is defined, look at 'void getLevel(Level *level,int number)' for more info
typedef struct Level {
	unsigned char delay[MAX_LEVEL_DELAYS];
	unsigned char waves;
	unsigned char wave;
	unsigned char index;
	unsigned char max_active_missiles;
} Level;

//This is the explosion
typedef struct Fire {
	unsigned char life;
	unsigned char size;
} Fire;

//This is the missiles
typedef struct Missile {
	unsigned char power;
	//Start position
	char s_x;
	char s_y;
	//Used for deleting last drawing and some calculation so no need to take times 1/100 
	char o_x;
	char o_y;
	//this is multiplied by 100 to get a decimal
	int x;
	int y;
	//speed
	int dx;
	int dy;
	//target, missile blows up here
	char target_x;
	char target_y;
	//If active or not
	unsigned char active;
	//If sent from player or not, if so then from which base
	unsigned char sent_from;
	//If it is on fire or not, this is its fire, its the missiles explosive
	Fire fire;
} Missile;

//The players Base
typedef struct Base {
	unsigned char size;
	unsigned char pos;//X-pos
	unsigned char alive;
	unsigned char active_missiles; //How many missiles fired from this base that still is flying around
} Base;

//Declare som functions
void draw_missile_fire(LCD_engine *lcd, Missile *m);
int start_game(LCD_engine *lcd, TouchCalibration *TC, int startlevel);

char isOnTarget(Missile *m) { //Checks if the missile is on target or not, pretty bad, should prob use o_x and o_y to skipp div by 100
	return (((m->x / 100 - m->target_x) * (m->x / 100 - m->target_x) + (m->y
			/ 100 - m->target_y) * (m->y / 100 - m->target_y)) < 8); //Well that 8 should be a variable, but this is a pixel game on a fixed screen
}

char uppdateMissile(Missile *m) {//Returns 1 on finish
	//Moving
	m->o_x = m->x / 100;
	m->o_y = m->y / 100;
	m->y += m->dy;
	m->x += m->dx;

	//Check for target
	if (m->fire.life <= 0) {
		if (isOnTarget(m)) {
			//Init fire
			m->fire.size = 1;
			m->fire.life = 40;
			//Stop moving
			m->dy = 0;
			m->dx = 0;

		}
	} else {//Allready blown
		if (m->fire.size < 36) {
			m->fire.size += 2;
		}
		m->fire.life--;
		//Blown out...
		if (m->fire.life <= 0) {
			m->x = m->s_x * 100;
			m->y = m->s_y * 100;

			return 1;
		}

	}
	return 0;
}
//Check if the misssile is on fire
char isOnFire(Missile *m) {
	return m->fire.life > 0;
}
//Returns abs value
int abs(int arg) {
	if (arg < 0)
		return arg * -1;
	return arg;
}
//Erase the drawing of the missile
void clear_missile(LCD_engine *lcd, Missile *m) {
	LCD_drawLine(lcd, m->s_x, m->s_y, m->o_x, m->o_y);
}
//Draw the missile
void draw_missile(LCD_engine *lcd, Missile *m) {
	if (isOnFire(m)) {
		draw_missile_fire(lcd, m);
	} else {
		LCD_drawLine(lcd, m->s_x, m->s_y, m->x / 100, m->y / 100);

	}
}

//Erase the drawing of the explosion (Fire)
void clear_missile_fire(LCD_engine *lcd, Missile *m) {

	LCD_drawLine(lcd, m->o_x - m->fire.size / 10, m->o_y - m->fire.size / 10,
			m->o_x + m->fire.size / 10, m->o_y + m->fire.size / 10);
	LCD_drawLine(lcd, m->o_x - m->fire.size / 10, m->o_y + m->fire.size / 10,
			m->o_x + m->fire.size / 10, m->o_y - m->fire.size / 10);

	LCD_drawLine(lcd, m->o_x - m->fire.size / 7, m->o_y, m->o_x + m->fire.size
			/ 7, m->o_y);
	LCD_drawLine(lcd, m->o_x, m->o_y - m->fire.size / 7, m->o_x, m->o_y
			+ m->fire.size / 7);
}
//Draw the explosion (Fire)
void draw_missile_fire(LCD_engine *lcd, Missile *m) {

	LCD_drawLine(lcd, m->x / 100 - m->fire.size / 10, m->y / 100 - m->fire.size
			/ 10, m->x / 100 + m->fire.size / 10, m->y / 100 + m->fire.size
			/ 10);
	LCD_drawLine(lcd, m->x / 100 - m->fire.size / 10, m->y / 100 + m->fire.size
			/ 10, m->x / 100 + m->fire.size / 10, m->y / 100 - m->fire.size
			/ 10);

	LCD_drawLine(lcd, m->x / 100 - m->fire.size / 7, m->y / 100, m->x / 100
			+ m->fire.size / 7, m->y / 100);
	LCD_drawLine(lcd, m->x / 100, m->y / 100 - m->fire.size / 7, m->x / 100,
			m->y / 100 + m->fire.size / 7);
}

//Wrapper to init the missile, variables speek for themself
void init_missile(Missile *m, char start_x, char start_y, char target_x,
		char target_y, unsigned char sent_from, char speed) {
	m->s_x = start_x;
	m->s_y = start_y;
	m->o_x = start_x;
	m->o_y = start_y;
	m->x = m->s_x * 100;
	m->y = m->s_y * 100;
	m->target_x = target_x;
	m->target_y = target_y;
	m->sent_from = sent_from;
	int R = (int) sqrt((m->s_y - target_y) * (m->s_y - target_y) + (m->s_x
			- target_x) * (m->s_x - target_x));
	m->dy = speed * (-m->s_y + target_y) / R;
	m->dx = speed * (-m->s_x + target_x) / R;
	m->fire.life = 0;
	m->fire.size = 0;
	m->active = 1;
}

//Set missile as non active
void delete_Missile(Missile *m) {
	m->active = 0;
}
//Send missile, wrapper for init_missile
void send_missile(Missile *missiles, char start_x, char start_y, char target_x,
		char target_y, unsigned char sent_from, char speed) {
	for (int i = 0; i < MAXMISSILES; i++) {
		if (!missiles[i].active) { //send this one
			missiles[i].active = 1;
			init_missile(&missiles[i], start_x, start_y, target_x, target_y,
					sent_from, speed);
			break;
		}

	}
}

//Calculates the time diff from to one time to another, taking owerflow in the calculation
unsigned int time_diff(unsigned int from, unsigned int to) {
	if ((to - from) < 0) {

		return to - from + 65536;
	}

	return to - from;
}

//Speaks for itself
int handle_fire_and_bases(Base *Bases, unsigned char x, unsigned char y) {//returns bases left
	int base_hit = -1;

	if (y < 30)
		return base_hit;

	for (int i = 0; i < MAXBASES; i++) {
		if (!Bases[i].alive)
			continue;

		if (((x - Bases[i].pos) * (x - Bases[i].pos) + (y - 60) * (y - 60))
				< 150) {//KILLED THIS BASE

			Bases[i].alive = 0;
			base_hit = i;
			break; //We only let it kill one base
		}
	}
	return base_hit;
}

//returns closets base index, closest meaning only in x-direction
int closestBase(Base *Bases, unsigned char x, unsigned char max_active_missiles) {
	int closest_index = -1;
	unsigned char closest = 255;
	for (int i = 0; i < MAXBASES; i++) {
		if (!Bases[i].alive || Bases[i].active_missiles >= max_active_missiles)
			continue;

		if (abs(Bases[i].pos - x) < closest) {
			closest = abs(Bases[i].pos - x);
			closest_index = i;
		}

	}
	return closest_index;
}

//Levels
void getLevel(Level *level, int number) {
	//The delay array specifies the delay before next missile launch in game loops.
	//The waves variable specifies how many times the delay array is to be used for that level
	//max_active_missiles tels how many missiles each base can have moving at one time
	//the others is for game state and should be set to 0
	//this way to init levels is not the best one, but I hade some wierd erros and this worked.

	level->wave = 0;
	level->index = 0;
	if (number == 1) {

		level->delay[0] = 11;
		level->delay[1] = 101;
		level->delay[2] = 61;
		level->delay[3] = 21;
		level->delay[4] = 11;
		level->delay[5] = 0;
		level->waves = 0;
		level->max_active_missiles = 2;

	} else if (number == 2) {

		level->delay[0] = 10;
		level->delay[1] = 10;
		level->delay[2] = 70;
		level->delay[3] = 90;
		level->delay[4] = 10;
		level->delay[5] = 0;
		level->waves = 1;
		level->max_active_missiles = 1;
	} else if (number == 3) {

		level->delay[0] = 10;
		level->delay[1] = 1;
		level->delay[2] = 10;
		level->delay[3] = 30;
		level->delay[4] = 70;
		level->delay[5] = 0;
		level->waves = 2;
		level->max_active_missiles = 2;
	} else if (number == 4) {

		level->delay[0] = 10;
		level->delay[1] = 1;
		level->delay[2] = 10;
		level->delay[3] = 30;
		level->delay[4] = 70;
		level->delay[5] = 30;
		level->delay[6] = 70;
		level->delay[7] = 0;
		level->waves = 1;
		level->max_active_missiles = 2;
	} else if (number == 5) {

		level->delay[0] = 10;
		level->delay[1] = 1;
		level->delay[2] = 1;
		level->delay[3] = 3;
		level->delay[4] = 70;
		level->delay[5] = 3;
		level->delay[6] = 70;
		level->delay[7] = 3;
		level->delay[8] = 70;
		level->delay[9] = 0;
		level->waves = 1;
		level->max_active_missiles = 2;
	} else if (number == 6) {

		level->delay[0] = 40;
		level->delay[1] = 11;
		level->delay[2] = 1;
		level->delay[3] = 3;
		level->delay[4] = 70;
		level->delay[5] = 30;
		level->delay[6] = 70;
		level->delay[7] = 30;
		level->delay[8] = 70;
		level->delay[9] = 0;
		level->waves = 3;
		level->max_active_missiles = 2;
	} else if (number == 7) {

		level->delay[0] = 10;
		level->delay[1] = 1;
		level->delay[2] = 80;
		level->delay[3] = 3;
		level->delay[4] = 7;
		level->delay[5] = 30;
		level->delay[6] = 7;
		level->delay[7] = 30;
		level->delay[8] = 7;
		level->delay[9] = 3;
		level->delay[10] = 7;
		level->delay[11] = 0;
		level->waves = 6;
		level->max_active_missiles = 2;
	}

}

//Winner splashscreen
void game_missile_winner(LCD_engine *lcd) {
	//fancy words here
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	DOGENABLE;
	LCD_setPage(0);
	LCD_setColumn(10);
	rprintfStr("Missile commander!");
	LCD_setPage(1);
	LCD_setColumn(10);
	rprintfStr("Well, you have saved your");
	LCD_setPage(2);
	LCD_setColumn(10);
	rprintfStr("country!!!");
	LCD_setPage(3);
	LCD_setColumn(10);
	rprintfStr("Good job commander!");

	while (1) {

		if (START_isPressedAndRel()) {
			break;
		}

	}

}

int game_missile(LCD_engine *lcd, TouchCalibration *TC) {
	//Entry point from main meny
	//Start sequence
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	DOGENABLE;
	LCD_setPage(0);
	LCD_setColumn(10);
	rprintfStr("Missile commander!");
	char animation = 0;
	while (1) {
		animation++;
		LCD_setPage(1);
		LCD_setColumn(10);
		if (animation < 30) {
			rprintfStr("                ");
		} else {
			rprintfStr("Press A to start");
		}
		//rprintfInt(animation);
		if (START_isPressedAndRel()) {
			return 0;
		}
		if (A_isPressedAndRel()) {

			break;
		}
	}

	//Loop for starting the game with specified level
	int GAME = 0; //Keep track of the player won or lost the level
	int start_level = 1;
	while (GAME != 2) {//Lost
		GAME = start_game(lcd, TC, start_level); //Starting the game
		if (GAME == 5) //Player exit
			return 0;

		start_level++;
		LCD_CLR();
		LCD_RAM_CLR(lcd);
		DOGENABLE;

		if (GAME == 1) {//Won
			LCD_setPage(3);
			LCD_setColumn(40);
			rprintfStr("WON");
			LCD_setPage(2);
			LCD_setColumn(2);
			rprintfStr("start mission: ");
			rprintfInt(start_level);

		} else if (GAME == 2) {//Lost
			LCD_setPage(3);
			LCD_setColumn(40);
			rprintfStr("LOST");
			LCD_setPage(4);
			LCD_setColumn(2);
			rprintfStr("Mission ");
			rprintfInt(start_level - 1);
			rprintfStr(" was to much.");

		}

		while (1) {
			if (START_isPressedAndRel()) {
				return 0; //Return, no errors
			}
			if (A_isPressedAndRel()) {

				break; //Player ready for next level
			}
		}
		if (start_level > NUMBEROFLEVELS) {
			game_missile_winner(lcd); //Won the entire game
			break;
		}
	}

	return 0;
}

//Check if user would like to quit game
int in_game_missiles_meny(LCD_engine *lcd, TouchCalibration *TC) {//return 1 if exit game
//This is really cool, it starts a new lcd buffer so the old one is intact. To use for ingame menys etc, for example a pause meny.
	DOGENABLE;
	LCD_setPage(0);
	LCD_setColumn(0);
	rprintfStr("Pressed START");
	LCD_engine * newLCD = get_new_ram(); //The cool stuff, allocates a new buffer
	setDubbelbuffer(newLCD, 1);
	LCD_RAM_CLR(newLCD);
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

int start_game(LCD_engine *lcd, TouchCalibration *TC, int startlevel) {

	//Clear screen
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	DOGENABLE;
	setDubbelbuffer(lcd, 1); //Dubbelbuffring is on
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	DOGENABLE;

	//Init the base buffer
	Base Bases[MAXBASES];

	for (int i = 0; i < MAXBASES; i++) {
		Bases[i].pos = (100 / MAXBASES) * i + 20; //Set with same distance 
		Bases[i].size = 10; //Size of the base
		Bases[i].alive = 1; //The base is alive
		Bases[i].active_missiles = 0; //There is no missiles on the move from this base now
	}

	unsigned int game_ticks = 0;

	Missile missiles[MAXMISSILES]; //there cant be more than 20 missiles, but thats more then enaugh

	//Init the missiles, they are all dead
	for (int i = 0; i < MAXMISSILES; i++) {
		missiles[i].active = 0;

	}

	Level level; //=malloc(sizeof(Level)); did not work, wierd errors
	getLevel(&level, startlevel); //Set the level variables with the correct level number
	unsigned char level_delay = 0;
	unsigned char pressCount = 0;
	int GAME = 0;//Running
	unsigned char winning_count = 0;
	char spread_count = 0;
	while (!GAME) {//Game loop


		if (START_isPressedAndRel()) {
			if (in_game_missiles_meny(lcd, TC))
				return 5; //Just exit


		}

		game_ticks = sys_time; //current time


		//check level
		if (level_delay <= 0) {//send next

			if (level.delay[level.index] == 0) {//last

				if (level.wave > level.waves) {//WON
					//GAME=1;
					winning_count++;
					if (winning_count >= 250)
						GAME = 1;
				} else {
					level.index = 0;
					level.wave++;
				}

			} else {

				level_delay = level.delay[level.index];
				level.index++;
				spread_count++;

				//send missile
				//Get target
				//Spreads the missiles so that they dont all come from the same place
				//they also only aims on alive Bases
				if (spread_count % 8 == 0) {
					int base_index = closestBase(Bases, 20, 99);
					if (base_index >= 0)
						send_missile(missiles, 20, 0,
								Bases[(unsigned) base_index].pos, 60, -1,
								M_SPEED);
				} else if (spread_count % 8 == 1) {
					int base_index = closestBase(Bases, 10, 99);
					if (base_index >= 0)
						send_missile(missiles, 90, 0,
								Bases[(unsigned) base_index].pos, 60, -1,
								M_SPEED);

				} else if (spread_count % 8 == 2) {
					int base_index = closestBase(Bases, 80, 99);
					if (base_index >= 0)
						send_missile(missiles, 10, 0,
								Bases[(unsigned) base_index].pos, 60, -1,
								M_SPEED);

				} else if (spread_count % 8 == 3) {
					int base_index = closestBase(Bases, 50, 99);
					if (base_index >= 0)
						send_missile(missiles, 30, 0,
								Bases[(unsigned) base_index].pos, 60, -1,
								M_SPEED);

				} else if (spread_count % 8 == 4) {
					int base_index = closestBase(Bases, 30, 99);
					if (base_index >= 0)
						send_missile(missiles, 120, 0,
								Bases[(unsigned) base_index].pos, 60, -1,
								M_SPEED);

				} else if (spread_count % 8 == 5) {
					int base_index = closestBase(Bases, 80, 99);
					if (base_index >= 0)
						send_missile(missiles, 100, 0,
								Bases[(unsigned) base_index].pos, 60, -1,
								M_SPEED);

				} else if (spread_count % 8 == 6) {
					int base_index = closestBase(Bases, 70, 99);
					if (base_index >= 0)
						send_missile(missiles, 40, 0,
								Bases[(unsigned) base_index].pos, 60, -1,
								M_SPEED);

				} else {
					int base_index = closestBase(Bases, 100, 99);
					if (base_index >= 0)
						send_missile(missiles, 30, 0,
								Bases[(unsigned) base_index].pos, 60, -1,
								M_SPEED);
				}
			}
		} else {
			level_delay--;
		}

		if (TOUCH_STATE == TOUCH_STATE_PRESSED) { //Someone is pressing
			Point p = getTouch(TC); //Get the touch
			if (p.down_count < 8) { //Not accuret touch yet
				pressCount = 0;
			} else {//pressed, sen missile
				if (pressCount != 10 && p.y < 48) {//Touch is accuret and we dont kill ourself
					pressCount = 10; // We have used this touch now
					int base_index = closestBase(Bases, p.x,
							level.max_active_missiles);
					if (base_index >= 0) {//else cannot fire...
						send_missile(missiles,
								Bases[(unsigned) base_index].pos, 50, p.x, p.y,
								base_index, M_DEF_SPEED);
						Bases[(unsigned) base_index].active_missiles++;

					}
				}
			}
		} else {
			pressCount = 0;
		}
		//Erase missiles
		pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
		for (int i = 0; i < MAXMISSILES; i++) {
			if (!missiles[i].active)
				continue;
			clear_missile(lcd, &missiles[i]);
		}
		//Draw missiles
		pixelSetOrClearOrInvert(lcd, SET_PIXELS);
		for (int i = 0; i < MAXMISSILES; i++) {
			if (!missiles[i].active)
				continue;
			draw_missile(lcd, &missiles[i]);
		}

		//check fire
		for (int i = 0; i < MAXMISSILES; i++) {
			if (!missiles[i].active)
				continue;
			if (uppdateMissile(&missiles[i])) {//this missile is done...
				if (missiles[i].sent_from >= 0) {//sent from a base
					Bases[missiles[i].sent_from].active_missiles--;
				}
				pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
				int hit = handle_fire_and_bases(Bases, missiles[i].o_x,
						missiles[i].o_y);
				if (hit >= 0) { //We did just hit a base... KILL IT!
					Base *baseKilled = &Bases[hit];
					fillRect(lcd, baseKilled->pos - baseKilled->size, 62
							- baseKilled->size, baseKilled->size * 2,
							baseKilled->size * 2); //Erase 

				}
				clear_missile_fire(lcd, &missiles[i]);
				missiles[i].active = 0; //This missile is not active any more
			} else {

				if (missiles[i].x >= 130 * 100 || missiles[i].x < 0
						|| missiles[i].y > 65 * 100 || missiles[i].y < 0) {//We are out of bounds, blow it up next round
					//This will make the missile blow up next round since it have hit its target.
					missiles[i].target_x = missiles[i].o_x;
					missiles[i].target_y = missiles[i].o_y;
				}
			}
		}

		//Fire against missile
		//This allows a chain explosion
		for (int i = 0; i < MAXMISSILES; i++) {
			if (!missiles[i].active)
				continue;
			for (int j = 0; j < MAXMISSILES; j++) {
				if (!missiles[i].active || j == i)
					continue; //Dont calculate aginst your self
				if (isOnFire(&missiles[i]) && (abs(missiles[i].o_x
						- missiles[j].o_x) < missiles[i].fire.size / 5 && abs(
						missiles[i].o_y - missiles[j].o_y)
						< missiles[i].fire.size / 5)) {
					missiles[j].target_x = missiles[j].o_x;
					missiles[j].target_y = missiles[j].o_y;
				}

			}
		}

		//Draw the bases
		pixelSetOrClearOrInvert(lcd, SET_PIXELS);
		char alive_bases = 0;
		for (int i = 0; i < MAXBASES; i++) {
			if (!Bases[i].alive)
				continue;
			alive_bases++;
			LCD_setPage(7);
			LCD_setColumn(Bases[i].pos);
			rprintfInt(level.max_active_missiles - Bases[i].active_missiles);
			drawCircle(lcd, Bases[i].pos, 62, Bases[i].size);
		}

		if (alive_bases == 0)
			GAME = 2; // LOST


		pushRAM(lcd);

		int diff = time_diff(game_ticks, sys_time);
		while (diff < 25) {
			diff = time_diff(game_ticks, sys_time);
			LCD_setColumn(10); //Time consuming, wierd erros without, cannot in the world tell you why
		}

	}//Game loop - END


	return GAME; //return if player won or lost

}
