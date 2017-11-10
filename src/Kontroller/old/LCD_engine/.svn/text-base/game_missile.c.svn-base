#include <avr/io.h>
#include "connections.h"
#include "macros.h"
#include "lcd_engine.h"
#include <util/delay.h>
#include "buttons.h"
#include "game_missile.h"
#include "touch.h"
#include <math.h> 
#include "system.h"
#define M_DEF_SPEED 50
#define M_SPEED 40
#define MAXMISSILES 20
#define MAXBASES 4
#define NUMBEROFLEVELS 7
#define MAX_LEVEL_DELAYS 15


typedef struct Level{
unsigned char delay[MAX_LEVEL_DELAYS];
unsigned char waves;
unsigned char wave;
unsigned char index;
unsigned char max_active_missiles;
}Level;


typedef struct Fire{
unsigned char life;
unsigned char size;
}Fire;


typedef struct Missile{
unsigned char power;
char s_x;
char s_y;
char o_x;
char o_y;
int x;
int y;
int dx;
int dy;
char target_x;
char target_y;
unsigned char active;
unsigned char sent_from;
Fire fire;

}Missile;

typedef struct Base{
unsigned char size;
unsigned char pos;
unsigned char alive;
unsigned char active_missiles;
}Base;


void draw_missile_fire(LCD_engine *lcd, Missile *m);
int start_game(LCD_engine *lcd,int startlevel);

char isOnTarget(Missile *m){
return (((m->x/100 - m->target_x)*(m->x/100 - m->target_x) + (m->y/100 - m->target_y)*(m->y/100 - m->target_y))<8);
}

char uppdateMissile(Missile *m){//returns 1 on finish
		//moving
		m->o_x=m->x/100;
		m->o_y=m->y/100;
		m->y+=m->dy;
		m->x+=m->dx;
		
		//check for target
		if (m->fire.life<=0){
		if (isOnTarget(m)){
			//init fire
			m->fire.size=1;
			m->fire.life=40;
			//Stop moving
			m->dy=0;
			m->dx=0;
			
		}
		}else {//Allready blown
			if (m->fire.size<36){
				m->fire.size+=2;
			}		
			m->fire.life--;
			//Blown out...
			if (m->fire.life<=0){
				m->x=m->s_x*100;
				m->y=m->s_y*100;
				//m->target_x=0;
				//m->target_y=0;
			//pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
			//fillRect(lcd,fire.x-(fire.size/10)-1,fire.y-(fire.size/10)-1,(fire.size/5)+2,(fire.size/5)+2);
			//pixelSetOrClearOrInvert(lcd,SET_PIXELS);
				return 1;
			}
		
		
		}
	return 0;
}
char isOnFire(Missile *m){
return m->fire.life>0;
}
int abs(int arg){
if (arg<0)
return arg*-1;
return arg;
}
void clear_missile(LCD_engine *lcd, Missile *m){
//LCD_drawLine(lcd,M_1.s_x,M_1.s_y,M_1.o_x,M_1.o_y);
LCD_drawLine(lcd,m->s_x,m->s_y,m->o_x,m->o_y);
}

void draw_missile(LCD_engine *lcd, Missile *m){
//LCD_drawLine(lcd,userMissile.s_x,userMissile.s_y,userMissile.x/100,userMissile.y/100);
if (isOnFire(m)){
draw_missile_fire(lcd,m);}else {
LCD_drawLine(lcd,m->s_x,m->s_y,m->x/100,m->y/100);

}
}

void clear_missile_fire(LCD_engine *lcd, Missile *m){
//LCD_drawLine(lcd,M_1.s_x,M_1.s_y,M_1.o_x,M_1.o_y);
//LCD_drawLine(lcd,m->s_x,m->s_y,m->o_x,m->o_y);

//fillRect(lcd,m->o_x-(m->fire.size/10)-1,m->o_y-(m->fire.size/10)-1,(m->fire.size/5)+2,(m->fire.size/5)+2);

LCD_drawLine(lcd,m->o_x-m->fire.size/10,m->o_y-m->fire.size/10,m->o_x+m->fire.size/10,m->o_y+m->fire.size/10);
LCD_drawLine(lcd,m->o_x-m->fire.size/10,m->o_y+m->fire.size/10,m->o_x+m->fire.size/10,m->o_y-m->fire.size/10);

LCD_drawLine(lcd,m->o_x - m->fire.size/7,m->o_y,m->o_x+m->fire.size/7,m->o_y);
LCD_drawLine(lcd,m->o_x,m->o_y - m->fire.size/7,m->o_x,m->o_y+m->fire.size/7);
}

