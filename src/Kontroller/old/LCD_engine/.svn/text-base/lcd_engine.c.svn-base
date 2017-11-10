#include "lcd_engine.h"
#include <avr/io.h>
#include <stdlib.h>
#include "connections.h"
#include "macros.h"
#include <util/delay.h>





//***************************************************
//					Init LCD_engine			
//***************************************************
//void hidden_print(char * x);
//void know(LCD_engine * me);
LCD_engine * init_lcd(void){
	LCD_engine *me=malloc(sizeof(LCD_engine));

pixelSetOrClearOrInvert(me,SET_PIXELS);
LCD_init();

	//me->k=31;
	//me->print=hidden_print;
	//me->know=know;
	//me->print=printf;
	//void (*aa)(char *x)=printf;
return me;
}


LCD_engine * get_new_ram(void){
	LCD_engine *me=malloc(sizeof(LCD_engine));

pixelSetOrClearOrInvert(me,SET_PIXELS);

	//me->k=31;
	//me->print=hidden_print;
	//me->know=know;
	//me->print=printf;
	//void (*aa)(char *x)=printf;
return me;
}

void trash_ram(LCD_engine * me){
free(me);
}

//void hidden_print(char * x){
	//printf(x);
//}

//void know(LCD_engine * me){
//me->print("I do know my self!");
//}



//Private functions



void LCD_init(void){


SPI_MasterInit();

DOG_MasterInit();

PORT_RE &= ~(1<<OUT_RE); // RES\ = LOW 
_delay_ms(50);
PORT_RE |= (1<<OUT_RE); // RES\ = HIGH
_delay_ms(50);

initDOGL128();



LCD_CLR();
DOGENABLE;
}



void DOG_MasterInit(void){
//Direction 
DDR_SPI|=(1<<DD_A0)|(1<<DD_RE); //A0 and RE is out 
//Data
//PORT_CS&=~(1<<OUT_CS); //Chip select to 0 
PORT_RE|=(1<<OUT_RE);  //Reset to 1
PORT_A0|=(1<<OUT_A0);  //A0 (command or data) to 1
}

void SPI_MasterInit(void)
{
/* Set MOSI and SCK output, all others input */
//DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_CS);
DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_CS);

/* Enable SPI, Master, set clock rate fck/16 */
SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
/* Start transmission */
SPDR = cData;
/* Wait for transmission complete */
while(!(SPSR & (1<<SPIF))){}
}


void initDOGL128(void)
{
  	DOGENABLE;
  	DOGCOMMAND;
/*
//RESET
	//dogm_reset(); //(100ms)
SPI_MasterTransmit(0x40);
SPI_MasterTransmit(0xA1);
SPI_MasterTransmit(0xC0);
SPI_MasterTransmit(0xA6);
SPI_MasterTransmit(0xA2);
SPI_MasterTransmit(0x2F);
SPI_MasterTransmit(0xF8);
SPI_MasterTransmit(0x00);
SPI_MasterTransmit(0x27);
SPI_MasterTransmit(0x81);
SPI_MasterTransmit(0x10);
SPI_MasterTransmit(0xAC);
SPI_MasterTransmit(0x00);
SPI_MasterTransmit(0xAF);
//SPI_MasterTransmit(0xA5);
DOGDISABLE;
*/	
	SPI_MasterTransmit(DISPSTART + 0);
	//SPI_MasterTransmit(ADCNORMAL);
	SPI_MasterTransmit(ADCREVERSE);
     SPI_MasterTransmit(COMNORMAL);

	
    //SPI_MasterTransmit(COMREVERSE);



    SPI_MasterTransmit(DISPNORMAL);
	//SPI_MasterTransmit(DISPREVERSE);
    SPI_MasterTransmit(LCDBIAS9);
    SPI_MasterTransmit(SETPOWERCTRL+7);
    SPI_MasterTransmit(BOOSTERRATIO);       
	SPI_MasterTransmit(0);
    SPI_MasterTransmit(REGRESISTOR+7);
    SPI_MasterTransmit(SETCONTRAST);        
	SPI_MasterTransmit(CONTRAST_VALUE);
    SPI_MasterTransmit(STATINDMODE);        
	SPI_MasterTransmit(0);
    SPI_MasterTransmit(DISPON);
	//SPI_MasterTransmit(0xA4);
	
	//
	//DOGDATA;
  DOGDISABLE; //Why?


}
void swaap(int *i, int *j) {
   int t = *i;
   *i = *j;
   *j = t;
}

