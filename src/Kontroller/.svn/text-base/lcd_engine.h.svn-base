#ifndef Oo_LCD_ENGINE_H_oO
#define Oo_LCD_ENGINE_H_oO

//Holding the framebuffer (LCD_RAM), and the dubbelbuffering variable (LCD_changed_RAM)
//and some LCD settings variables
typedef struct {
unsigned char LCD_RAM[1024];
unsigned char LCD_changed_RAM[128];
char DISPINVERTED;
char SETPIXEL;
char DUBBELBUFFER;
} LCD_engine;

//Needed once, to init the lcd
LCD_engine * init_lcd(void);

//***************************************************
//				LCD COMMANDS
//***************************************************
#define DISPOFF      0xAE
#define DISPON       0xAF
#define DISPSTART    0x40
#define PAGEADR      0xB0
#define COLADRL      0x00
#define COLADRH      0x10
#define ADCNORMAL    0xA0
#define ADCREVERSE   0xA1
#define COMNORMAL    0xC0
#define COMREVERSE   0xC8
#define DISPNORMAL   0xA6
#define DISPALLOFF   0xA4
#define DISPALLON    0xA5
#define DISPREVERSE  0xA7
#define LCDBIAS9     0xA2
#define LCDBIAS7     0xA3
#define RESET        0xE2
#define SETPOWERCTRL 0x28
#define REGRESISTOR  0x20
#define SETCONTRAST  0x81
#define STATINDMODE  0xAC
#define BOOSTERRATIO 0xF8


//***************************************************
//				Values
//***************************************************
#define CONTRAST_VALUE 	16
#define CLEAR_PIXELS 	0
#define SET_PIXELS 		1
#define INVERT_PIXELS	2

//***************************************************
//				Declare functions
//***************************************************
//The names speek for themself. For more info, see lcd_engine.c 

//Vital
void LCD_setColumn(unsigned char col);
void LCD_setPage(unsigned char page);
void LCD_init(void);
void DOG_MasterInit(void);
void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);
void initDOGL128(void);
LCD_engine * get_new_ram(void);
void trash_ram(LCD_engine * me);
void resetConnection();

//LCD handling
void pixelSetOrClearOrInvert(LCD_engine * me, char set);
void DisplaySetDispInverted(LCD_engine * me, char set);
void setDubbelbuffer(LCD_engine * me,char dubbel);
void LCD_RAM_CLR(LCD_engine * me);
void LCD_CLR(void);
void pushRAM(LCD_engine * me);
void redrawRAM(LCD_engine * me);

//LCD drawing
void fillRect(LCD_engine * me,int x,int y,int w,int h);
void h_fillRect(LCD_engine * me,int x,int y,int w,int h,int inc);
void v_fillRect(LCD_engine * me,int x,int y,int w,int h,int inc);
void c_fillRect(LCD_engine * me,int x,int y,int w,int h,int inc);
void drawRect(LCD_engine * me,int x,int y,int w,int h);
void LCD_drawLine(LCD_engine * me,int x0,int y0,int x1,int y1);
void drawCircle(LCD_engine * me,int x0, int y0, int radius);
void fillCircle(LCD_engine * me,int x0, int y0, int radius);
void LCD_drawPixel(LCD_engine * me,int x,int y);

//LCD images
void LCD_drawImageToRam(LCD_engine * me,unsigned char image[1024]);
void LCD_draw16x16ImageToRam(LCD_engine * me,unsigned char image[32],unsigned char x,unsigned char y);

//LCD printing
void rprintfStrCenter(char str[]);
void rprintfInt(int i);
void rprintfBin(int i);
void rprintfStr(char str[]);
void printfInt(LCD_engine * me,int i,unsigned char x,unsigned char y);
void printfBin(LCD_engine * me,int i,unsigned char x,unsigned char y);
void printfStr(LCD_engine * me,char str[],unsigned char x,unsigned char y);

#endif