void draw_missile_fire(LCD_engine *lcd, Missile *m){
//LCD_drawLine(lcd,userMissile.s_x,userMissile.s_y,userMissile.x/100,userMissile.y/100);
//LCD_drawLine(lcd,m->s_x,m->s_y,m->x/100,m->y/100);
//drawCircle(lcd,m->x/100,m->y/100,m->fire.size/10);
//drawCircle(lcd,m->x/100,m->y/100,m->fire.size/10);

LCD_drawLine(lcd,m->x/100-m->fire.size/10,m->y/100-m->fire.size/10,m->x/100+m->fire.size/10,m->y/100+m->fire.size/10);
LCD_drawLine(lcd,m->x/100-m->fire.size/10,m->y/100+m->fire.size/10,m->x/100+m->fire.size/10,m->y/100-m->fire.size/10);

LCD_drawLine(lcd,m->x/100 - m->fire.size/7,m->y/100,m->x/100+m->fire.size/7,m->y/100);
LCD_drawLine(lcd,m->x/100,m->y/100 - m->fire.size/7,m->x/100,m->y/100+m->fire.size/7);

}

void init_missile(Missile *m,char start_x,char start_y,char target_x,char target_y,unsigned char sent_from,char speed){
	m->s_x=start_x;		
	m->s_y=start_y;
	m->o_x=start_x;		
	m->o_y=start_y;
	m->x=m->s_x*100;
	m->y=m->s_y*100;
	m->target_x=target_x;
	m->target_y=target_y;
	m->sent_from=sent_from;
	int R=(int)sqrt((m->s_y-target_y)*(m->s_y-target_y)+(m->s_x-target_x)*(m->s_x-target_x));
	m->dy=speed*(-m->s_y+target_y)/R;
	m->dx=speed*(-m->s_x+target_x)/R;

	m->fire.life=0;
	m->fire.size=0;
	m->active=1;
}
void delete_Missile(Missile *m){
	m->active=0;
}
void send_missile(Missile *missiles,char start_x,char start_y,char target_x,char target_y,unsigned char sent_from,char speed){
for (int i=0;i<MAXMISSILES;i++){
		if (!missiles[i].active){ //send this one
			missiles[i].active=1;
			init_missile(&missiles[i],start_x,start_y,target_x,target_y,sent_from,speed);
		break;
		}
	
		}
}
unsigned int time_diff(unsigned int from,unsigned int to){
if ((to-from)<0){

return to - from +65536;
}

return to-from;
}


int handle_fire_and_bases(Base *Bases,unsigned char x,unsigned char y){//returns bases left
int base_hit=-1;

if (y<30)
	return base_hit;

for (int i=0;i<MAXBASES;i++){
	if (!Bases[i].alive)
		continue;
	
	if ( ((x-Bases[i].pos)*(x-Bases[i].pos)+(y-60)*(y-60))<150 ) {//KILLED THIS BASE
	//if ( abs(x-Bases[i].pos)<Bases[i].size) {//KILLED THIS BASE

		Bases[i].alive=0;
		base_hit=i;
		break; //We only let it kill one base
	}	
}
return base_hit;
}

int closestBase(Base *Bases,unsigned char x,unsigned char max_active_missiles){
int closest_index=-1;
unsigned char closest=255;
for (int i=0;i<MAXBASES;i++){
		if (!Bases[i].alive || Bases[i].active_missiles>=max_active_missiles)
			continue;

		if (abs(Bases[i].pos-x)<closest){
			closest=abs(Bases[i].pos-x);
			closest_index=i;
		}


	}
return closest_index;
}

