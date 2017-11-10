#include "touch.h"
#include <avr/io.h>
#include "connections.h"
#include "macros.h"

#include <avr/interrupt.h> 
#include "buttons.h"



/*

BOTTOM 		=	PA0 
LEFT		= 	PA3
TOP			= 	PA2
RIGHT		=	PA1

Rigth -> 1.8 kohm -> GND

*/

//0 = off
//1 = listen for press
//2 = read y
//3 = read x

//#define TOUCH_STATE_PRESSED 	2
//#define TOUCH_STATE_WAITING 	1
//#define TOUCH_STATE_OFF 		0


unsigned char TOUCH_READING;

Point private_touch_point;

ISR(ADC_vect)
{

int value=ADCH;
if (TOUCH_READING){//Read y
if (value<=35){//Stopt pressing
private_touch_point.y=0;
private_touch_point.x=0;
private_touch_point.down_count=0;
//private_touch_point.action=ACTION_NONE;
//TOUCH_STATE=5;
//start_reading_with_interrupt();
////////////start_reading_with_interrupt();
TOUCH_STATE=TOUCH_STATE_WAITING;
return;
}

private_touch_point.y=value;
set_x_reading();
TOUCH_READING=0;
start_reading_with_interrupt();
////////////start_reading_with_interrupt();

}else {// Read x
private_touch_point.x=value;
private_touch_point.down_count++;
if (private_touch_point.down_count>250){
private_touch_point.down_count=100;
}
//private_touch_point.action=ACTION_DOWN;
TOUCH_READING=1;
set_y_reading();
TOUCH_STATE=TOUCH_STATE_PRESSED;
//TOUCH_STATE=5;
}
////////////start_reading_with_interrupt();
}



Point getRawTouch(){
//TOUCH_STATE=TOUCH_STATE_WAITING;
//TOUCH_STATE=1;
//TOUCH_READING=1;
//set_y_reading();
//start_reading_with_interrupt();
//while (TOUCH_STATE!=5){}
Point P;

P.y=private_touch_point.y;
P.x=private_touch_point.x;
P.down_count=private_touch_point.down_count;
/*
return P;
P.y=0;
P.x=0;
int y=get_touch_y();
if (y<35){

P.action=ACTION_NONE;
}else if (!TC.calibrated){
P.y=y;
P.x=get_touch_x();
}else {
//y=get_touch_y();
P.y=y*TC.k_y+TC.m_y;
//x=get_touch_x();
P.x=get_touch_x()*TC.k_x+TC.m_x;
}
*/
return P;
}


Point getTouch(){
//TOUCH_STATE=TOUCH_STATE_WAITING;
//TOUCH_STATE=1;
//TOUCH_READING=1;
//set_y_reading();
//start_reading_with_interrupt();
//while (TOUCH_STATE!=5){}
Point P;

P.y=private_touch_point.y*TC.k_y/100+TC.m_y;
P.x=private_touch_point.x*TC.k_x/100+TC.m_x;
P.down_count=private_touch_point.down_count;
/*
return P;
P.y=0;
P.x=0;
int y=get_touch_y();
if (y<35){

P.action=ACTION_NONE;
}else if (!TC.calibrated){
P.y=y;
P.x=get_touch_x();
}else {
//y=get_touch_y();
P.y=y*TC.k_y+TC.m_y;
//x=get_touch_x();
P.x=get_touch_x()*TC.k_x+TC.m_x;
}
*/
return P;
}
void set_calibration(unsigned char diff_x,unsigned char m_x,unsigned char diff_y,unsigned char m_y){
TC.k_x=-3000/(diff_x);
//m_x=25-(k_x*x_0)/100;
TC.k_y=-2400/(diff_y);
//m_y=21-(k_y*y_0)/100;



TC.m_x=m_x;
TC.m_y=m_y;


}

void load_eeprom_touch(){

unsigned char diff_x=eeprom_read(11);
unsigned char m_x=eeprom_read(12);
unsigned char diff_y=eeprom_read(13);
unsigned char m_y=eeprom_read(14);


set_calibration(diff_x,m_x,diff_y,m_y);



//k_x=-3000/(x_0-p.x);


//k_y=-2400/(y_0-p.y);


/*
unsigned char diff_x_H=eeprom_read(11);//(x_0-p.x), first 4 bits
unsigned char diff_x_L=eeprom_read(12);//(x_0-p.x), last 4 bits
unsigned char m_x_H=eeprom_read(13);//(x_0-p.x), first 4 bits
unsigned char m_x_L=eeprom_read(14);//(x_0-p.x), last 4 bits

unsigned char diff_y_H=eeprom_read(15);//(y_0-p.y), first 4 bits
unsigned char diff_y_L=eeprom_read(16);//(y_0-p.y), last 4 bits
unsigned char m_y_H=eeprom_read(17);//(x_0-p.x), first 4 bits
unsigned char m_y_L=eeprom_read(18);//(x_0-p.x), last 4 bits



TC.k_x=(-30/(double)((diff_x_H<<4)&(diff_x_L)))*100;
m_x=25-k_x*x_0;

TC.k_y=(-24/(double)((diff_y_H<<4)&(diff_y_L)))*100;
m_y=21-k_y*y_0;


TC.m_x=(m_x_H<<4)&(m_x_L);
TC.m_y=(m_y_H<<4)&(m_y_L);
*/
}