void pixelSetOrClearOrInvert(LCD_engine * me, char set){
me->SETPIXEL=set;
}
void DisplaySetDispInverted(LCD_engine * me, char set){
me->DISPINVERTED=set;// 1 for inverted, 0 for normal
DOGCOMMAND;
if (set){
SPI_MasterTransmit(DISPREVERSE);
	//SPI_MasterTransmit(DISPREVERSE);
}else {
SPI_MasterTransmit(DISPNORMAL);
}
//		SPI_MasterTransmit(DISPREVERSE);
	//	DOGDATA;
}
void setDubbelbuffer(LCD_engine * me,char dubbel){
me->DUBBELBUFFER=dubbel;
}

void LCD_draw16x16ImageToRam(LCD_engine * me,unsigned char image[32],unsigned char x,unsigned char y){


//for (int row=0;row<2;row++){
for (int i=0;i<16;i++){
//me->LCD_RAM[((y/8)+row)*128+(i+x)]|=image[(i+row*16)];
if (y%8!=0){

me->LCD_RAM[((y/8))*128+(i+x)]|=image[(i)]<<(y%8);

me->LCD_RAM[((y/8)+1)*128+(i+x)]|=image[(i)]>>(8-y%8);

me->LCD_RAM[((y/8)+1)*128+(i+x)]|=image[(i+16)]<<(y%8);

me->LCD_RAM[((y/8)+2)*128+(i+x)]|=image[(i+16)]>>(8-y%8);

//me->LCD_RAM[((y/8)+1)*128+(i+x)]|=image[(i)]>>(y%8);
//me->LCD_RAM[((y/8)+1)*128+(i+x)]|=image[(i+16)]<<(y%8);


//me->LCD_RAM[((y/8)+row+1)*128+(i+x)]|=image[(i+row*16)]<<(y%8);
}else{
me->LCD_RAM[((y/8))*128+(i+x)]|=image[(i)];
me->LCD_RAM[((y/8)+1)*128+(i+x)]|=image[(i+16)];
}
//me->LCD_RAM[((y/8)+row-1)*128+(i+x)]|=image[(i+row*16)]>>(y%8);

}
//}

if (y%8!=0){
me->LCD_changed_RAM[(((y)/8+2)*128+x)/8]=0xFF;
me->LCD_changed_RAM[(((y)/8+2)*128+x)/8+1]=0xFF;

}

me->LCD_changed_RAM[((y/8)*128+x)/8]=0xFF;
me->LCD_changed_RAM[((y/8)*128+x)/8+1]=0xFF;
me->LCD_changed_RAM[(((y)/8+1)*128+x)/8]=0xFF;
me->LCD_changed_RAM[(((y)/8+1)*128+x)/8+1]=0xFF;

//for (int j=0;j<128;j++){
//me->LCD_changed_RAM[j]=0xFF;
//}



}

void LCD_drawImageToRam(LCD_engine * me,unsigned char image[1024]){

for (int i=0;i<1024;i++){
me->LCD_RAM[i]=image[i];
}
for (int j=0;j<128;j++){
me->LCD_changed_RAM[j]=0xFF;
}



}

