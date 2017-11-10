#include "touch.h"
#include "gui.h"
#include "lcd_engine.h"
#include <stdlib.h>
#include "macros.h"
#include "buttons.h"

int Buttons_meny_private(LCD_engine *lcd,ButtonsMeny *BM);


int length(char str[]){
int l=0;
if (!str) return -1;
	// check the string until a null-terminator
	while (*str){
			*str++;
		l++;
	}
	return l;
}




void addMenyButtons(ButtonsMeny *BM, MenyButton **newMenyButton){



}

void addMenyButton(ButtonsMeny *BM, MenyButton *newMenyButton){
//MenyButton
if (BM->RootButton==0){
//if (BM->number_of_buttons==0){
newMenyButton->next=0;
BM->RootButton=newMenyButton;
}else {
newMenyButton->next=BM->RootButton;
BM->RootButton=newMenyButton;
}
//BM->number_of_buttons++;

}

/*
if (BM->Buttons==0){
BM->Buttons=malloc(4*(1));
BM->Buttons=button;
}else{
Button *OldButtons=BM->Buttons;
BM->Buttons=malloc(4*(BM->number_of_buttons+1));
for (int i=0;i<BM->number_of_buttons;i++){
BM->Buttons[i]=OldButtons[i];
}
BM->Buttons[BM->number_of_buttons]=*button;

}
BM->number_of_buttons++;
}
*/


void drawMenyButton(LCD_engine *lcd,MenyButton *button){

if (button->down){
pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
drawRect(lcd,button->x,button->y,button->w-1,button->h-1);
//LCD_drawLine(lcd,button->x+1,button->y+button->h,button->x+button->w,button->y+button->h);
//fillRect(lcd,button->x+button->w/2-length(button->text)*6/2,button->y+button->h/2-4,length(button->text)*6,8);
fillRect(lcd,button->x+button->w/2-button->text_pos,button->y+button->h/2-4,button->text_pos*2,8);


pixelSetOrClearOrInvert(lcd,SET_PIXELS);
drawRect(lcd,button->x+1,button->y+1,button->w-1,button->h-1);
//printfStr(lcd,button->text,button->x+button->w/2-length(button->text)*6/2+1,button->y+button->h/2-4+1);
printfStr(lcd,button->text,button->x+button->w/2-button->text_pos+1,button->y+button->h/2-4+1);
}else {
pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
drawRect(lcd,button->x+1,button->y+1,button->w-1,button->h-1);
//fillRect(lcd,button->x+button->w/2-length(button->text)*6/2+1,button->y+button->h/2-4,length(button->text)*6,8);
fillRect(lcd,button->x+button->w/2-button->text_pos+1,button->y+button->h/2-4,button->text_pos*2,8);

pixelSetOrClearOrInvert(lcd,SET_PIXELS);
drawRect(lcd,button->x,button->y,button->w-1,button->h-1);
LCD_drawLine(lcd,button->x+1,button->y+button->h,button->x+button->w,button->y+button->h);
LCD_drawLine(lcd,button->x+button->w,button->y+1,button->x+button->w,button->y+button->h);
//printfStr(lcd,button->text,button->x+button->w/2-length(button->text)*6/2,button->y+button->h/2-4);
printfStr(lcd,button->text,button->x+button->w/2-button->text_pos,button->y+button->h/2-4);


}
//drawRect(lcd,button->x,button->y,button->w,button->h);
//printfInt(lcd,button->down,button->x+5,button->y+2);



//printfInt(lcd,length(button->text),button->x+2,button->y+20);

}