void touch_init_advance(void){
//DDRA=0x00;
//ADMUX= 0b01100000; //single ended ADC0
ddd=0;
private_touch_point.y=0;
private_touch_point.x=0;
ADMUX= 0b00100000; //single ended ADC0

ADCSRA=0b10000000; //enable 


   ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
   sei();   // Enable Global Interrupts

	//read from eeprom
	load_eeprom_touch();



   TOUCH_STATE=TOUCH_STATE_WAITING;
   set_y_reading();
  	start_reading_with_interrupt();

}

void touch_init(void){
DDRA=0x00;
//ADMUX= 0b01100000; //single ended ADC0

ADMUX= 0b00100000; //single ended ADC0

ADCSRA=0b10000000; //enable 

   ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
   //sei();   // Enable Global Interrupts 


}
void start_reading_with_interrupt(){
	ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
	ADCSRA|=0b01000000; //start next
}

void set_y_reading(){
ADCSRA=0x00;
DDRA=0xFF; //Allt ut
PORTA=0x00; //GND
DDRA=0x00; //Allt in
PORTA=0x00; //varför?
ADMUX= 0b00100001; //single ended ADC1
ADCSRA=0b10000000; //enable 
//ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
//Sätt rätt ut och in
DDRA|=(1<<DDA0)|(1<<DDA2);
//PORTA|=(1<<PINA0);
//PORTA&=~(1<<PINA2);
PORTA|=(1<<PINA2);
PORTA&=~(1<<PINA0);

}

void set_x_reading(){
ADCSRA=0x00;
DDRA=0xFF; //Allt ut
PORTA=0x00; //GND
DDRA=0x00; //Allt in
PORTA=0x00; //varför?
ADMUX= 0b00100000; //single ended ADC1
ADCSRA=0b10000000; //enable 
//ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
//Sätt rätt ut och in
DDRA|=(1<<DDA1)|(1<<DDA3);
//PORTA|=(1<<PINA0);
//PORTA&=~(1<<PINA2);
PORTA|=(1<<PINA3);
PORTA&=~(1<<PINA1);
}







int get_touch_x(){

/*
get_touch_y();
ADCSRA=0x00;
DDRA=0xFF; //Allt ut
PORTA=0x00; //GND
DDRA=0x00; //Allt in
PORTA=0x00; //varför?
ADMUX= 0b00100001; //single ended ADC1
ADCSRA=0b10000000; //enable 
ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
//Sätt rätt ut och in
DDRA|=(1<<DDA0)|(1<<DDA2);
//PORTA|=(1<<PINA0);
//PORTA&=~(1<<PINA2);
PORTA|=(1<<PINA2);
PORTA&=~(1<<PINA0);
*/

set_x_reading();
ADCSRA|=0b01000000; //start 

while ((ADCSRA&ADSC)>>ADSC != 0){}

return ADCH;
}


void start_adc_check(){


}

int get_touch_y(){
/*
DDRA=0xFF;
PORTA=0x00;
DDRA=0x00;
PORTA=0x00;
//DDRA=0xFF;
//PORTA=0x00;
//DDRA=0x00;
ADMUX= 0b00100000; //single ended ADC0
ADCSRA=0b10000000; //enable 
ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
//Sätt rätt ut och in
DDRA|=(1<<DDA1)|(1<<DDA3);
PORTA|=(1<<PINA3);
PORTA&=~(1<<PINA1);

*/
set_y_reading();
ADCSRA|=0b01000000; //start 

while ((ADCSRA&ADSC)>>ADSC != 0){}

return ADCH;
}