//void LCD_drawPixel(LCD_engine * me,unsigned char x,unsigned char y){
void LCD_drawPixel(LCD_engine * me,int x,int y){
if (x>127 || y>63 || x<0 || y<0){
	return;
}


if (me->SETPIXEL== SET_PIXELS){//set pixel
me->LCD_RAM[(y/8)*128+x]|=1<<(y%8); //Set
} else if (me->SETPIXEL== CLEAR_PIXELS){//clear pixel
me->LCD_RAM[(y/8)*128+x]&=~(1<<(y%8)); //Clear
}else {//INVERT_PIXELS allways go to this when out of range, for debugging

if (me->LCD_RAM[(y/8)*128+x]&(1<<(y%8))){//is it set?

me->LCD_RAM[(y/8)*128+x]&=~(1<<(y%8)); //Clear
}else {
me->LCD_RAM[(y/8)*128+x]|=1<<(y%8); //Set
}


}

if (me->DUBBELBUFFER){
//LCD_changed_RAM[((y/8)*128+x)/8]|=1<<(((y/8)*128+x)%8);
//LCD_changed_RAM[((y/8)*128+x)/8]=0xFF;
//unsigned char pos=(y/8)*128+x;
//LCD_changed_RAM[pos/8]|=1<<(pos%8);
me->LCD_changed_RAM[((y/8)*128+x)/8]|=1<<(7-((y/8)*128+x)%8);
return;
}
LCD_setColumn(x);
LCD_setPage(y/8);
DOGDATA;
SPI_MasterTransmit(me->LCD_RAM[(y/8)*128+x]);
}
void redrawRAM(LCD_engine * me){
for(int adrPage = 0; adrPage < 8; adrPage++)
	{  
	
	DOGCOMMAND;
	SPI_MasterTransmit(PAGEADR+adrPage);
	SPI_MasterTransmit(COLADRH+0);
	SPI_MasterTransmit(COLADRL+0);
	DOGDATA;
      	for(int adrLaw = 0; adrLaw < 128; adrLaw++)
      	{SPI_MasterTransmit(me->LCD_RAM[adrPage*128+adrLaw]);
		}
	}

}

void pushRAM(LCD_engine * me){
int active_col=0;
for(int adrPage = 0; adrPage < 8; adrPage++)
	{  
	
	DOGCOMMAND;
	SPI_MasterTransmit(PAGEADR+adrPage);
	SPI_MasterTransmit(COLADRH+0);
	SPI_MasterTransmit(COLADRL+0);
	active_col=-1;
		//LCD_SetPageAddress(adrPage);
		//LCD_SetColumnAddress(0);
		DOGDATA;
      	for(int adrLaw = 0; adrLaw < 128; adrLaw++)
      	{	//LCD_RAM[adrPage*128+adrLaw]|=0x00;
			
	//LCD_changed_RAM[((y/8)*128+x)/8]|=1<<(((y/8)*128+x)%8);
			//unsigned char pos=adrPage*128+adrLaw;
			//if (LCD_changed_RAM[((adrPage)*128+adrLaw)/8]){//pixel have changed
      		if (me->LCD_changed_RAM[((adrPage)*128+adrLaw)/8]&(1<<(7-((adrPage)*128+adrLaw)%8))){//pixel have changed
				
				if (active_col!=adrLaw){
				LCD_setColumn(adrLaw);
				active_col=adrLaw;
				DOGDATA;
				}
				active_col++;
				
				//SPI_MasterTransmit(~me->LCD_RAM[adrPage*128+adrLaw]);
				
				SPI_MasterTransmit(me->LCD_RAM[adrPage*128+adrLaw]);
				
				me->LCD_changed_RAM[((adrPage)*128+adrLaw)/8]&=~(1<<(7-((adrPage)*128+adrLaw)%8));
				//LCD_changed_RAM[((adrPage)*128+adrLaw)/8]&=~(1<<(((adrPage)*128+adrLaw)%8));
				//LCD_changed_RAM[pos/8]&=~1<<(pos%8);
				//LCD_changed_RAM[((y/8)*128+x)/8]|=1<<(((y/8)*128+x)%8);
				//LCD_changed_RAM[((adrPage)*128+adrLaw)/8]=0;
				//_delay_ms(200);
      		}
			
		}
   	}
}