void drawButton(LCD_engine *lcd,Button *button){

if (button->down){
pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
drawRect(lcd,button->x,button->y,button->w-1,button->h-1);
//LCD_drawLine(lcd,button->x+1,button->y+button->h,button->x+button->w,button->y+button->h);
fillRect(lcd,button->x+button->w/2-length(button->text)*6/2,button->y+button->h/2-4,length(button->text)*6,8);

pixelSetOrClearOrInvert(lcd,SET_PIXELS);
drawRect(lcd,button->x+1,button->y+1,button->w-1,button->h-1);
printfStr(lcd,button->text,button->x+button->w/2-length(button->text)*6/2+1,button->y+button->h/2-4+1);
}else {
pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
drawRect(lcd,button->x+1,button->y+1,button->w-1,button->h-1);
fillRect(lcd,button->x+button->w/2-length(button->text)*6/2+1,button->y+button->h/2-4,length(button->text)*6,8);

pixelSetOrClearOrInvert(lcd,SET_PIXELS);
drawRect(lcd,button->x,button->y,button->w-1,button->h-1);
LCD_drawLine(lcd,button->x+1,button->y+button->h,button->x+button->w,button->y+button->h);
LCD_drawLine(lcd,button->x+button->w,button->y+1,button->x+button->w,button->y+button->h);
printfStr(lcd,button->text,button->x+button->w/2-length(button->text)*6/2,button->y+button->h/2-4);

}
//drawRect(lcd,button->x,button->y,button->w,button->h);
//printfInt(lcd,button->down,button->x+5,button->y+2);



//printfInt(lcd,length(button->text),button->x+2,button->y+20);

}


char insideButton(Button *button,int x,int y){

return (button->x<x && button->x+button->w>x && button->y<y && button->y+button->h>y );
}

char insideMenyButton(MenyButton *button,int x,int y){

return (button->x<x && button->x+button->w>x && button->y<y && button->y+button->h>y );
}




//Meny
MenyButton makeButton(char *text,int id){
MenyButton MB;
MB.text=text;
MB.x=0;
MB.y=0;
MB.w=0;
MB.h=0;
MB.id=id;
MB.down=0;
MB.next=0;
MB.text_pos=0;
return MB;
}

MenyButton creatButton(char *text,int id,int x,int y,int w, int h){
MenyButton MB;
MB.text=text;
MB.x=x;
MB.y=y;
MB.w=w;
MB.h=h;
MB.id=id;
MB.down=0;
MB.next=0;
MB.text_pos=length(MB.text)*6/2;
return MB;
}


int Buttons_meny(LCD_engine *lcd,ButtonsMeny *BM,int rows){

MenyButton *button=BM->RootButton;
int maxW=0;
	int number_of_buttons=0;
	do{
	if (length(button->text)>maxW){
		maxW=length(button->text);
	}

	number_of_buttons++;
}while ((button=button->next));


button=BM->RootButton;
maxW=maxW*6+8;
//if (maxW*number_of_buttons>120){//two rows


//}else {
if (rows>number_of_buttons)
	rows=number_of_buttons;

int rowB[rows];
rowB[0]=(number_of_buttons/rows);
int bLeft=number_of_buttons;
for (int i=1;i<rows;i++){
rowB[i]=((bLeft-rowB[i-1])/(rows-i));
bLeft-=rowB[i-1];
}

maxW=120/(number_of_buttons/rows);

//}


int i=0;
int r=0;
int buttons_on_row=rowB[0];
do{
button->w=maxW-1;
button->h=50/rows;
button->x=i*(maxW+1)+(120-maxW*buttons_on_row)/2+3;
button->y=9+r*54/rows;
button->down=0;
button->text_pos=length(button->text)*6/2;
i++;

if (i>=(buttons_on_row)){
r++;
buttons_on_row=rowB[r];
maxW=120/((buttons_on_row));
i=0;
}
}while ((button=button->next));



return Buttons_meny_private(lcd,BM);
}