//Calibrate
void calibrate_touch(LCD_engine *lcd){
//touch_init();
//get_touch_x();
LCD_CLR();
setDubbelbuffer(lcd,1);
LCD_RAM_CLR(lcd);
//LCD_RAM_CLR();
DOGENABLE;

//LCD_setPage(0);
//LCD_setColumn(10);
//rprintfStr("Hello TOUCH-test!");

/*
Räta linjen
x:  x=k_x*r_x+m_x
tryck 55 få x_0
tryck 5 få x_1
tryck 110 få x_2
k_x=(55-5)/(x_0-x_1);
m_x=55-k_x;

*/
Point p;
int k_x=0;
unsigned char m_x=0;
int x_0=0;
int X,Y;
int old_X=0;
int old_Y=0;
int k_y=0;
unsigned char m_y=0;
unsigned char diff_x=0;
unsigned char diff_y=0;
int y_0=0;
//int D_x=0;
//int D_y=0;
char count=0;
char down=0;
//draw_cross(lcd,50,30);
LCD_setPage(2);
LCD_setColumn(22);rprintfStr("x");
ADCSRA|=0b01000000; //start 
while (1){

if (START_isPressedAndRel()){
		break;
		}
p=getRawTouch();
//y=get_touch_y();
Y=(p.y*k_y)/100+m_y;
//x=get_touch_x();
//X=p.x*k_x+m_x;
X=(p.x*k_x)/100+m_x;

/*
LCD_setPage(0);
LCD_setColumn(0);rprintfStr("r_x:");rprintfInt(x);rprintfStr(",");rprintfInt(X);rprintfStr("  ");
LCD_setPage(1);
LCD_setColumn(0);rprintfStr("r_y:");rprintfInt(y);rprintfStr(",");rprintfInt(Y);rprintfStr("  ");
LCD_setPage(0);
LCD_setColumn(90);rprintfStr("c:");rprintfInt(count);rprintfStr("  ");
LCD_setPage(6);
LCD_setColumn(0);rprintfStr("k_y");rprintfInt(k_y*10);rprintfStr("m_y");rprintfInt(m_y*10);
LCD_setPage(7);
LCD_setColumn(0);
rprintfStr(" k_x");rprintfInt(k_x*10);rprintfStr("m_x");rprintfInt(m_x*10);
*/
LCD_setPage(0);LCD_setColumn(0);rprintfInt(p.down_count);
LCD_setColumn(90);rprintfStr("c:");rprintfInt(count);rprintfStr("  ");
if (p.down_count>0){//PRESSED
if (down==5){


	if (count==0){
x_0=p.x;
y_0=p.y;
LCD_setPage(2);
LCD_setColumn(22);rprintfStr(" ");
LCD_setPage(5);
LCD_setColumn(52);rprintfStr("x");


}
if (count==1){
//k_x=-30/(double)(x_0-p.x);

diff_x=x_0-p.x;
diff_y=y_0-p.y;

//k_x=-3000/(x_0-p.x);
k_x=-3000/(diff_x);
m_x=25-(k_x*x_0)/100;

//k_y=-2400/(y_0-p.y);
k_y=-2400/(diff_y);
m_y=21-(k_y*y_0)/100;
//int D_x=x_0-p.x;
//int D_y=y_0-p.y;

eeprom_write(11,diff_x);//
eeprom_write(12,m_x);//
eeprom_write(13,diff_y);//
eeprom_write(14,m_y);//
//eeprom_write(11,(unsigned int)((x_0-p.x)>>4));//(x_0-p.x), first 4 bits
//eeprom_write(12,(unsigned int)((x_0-p.x)&0x0F));//(x_0-p.x), last 4 bits
//eeprom_write(13,(unsigned int)((m_x)>>4));//(m_x), first 4 bits
//eeprom_write(14,(unsigned int)((m_x)&0x0F));//(m_x), last 4 bits


//eeprom_write(15,(unsigned int)((y_0-p.y)>>4));//(y_0-p.y), first 4 bits
//eeprom_write(16,(unsigned int)((y_0-p.y)&0x0F));//(y_0-p.y), last 4 bits
//eeprom_write(17,(unsigned int)((m_y)>>4));//(m_x), first 4 bits
//eeprom_write(18,(unsigned int)((m_y)&0x0F));//(m_x), last 4 bits



TC.k_y=k_y;
TC.k_x=k_x;
TC.m_x=m_x;
TC.m_y=m_y;




}
	
	


count++;


}

down++;
if (down>5 && k_x!=0){
pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
LCD_drawLine(lcd,0,old_Y,127,old_Y);
LCD_drawLine(lcd,old_X,0,old_X,63);
pixelSetOrClearOrInvert(lcd,SET_PIXELS);
LCD_drawLine(lcd,0,Y,127,Y);
LCD_drawLine(lcd,X,0,X,63);
old_Y=Y;
old_X=X;
}

if (down>10)
	down=10;
}else {

if (k_x!=0){
pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
LCD_drawLine(lcd,0,old_Y,127,old_Y);
LCD_drawLine(lcd,old_X,0,old_X,63);
	}
down=0;
}

if (A_isPressed()){
LCD_setPage(2);
LCD_setColumn(0);rprintfStr("diff_x:");rprintfInt(diff_x);
rprintfStr(" m_x:");rprintfInt(TC.m_x);
LCD_setPage(3);
LCD_setColumn(0);rprintfStr("diff_y:");rprintfInt(diff_y);
rprintfStr(" m_y:");rprintfInt(TC.m_y);

}
pushRAM(lcd);
//if (~R_BUTTONS&(1<<B_A)){
//		Touch touch;
//		y=get_touch_y();

//		touch.k_x=k_x;
//		touch.k_y=k_y;
//		touch.m_y=m_y;
//		touch.m_x=m_x;
//		paint(lcd,touch);

//	}
}



}

unsigned char eeprom_read(unsigned int adress){
while (EECR & (1<<EEPE)){} //Wait if already writing
EEAR=adress;
EECR|=(1<<EERE);
return EEDR;
}

void eeprom_write(unsigned int adress,unsigned char data){
while (EECR & (1<<EEPE)){} //Wait if already writing
EEAR=adress;
EEDR=data;
EECR|=(1<<EEMPE);
EECR|=(1<<EEPE);

}