void LCD_drawLine(LCD_engine * me,int x0,int y0,int x1,int y1){
//http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	int steep=abs(y1 - y0) > abs(x1 - x0);
    if (steep){
         swaap(&x0, &y0);
         swaap(&x1, &y1);	 
	}
    if (x0 > x1){
           swaap(&x0, &x1);
           swaap(&y0, &y1);
	}
     int deltax = x1 - x0;
     int deltay = abs(y1 - y0);
     int error = deltax / 2;
     int ystep;
     int y = y0;
     if (y0 < y1){ystep = 1;} else {ystep = -1;}
     for (int x = x0 ; x<=x1;x++){
         if (steep){ LCD_drawPixel(me,y,x);} else {LCD_drawPixel(me,x,y);}
         error = error - deltay;
         if (error < 0){
             y = y + ystep;
             error = error + deltax;
		}
	}


}


//		FIGURES
void fillRect(LCD_engine * me,int x,int y,int w,int h){
if (w>h){
for (int j=0;j<h;j++)
	LCD_drawLine(me,x,j+y,x+w,j+y);
}else{
for (int j=0;j<w;j++)
	LCD_drawLine(me,x+j,y,x+j,y+h);

}

}

void h_fillRect(LCD_engine * me,int x,int y,int w,int h,int inc){
drawRect(me,x,y,w,h);
for (int j=inc;j<h;j+=inc)
	LCD_drawLine(me,x+1,j+y,x+w-1,j+y);
}

void v_fillRect(LCD_engine * me,int x,int y,int w,int h,int inc){
drawRect(me,x,y,w,h);
for (int j=inc;j<w;j+=inc)
	LCD_drawLine(me,x+j,y+1,x+j,h+y-1);
}

void c_fillRect(LCD_engine * me,int x,int y,int w,int h,int inc){
drawRect(me,x,y,w,h);
for (int j=inc;j<w;j+=inc)
	LCD_drawLine(me,x+j,y+1,x+j,h+y-1);
for (int j=inc;j<h;j+=inc)
	LCD_drawLine(me,x+1,j+y,x+w-1,j+y);
}



void drawRect(LCD_engine * me,int x,int y,int w,int h){
LCD_drawLine(me,x+1,y,x+w-1,y);
LCD_drawLine(me,x+1,y+h,x+w-1,y+h);
LCD_drawLine(me,x,y,x,y+h);
LCD_drawLine(me,x+w,y,x+w,y+h);
}


void fillCircle(LCD_engine * me,int x0, int y0, int radius)
{

//http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
 
  LCD_drawLine(me,x0, y0 + radius,x0, y0 - radius);
  LCD_drawLine(me,x0 + radius, y0,x0 - radius, y0);
  while(x < y)
  {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    

	LCD_drawLine(me,x0 + x, y0 + y,x0 - x, y0 + y);
	LCD_drawLine(me,x0 + x, y0 - y,x0 - x, y0 - y);
	LCD_drawLine(me,x0 + y, y0 + x,x0 - y, y0 + x);
	LCD_drawLine(me,x0 + y, y0 - x,x0 - y, y0 - x);

  }
}