//Levels
void getLevel(Level *level,int number){
		
//unsigned char d1[] = {100, 150, 12,100, 150, 12,0};
//unsigned char d2[] = {10, 15, 12,0};

		if (number==1){
		
		//level->delay = ;
		level->delay[0]=11;
		level->delay[1]=101;
		level->delay[2]=61;
		level->delay[3]=21;
		level->delay[4]=11;
		level->delay[5]=0;
		level->waves = 0;
		level->index = 0;
		level->max_active_missiles=2;
		level->wave=0;
 		} else if (number==2){

		level->delay[0]=10;
		level->delay[1]=10;
		level->delay[2]=70;
		level->delay[3]=90;
		level->delay[4]=10;
		level->delay[5]=0;
		level->waves = 1;
		level->index = 0;
		level->max_active_missiles=1;
		level->wave=0;
 		}else if (number==3){

		level->delay[0]=10;
		level->delay[1]=1;
		level->delay[2]=10;
		level->delay[3]=30;
		level->delay[4]=70;
		level->delay[5]=0;
		level->waves = 2;
		level->index = 0;
		level->max_active_missiles=2;
		level->wave=0;
 		}else if (number==4){

		level->delay[0]=10;
		level->delay[1]=1;
		level->delay[2]=10;
		level->delay[3]=30;
		level->delay[4]=70;
		level->delay[5]=30;
		level->delay[6]=70;
		level->delay[7]=0;
		level->waves = 1;
		level->index = 0;
		level->max_active_missiles=2;
		level->wave=0;
 		}    else if (number==5){

		level->delay[0]=10;
		level->delay[1]=1;
		level->delay[2]=1;
		level->delay[3]=3;
		level->delay[4]=70;
		level->delay[5]=3;
		level->delay[6]=70;
		level->delay[7]=3;
		level->delay[8]=70;
		level->delay[9]=0;
		level->waves = 1;
		level->index = 0;
		level->max_active_missiles=2;
		level->wave=0;
 		}  else if (number==6){

		level->delay[0]=40;
		level->delay[1]=11;
		level->delay[2]=1;
		level->delay[3]=3;
		level->delay[4]=70;
		level->delay[5]=30;
		level->delay[6]=70;
		level->delay[7]=30;
		level->delay[8]=70;
		level->delay[9]=0;
		level->waves = 3;
		level->index = 0;
		level->max_active_missiles=2;
		level->wave=0;
 		}  else if (number==7){

		level->delay[0]=10;
		level->delay[1]=1;
		level->delay[2]=80;
		level->delay[3]=3;
		level->delay[4]=7;
		level->delay[5]=30;
		level->delay[6]=7;
		level->delay[7]=30;
		level->delay[8]=7;
		level->delay[9]=3;
		level->delay[10]=7;
		level->delay[11]=0;
		level->waves = 6;
		level->index = 0;
		level->max_active_missiles=2;
		level->wave=0;
 		}  
	


//return level;

}


int game_missile(LCD_engine *lcd ){
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	DOGENABLE;
	LCD_setPage(0);
	LCD_setColumn(10);
	rprintfStr("Missile commander!");
	//setDubbelbuffer(lcd,1);
	//pixelSetOrClearOrInvert(lcd,SET_PIXELS);
	//DOGENABLE;
	char animation=0;
	while (1){
	animation++;
	LCD_setPage(1);
	LCD_setColumn(10);
	if (animation<30){
	rprintfStr("                ");
	}else {
	rprintfStr("Press A to start");
	}
	//rprintfInt(animation);
	if (START_isPressedAndRel()){
		return 0;
		}
	if (A_isPressedAndRel()){
		
		break;
		}
	}

int GAME=0;
int start_level=1;
while (GAME!=2){//Lost
	GAME=start_game(lcd,start_level);
	if (GAME==5)
		return 0;

	start_level++;
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	DOGENABLE;


	if (GAME==1){//Won
	LCD_setPage(3);
	LCD_setColumn(40);
	rprintfStr("WON");
	LCD_setPage(2);
	LCD_setColumn(2);
	rprintfStr("start mission: ");rprintfInt(start_level);

	}else if (GAME==2){//Lost
	LCD_setPage(3);
	LCD_setColumn(40);
	rprintfStr("LOST");
	LCD_setPage(4);
	LCD_setColumn(2);
	rprintfStr("Mission ");rprintfInt(start_level-1);rprintfStr(" was to much.");

	}

	while (1){
	if (START_isPressedAndRel()){
		return 0;
		}
	if (A_isPressedAndRel()){
		//return 0;
		
		break;
		}
	}
	if (start_level>NUMBEROFLEVELS)
		break;
}


return 0;	
}