int Buttons_meny_private(LCD_engine *lcd,ButtonsMeny *BM){
	//LCD_CLR();
	LCD_RAM_CLR(lcd);
	//resetConnection();
	//setDubbelbuffer(lcd,1);
	pixelSetOrClearOrInvert(lcd,SET_PIXELS);
	//resetConnection();
	Point p;
	Point old_points[3];

	char useTouch=1;
	int activeButton=0;
	MenyButton *button=BM->RootButton;
	char number_of_buttons=0;
	do{
	button->down=0;
	number_of_buttons++;
}while ((button=button->next));



for (int i=0;i<3;i++){
		old_points[i].x=0;
		old_points[i].y=0;
		old_points[i].down_count=0;
	}

	printfStr(lcd,BM->title,64-length(BM->title)*6/2,1);
	redrawRAM(lcd);

while (1){

if  (START_isPressedAndRel()){
return -1;
}

if (TOUCH_STATE==TOUCH_STATE_PRESSED){
useTouch=1;
p=getTouch();
if (p.down_count>2){
if (p.down_count!=old_points[0].down_count){
//save old
for (int i=2;i>0;i--){
old_points[i].x=old_points[i-1].x;
old_points[i].y=old_points[i-1].y;
}
old_points[0].x=p.x;
old_points[0].y=p.y;
old_points[0].down_count=p.down_count;
}

//LCD_setPage(0);
//LCD_setColumn(0);
//rprintfStr("tuching");
MenyButton *button=BM->RootButton;
do{
if (insideMenyButton(button,old_points[1].x,old_points[1].y)){
button->down=1;
}else{
button->down=0;

}
}while ((button=button->next));

}else {
MenyButton *button=BM->RootButton;
	do{

button->down=0;

}while ((button=button->next));

	
	
	}

}else if (useTouch){
//printfStr(lcd,"x",p.x,p.y);
//printfStr(lcd,"o",p2.x,p2.y);
//for (int i=0;i<4;i++){


//}

//LCD_drawPixel(lcd,old_points[2].x,old_points[2].y);

MenyButton *button=BM->RootButton;
do{
	if (insideMenyButton(button,old_points[2].x,old_points[2].y)){
		return button->id;

		}else {
			button->down=0;
		}
	//button->down=0;
}while ((button=button->next));

for (int i=0;i<3;i++){
		old_points[i].x=0;
		old_points[i].y=0;
		old_points[i].down_count=0;
	}

}

if (useTouch && any_isPressed()){
useTouch=0;

}


if (!useTouch){//Using buttons too navigate

if (UP_isPressedAndRel() || LEFT_isPressedAndRel()){
activeButton--;
if (activeButton<0){

activeButton=number_of_buttons-1;

}
}else if (DOWN_isPressedAndRel() || RIGHT_isPressedAndRel()){
activeButton++;
if (activeButton>=number_of_buttons){

activeButton=0;

}
}else if (B_isPressedAndRel()){
activeButton=0;
}

if (A_isPressedAndRel()){
MenyButton *button=BM->RootButton;
do{
	
		if (button->down){
			return button->id;
		}
	
}while ((button=button->next));

}


MenyButton *button=BM->RootButton;
char i=0;
do{
	if (activeButton==i){
		button->down=1;
	}else{
		button->down=0;
	}
	i++;
}while ((button=button->next));
	
	
	
	}

MenyButton *button=BM->RootButton;
do{
	drawMenyButton(lcd,button);
}while ((button=button->next));

//LCD_drawPixel(lcd,old_points[2].x,old_points[2].y);
pushRAM(lcd);
}


}



int gui_meny_demo(LCD_engine *lcd){
LCD_CLR();
LCD_RAM_CLR(lcd);
resetConnection();
setDubbelbuffer(lcd,1);
pixelSetOrClearOrInvert(lcd,SET_PIXELS);
resetConnection();
LCD_setPage(0);
LCD_setColumn(0);
rprintfStr("meny");

ButtonsMeny BMA;
BMA.title="hello";
BMA.RootButton=0;
MenyButton b[9];
b[0]=makeButton("1",1);
b[1]=makeButton("2",2);
b[2]=makeButton("3",3);
b[3]=makeButton("4",4);
b[4]=makeButton("5",5);
b[5]=makeButton("6",6);
b[6]=makeButton("7",7);
b[7]=makeButton("8",8);
b[8]=makeButton("9",9);
addMenyButton(&BMA, &b[0]);
addMenyButton(&BMA, &b[1]);
int valde=0;
int add=2;

while (1){

valde = Buttons_meny(lcd,&BMA,4);
if (valde==9){
return 0;
}
char str[10]; 
BMA.title=itoa(valde, str, 10);
if (add<9){
if (add==8){
b[add].text="exit";
}

addMenyButton(&BMA, &b[add]);
add++;
}

}
return 0;
}