void drawCircle(LCD_engine * me,int x0, int y0, int radius)
{

//http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
 
  LCD_drawPixel(me,x0, y0 + radius);
  LCD_drawPixel(me,x0, y0 - radius);
  LCD_drawPixel(me,x0 + radius, y0);
  LCD_drawPixel(me,x0 - radius, y0);
 
  while(x < y)
  {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0) 
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;    
    LCD_drawPixel(me,x0 + x, y0 + y);
    LCD_drawPixel(me,x0 - x, y0 + y);
    LCD_drawPixel(me,x0 + x, y0 - y);
    LCD_drawPixel(me,x0 - x, y0 - y);
    LCD_drawPixel(me,x0 + y, y0 + x);
    LCD_drawPixel(me,x0 - y, y0 + x);
    LCD_drawPixel(me,x0 + y, y0 - x);
    LCD_drawPixel(me,x0 - y, y0 - x);
  }
}
//		FIGURES - END




void LCD_setContrast(LCD_engine * me,unsigned char contrast){
DOGCOMMAND;
SPI_MasterTransmit(SETCONTRAST);        
SPI_MasterTransmit(contrast);

}
void LCD_RAM_CLR(LCD_engine * me)
{
	for(int i = 0; i < 1024; i++)
	{  
      		me->LCD_changed_RAM[(i)/8]=0;
      		me->LCD_RAM[i]=0x00;

      	
   	}

}
void LCD_CLR(void)
{

	//DOGENABLE;
	//DOGCOMMAND;
	//SPI_MasterTransmit(DISPREVERSE);
	//SPI_MasterTransmit(DISPALLON);
	//SPI_MasterTransmit(DISPNORMAL);
	//SPI_MasterTransmit(DISPALLOFF);

	//return;
   	uint8_t adrPage, adrLaw, Data;
	Data = 0;
   	
	//set page 0
	DOGENABLE;
	//SPI_MasterTransmit(PAGEADR+0);
	//SPI_MasterTransmit(COLADRH+0);
	//SPI_MasterTransmit(COLADRL+0);
	
	for(adrPage = 0; adrPage < 8; adrPage++)
	{  
	DOGCOMMAND;
	SPI_MasterTransmit(PAGEADR+adrPage);
	SPI_MasterTransmit(COLADRH+0);
	SPI_MasterTransmit(COLADRL+0);
		//LCD_SetPageAddress(adrPage);
		//LCD_SetColumnAddress(0);
		DOGDATA;
      	for(adrLaw = 0; adrLaw < 128; adrLaw++)
      	{	//LCD_RAM[adrPage*128+adrLaw]|=0x00;
			//if (me->INVERTED){
      		//SPI_MasterTransmit(~Data);
			//}else {
			SPI_MasterTransmit(Data);
			//}
      	}
   	}
	 DOGDISABLE;
} 
void resetConnection(){DOGENABLE;}
void LCD_setPage(unsigned char page){
DOGCOMMAND;
SPI_MasterTransmit(PAGEADR+page);
//SPI_MasterTransmit(COLADRH+0x7);
//SPI_MasterTransmit(COLADRL+0xF);
//DOGDATA;


}
void LCD_setColumn(unsigned char col){
DOGCOMMAND;
//SPI_MasterTransmit(PAGEADR+page);
SPI_MasterTransmit(COLADRH+((col&0xF0)>>4));
SPI_MasterTransmit(COLADRL+(col&0x0F));
//DOGDATA;

}


//Printing

void printHart(){
DOGDATA;
SPI_MasterTransmit(0b00001100); 				//-##-##-
SPI_MasterTransmit(0b00011110); 				//#######
SPI_MasterTransmit(0b00111110); 				//#######
SPI_MasterTransmit(0b01111100); 				//-#####-
SPI_MasterTransmit(0b00111110); 				//--###--
SPI_MasterTransmit(0b00011110); 				//---#---
SPI_MasterTransmit(0b00001100); 				//-------
}