int start_game(LCD_engine *lcd,int startlevel){
	
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	DOGENABLE;
	setDubbelbuffer(lcd,1);
	pixelSetOrClearOrInvert(lcd,SET_PIXELS);
	DOGENABLE;

	Base Bases[MAXBASES];
		
	for (int i=0;i<MAXBASES;i++){
		Bases[i].pos=(100/MAXBASES)*i+20;
		Bases[i].size=10;
		Bases[i].alive=1;
		Bases[i].active_missiles=0;
	}



	Fire fire;	
	fire.life=0;
	unsigned int game_ticks=0;

	

	Missile missiles[MAXMISSILES]; //there cant be more than 10 missiles, but thats more then enugth

	for (int i=0;i<MAXMISSILES;i++){
		missiles[i].active=0;
	
		}

	Level level; //=malloc(sizeof(Level));
	getLevel(&level,startlevel);
	unsigned char level_delay=0;
	unsigned char pressCount=0;
	int GAME=0;//Running
	unsigned char winning_count=0;
	char spread_count=0;
	//return 1;
	while (!GAME){//Game loop

		
		if (START_isPressedAndRel()){
		return 5;
		}

			
		game_ticks = sys_time; //current time

		
		//check level
		if (level_delay<=0){//send next

			if (level.delay[level.index]==0){//last
				
				if (level.wave>level.waves){//WON
					//GAME=1;
					winning_count++;
					if (winning_count>=250)
						GAME=1;
				}else {
				level.index=0;
				level.wave++;
				}
				
				
			}else {
				
			level_delay=level.delay[level.index];
			level.index++;
			spread_count++;
			

			//send missile
			//Get target
			
			if (spread_count%8==0){
				int base_index =	closestBase(Bases,20,99);
				if (base_index>=0)
					send_missile(missiles,20,0,Bases[(unsigned)base_index].pos,60,-1,M_SPEED);
			}else if (spread_count%8==1){
				int base_index =	closestBase(Bases,10,99);
				if (base_index>=0)
					send_missile(missiles,90,0,Bases[(unsigned)base_index].pos,60,-1,M_SPEED);
			

			}else if (spread_count%8==2){
				int base_index =	closestBase(Bases,80,99);
				if (base_index>=0)
					send_missile(missiles,10,0,Bases[(unsigned)base_index].pos,60,-1,M_SPEED);
			

			}else if (spread_count%8==3){
				int base_index =	closestBase(Bases,50,99);
				if (base_index>=0)
					send_missile(missiles,30,0,Bases[(unsigned)base_index].pos,60,-1,M_SPEED);
			

			}else if (spread_count%8==4){
				int base_index =	closestBase(Bases,30,99);
				if (base_index>=0)
					send_missile(missiles,120,0,Bases[(unsigned)base_index].pos,60,-1,M_SPEED);
			

			}else if (spread_count%8==5){
				int base_index =	closestBase(Bases,80,99);
				if (base_index>=0)
					send_missile(missiles,100,0,Bases[(unsigned)base_index].pos,60,-1,M_SPEED);
			

			}else if (spread_count%8==6){
				int base_index =	closestBase(Bases,70,99);
				if (base_index>=0)
					send_missile(missiles,40,0,Bases[(unsigned)base_index].pos,60,-1,M_SPEED);
			

			}else {
				int base_index =	closestBase(Bases,100,99);
				if (base_index>=0)
					send_missile(missiles,30,0,Bases[(unsigned)base_index].pos,60,-1,M_SPEED);		
			}
			}
		}else {
			level_delay--;
		}



		if (TOUCH_STATE==TOUCH_STATE_PRESSED){
		Point p=getTouch();
		if (p.down_count<8){
			//pressCount++;
			pressCount=0;
		}else {//pressed, sen missile
		if (pressCount!=10 && p.y<48){
		pressCount=10;

		//Point p=getTouch();
		//init_missile(&userMissile,10,60,p.x,p.y,M_SPEED);
		 int base_index =	closestBase(Bases,p.x,level.max_active_missiles);
		if (base_index>=0){//else cannot fire...
			//error
			//break;

					send_missile(missiles,Bases[(unsigned)base_index].pos,50,p.x,p.y,base_index,M_DEF_SPEED);
					Bases[(unsigned)base_index].active_missiles++;

		}


		}
		}
		}else {
		pressCount=0;
		}
		
		pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
		for (int i=0;i<MAXMISSILES;i++){
		if (!missiles[i].active)
				continue;
			clear_missile(lcd,&missiles[i]);	
		}



		pixelSetOrClearOrInvert(lcd,SET_PIXELS);
		for (int i=0;i<MAXMISSILES;i++){
		if (!missiles[i].active)
				continue;
			draw_missile(lcd,&missiles[i]);	
		}


		//check fire


		for (int i=0;i<MAXMISSILES;i++){
			if (!missiles[i].active)
				continue;
			if (uppdateMissile(&missiles[i])){//this missile is done...
				if (missiles[i].sent_from>=0){//sent from a base
					Bases[missiles[i].sent_from].active_missiles--;
				}
				pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
				int hit = handle_fire_and_bases(Bases,missiles[i].o_x,missiles[i].o_y);
				if (hit>=0){
					Base *baseKilled=&Bases[hit];
					
					
					fillRect(lcd,baseKilled->pos-baseKilled->size,62-baseKilled->size,baseKilled->size*2,baseKilled->size*2);
				
				}
				
				clear_missile_fire(lcd,&missiles[i]);
				
				
				missiles[i].active=0;
				
					

			}else {
			
				if (missiles[i].x>=130*100 || missiles[i].x<0 || missiles[i].y>65*100 || missiles[i].y<0){
					missiles[i].target_x=missiles[i].o_x;
					missiles[i].target_y=missiles[i].o_y;
				}
			}
		}


		//Fire against missile
		for (int i=0;i<MAXMISSILES;i++){
			if (!missiles[i].active)
				continue;
						for (int j=0;j<MAXMISSILES;j++){
							if (!missiles[i].active || j==i)
								continue;
							if (isOnFire(&missiles[i]) && ( abs(missiles[i].o_x-missiles[j].o_x)<missiles[i].fire.size/5 && abs( missiles[i].o_y-missiles[j].o_y)<missiles[i].fire.size/5) ){
								missiles[j].target_x=missiles[j].o_x;
								missiles[j].target_y=missiles[j].o_y;
							}
								
				
						}
		}

		



		
		
		pixelSetOrClearOrInvert(lcd,SET_PIXELS);
		//drawCircle(lcd,Base_1.pos,62,Base_1.size);
		char alive_bases=0;
		for (int i=0;i<MAXBASES;i++){
			//Bases[i].pos=(100/MAXBASES)*i+20;
			if (!Bases[i].alive)
				continue;
			alive_bases++;
			LCD_setPage(7);
			LCD_setColumn(Bases[i].pos);
			rprintfInt(level.max_active_missiles-Bases[i].active_missiles);
						
			drawCircle(lcd,Bases[i].pos,62,Bases[i].size);


		}

		if (alive_bases==0)
			GAME=2; // LOST


		
		pushRAM(lcd);

		int diff=time_diff(game_ticks,sys_time);
		while (diff<25){
			diff=time_diff(game_ticks,sys_time);
			LCD_setColumn(10); //Time consuming
		}
		
		DOGENABLE;
	}//Game loop - END


return GAME;

}