void rprintfChar(unsigned char c)
{
DOGDATA;




//if (c >95) {//Only big ascii table
if (c >127) {//end of table
//if (c<123){//Small ascii
//c=c-32; //To big ascii
//}else {//Make a hart
printHart();
SPI_MasterTransmit(0x00); // small spaceing
return;
//}
}
if(c < 0x20) c = 0x20;
unsigned long dptr = (c-0x20)*5;

for(int i=0; i< 5; i++)
	{
	//((((x)&0x0F)>9)?((x)+'A'-10):((x)+'0'))
		//((((dptr)&0x0F)>9)?((dptr)+'A'-10):((dptr)+'0'))
    	SPI_MasterTransmit(ASCII5_7[dptr]);
		
    	dptr = dptr + 1;
	}
SPI_MasterTransmit(0x00); // small spaceing





    /*
	uint8_t *dptr;               // dptr_adderss for Dig code.
   	uint8_t i;

   	if(c < 0x20) c = 0x20;
   	dptr = &ASCII5_7[(c-0x20)*5];
	//LCD_SetPageAddress(xy_adrPage);
	//LCD_SetColumnAddress(xy_adrColumn);
	for(i=0; i< 5; i++)
	{
    	//dogm_core_data(pgm_read_byte(dptr));
		
    	dptr = dptr + 1;
	} 
		//xy_adrColumn = xy_adrColumn+6;  // Zeichenabstand 6 = ok, 5 fast zu eng, 

*/
}



void printfChar(LCD_engine * me,unsigned char c,unsigned char *x,unsigned char y)
{
DOGDATA;




//if (c >95) {//Only big ascii table
if (c >127) {//end of table
//if (c<123){//Small ascii
//c=c-32; //To big ascii
//}else {//Make a hart
//printHart();
//SPI_MasterTransmit(0x00); // small spaceing
return;
//}
}
if(c < 0x20) c = 0x20;
unsigned long dptr = (c-0x20)*5;

for(int i=0; i< 5; i++)
	{
	//((((x)&0x0F)>9)?((x)+'A'-10):((x)+'0'))
		//((((dptr)&0x0F)>9)?((dptr)+'A'-10):((dptr)+'0'))
		for (int j=7;j>=0;j--){
			if (ASCII5_7[dptr]&(1<<j)){
				LCD_drawPixel(me,*x,y+j);
				//(*x)=(*x)+1;
				
			}
		}
    	//SPI_MasterTransmit(ASCII5_7[dptr]);
		//x++;
		(*x)=(*x)+1;
    	dptr = dptr + 1;
	}

//SPI_MasterTransmit(0x00); // small spaceing





    /*
	uint8_t *dptr;               // dptr_adderss for Dig code.
   	uint8_t i;

   	if(c < 0x20) c = 0x20;
   	dptr = &ASCII5_7[(c-0x20)*5];
	//LCD_SetPageAddress(xy_adrPage);
	//LCD_SetColumnAddress(xy_adrColumn);
	for(i=0; i< 5; i++)
	{
    	//dogm_core_data(pgm_read_byte(dptr));
		
    	dptr = dptr + 1;
	} 
		//xy_adrColumn = xy_adrColumn+6;  // Zeichenabstand 6 = ok, 5 fast zu eng, 

*/
}

void printfInt(LCD_engine * me,int i,unsigned char x,unsigned char y){
char str[10]; 
printfStr(me,itoa(i, str, 10),x,y);
}
void printfBin(LCD_engine * me,int i,unsigned char x,unsigned char y){
char str[10]; 
printfStr(me,itoa(i, str, 2),x,y);
}
void printfStr(LCD_engine * me,char str[],unsigned char x,unsigned char y)
{
	// send a string stored in RAM
	// check to make sure we have a good pointer
	if (!str) return;

	// print the string until a null-terminator
	while (*str){
		printfChar(me,*str++,&x,y);
		x++;
		}
}

void rprintfInt(int i){
char str[10]; 
rprintfStr(itoa(i, str, 10));
}
void rprintfBin(int i){
char str[10]; 
rprintfStr(itoa(i, str, 2));
}
void rprintfStr(char str[])
{
	// send a string stored in RAM
	// check to make sure we have a good pointer
	if (!str) return;

	// print the string until a null-terminator
	while (*str)
		rprintfChar(*str++);
}
int StrLength(char str[]){
int l=0;
if (!str) return -1;
	// check the string until a null-terminator
	while (*str){
			*str++;
		l++;
	}
	return l;
}
void rprintfStrCenter(char str[])
{LCD_setColumn(64-StrLength(str)*3);
rprintfStr(str);
}

//FONT



static unsigned char ASCII5_7[] =
{
	0x00, 0x00, 0x00, 0x00, 0x00,// (space)   
	0x00, 0x00, 0x5F, 0x00, 0x00,// !         
	0x00, 0x07, 0x00, 0x07, 0x00,// "         
	0x14, 0x7F, 0x14, 0x7F, 0x14,// #         
	0x24, 0x2A, 0x7F, 0x2A, 0x12,// $         
	0x23, 0x13, 0x08, 0x64, 0x62,// %         
	0x36, 0x49, 0x55, 0x22, 0x50,// &         
	0x00, 0x05, 0x03, 0x00, 0x00,// '         
	0x00, 0x1C, 0x22, 0x41, 0x00,// (         
	0x00, 0x41, 0x22, 0x1C, 0x00,// )         
	0x08, 0x2A, 0x1C, 0x2A, 0x08,// *         
	0x08, 0x08, 0x3E, 0x08, 0x08,// +         
	0x00, 0x50, 0x30, 0x00, 0x00,// ,         
	0x08, 0x08, 0x08, 0x08, 0x08,// -         
	0x00, 0x60, 0x60, 0x00, 0x00,// .         
	0x20, 0x10, 0x08, 0x04, 0x02,// /         
	0x3E, 0x51, 0x49, 0x45, 0x3E,// 0         
	0x00, 0x42, 0x7F, 0x40, 0x00,// 1         
	0x42, 0x61, 0x51, 0x49, 0x46,// 2         
	0x21, 0x41, 0x45, 0x4B, 0x31,// 3         
	0x18, 0x14, 0x12, 0x7F, 0x10,// 4         
	0x27, 0x45, 0x45, 0x45, 0x39,// 5         
	0x3C, 0x4A, 0x49, 0x49, 0x30,// 6         
	0x01, 0x71, 0x09, 0x05, 0x03,// 7         
	0x36, 0x49, 0x49, 0x49, 0x36,// 8         
	0x06, 0x49, 0x49, 0x29, 0x1E,// 9         
	0x00, 0x36, 0x36, 0x00, 0x00,// :         
	0x00, 0x56, 0x36, 0x00, 0x00,// ;         
	0x00, 0x08, 0x14, 0x22, 0x41,// <         
	0x14, 0x14, 0x14, 0x14, 0x14,// =         
	0x41, 0x22, 0x14, 0x08, 0x00,// >         
	0x02, 0x01, 0x51, 0x09, 0x06,// ?         
	0x32, 0x49, 0x79, 0x41, 0x3E,// @         
	0x7E, 0x11, 0x11, 0x11, 0x7E,// A         
	0x7F, 0x49, 0x49, 0x49, 0x36,// B         
	0x3E, 0x41, 0x41, 0x41, 0x22,// C         
	0x7F, 0x41, 0x41, 0x22, 0x1C,// D         
	0x7F, 0x49, 0x49, 0x49, 0x41,// E         
	0x7F, 0x09, 0x09, 0x01, 0x01,// F         
	0x3E, 0x41, 0x41, 0x51, 0x32,// G         
	0x7F, 0x08, 0x08, 0x08, 0x7F,// H         
	0x00, 0x41, 0x7F, 0x41, 0x00,// I         
	0x20, 0x40, 0x41, 0x3F, 0x01,// J         
	0x7F, 0x08, 0x14, 0x22, 0x41,// K         
	0x7F, 0x40, 0x40, 0x40, 0x40,// L         
	0x7F, 0x02, 0x04, 0x02, 0x7F,// M         
	0x7F, 0x04, 0x08, 0x10, 0x7F,// N         
	0x3E, 0x41, 0x41, 0x41, 0x3E,// O         
	0x7F, 0x09, 0x09, 0x09, 0x06,// P         
	0x3E, 0x41, 0x51, 0x21, 0x5E,// Q         
	0x7F, 0x09, 0x19, 0x29, 0x46,// R         
	0x46, 0x49, 0x49, 0x49, 0x31,// S         
	0x01, 0x01, 0x7F, 0x01, 0x01,// T         
	0x3F, 0x40, 0x40, 0x40, 0x3F,// U         
	0x1F, 0x20, 0x40, 0x20, 0x1F,// V         
	0x7F, 0x20, 0x18, 0x20, 0x7F,// W         
	0x63, 0x14, 0x08, 0x14, 0x63,// X         
	0x03, 0x04, 0x78, 0x04, 0x03,// Y         
	0x61, 0x51, 0x49, 0x45, 0x43,// Z         
	0x00, 0x00, 0x7F, 0x41, 0x41,// [         
	0x02, 0x04, 0x08, 0x10, 0x20,// "\"       
	0x41, 0x41, 0x7F, 0x00, 0x00,// ]         
	0x04, 0x02, 0x01, 0x02, 0x04,// ^         
	0x40, 0x40, 0x40, 0x40, 0x40,// _         
	0x00, 0x01, 0x02, 0x04, 0x00,// `         
	0x20, 0x54, 0x54, 0x54, 0x78,// a         
	0x7F, 0x48, 0x44, 0x44, 0x38,// b         
	0x38, 0x44, 0x44, 0x44, 0x20,// c         
	0x38, 0x44, 0x44, 0x48, 0x7F,// d         
	0x38, 0x54, 0x54, 0x54, 0x18,// e         
	0x08, 0x7E, 0x09, 0x01, 0x02,// f         
	0x08, 0x14, 0x54, 0x54, 0x3C,// g         
	0x7F, 0x08, 0x04, 0x04, 0x78,// h         
	0x00, 0x44, 0x7D, 0x40, 0x00,// i         
	0x20, 0x40, 0x44, 0x3D, 0x00,// j         
	0x00, 0x7F, 0x10, 0x28, 0x44,// k         
	0x00, 0x41, 0x7F, 0x40, 0x00,// l         
	0x7C, 0x04, 0x18, 0x04, 0x78,// m         
	0x7C, 0x08, 0x04, 0x04, 0x78,// n         
	0x38, 0x44, 0x44, 0x44, 0x38,// o         
	0x7C, 0x14, 0x14, 0x14, 0x08,// p         
	0x08, 0x14, 0x14, 0x18, 0x7C,// q         
	0x7C, 0x08, 0x04, 0x04, 0x08,// r         
	0x48, 0x54, 0x54, 0x54, 0x20,// s         
	0x04, 0x3F, 0x44, 0x40, 0x20,// t         
	0x3C, 0x40, 0x40, 0x20, 0x7C,// u         
	0x1C, 0x20, 0x40, 0x20, 0x1C,// v         
	0x3C, 0x40, 0x30, 0x40, 0x3C,// w         
	0x44, 0x28, 0x10, 0x28, 0x44,// x         
	0x0C, 0x50, 0x50, 0x50, 0x3C,// y         
	0x44, 0x64, 0x54, 0x4C, 0x44,// z         
	0x00, 0x08, 0x36, 0x41, 0x00,// {         
	0x00, 0x00, 0x7F, 0x00, 0x00,// |         
	0x00, 0x41, 0x36, 0x08, 0x00,// }         
	0x08, 0x08, 0x2A, 0x1C, 0x08,// ->        
	0x08, 0x1C, 0x2A, 0x08, 0x08 // <-        

};

