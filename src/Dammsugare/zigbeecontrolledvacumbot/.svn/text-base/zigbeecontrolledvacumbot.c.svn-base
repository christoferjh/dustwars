#include "zigbeeny.h" 
#include "drivning.h"
#include <avr/io.h>
#include "dustwarsIDs.h"
#include "ultraljud.h"
#include "CAN_sender.h"
//#include "lcd.h"

int main(void){

//DDRD=0xff;
//PORTD=0xff;
//while(1){}



zigbee_init();
can_init();

init_drivning();

//lcd_init(LCD_DISP_OFF);
//lcd_init(LCD_DISP_ON);
//lcd_clrscr();

DDRD|=(1<<7);
DDRB=0xFF;
PORTB=0x00;
PORTB=0x55;

//DDRA=0xFF;
//PORTA=0xFF;
//while(1){//PORTB=unread_from_zigge;
//PORTB=get_unread_from_zigge();

//}
//PORTB=0x55;
char L_Motor=0;
char R_Motor=0;
int distance=0;
unsigned char b=0;
unsigned int a=0;
unsigned char sent_distance=0;
char calcs=0;
while (1){
a++;
if (a>40000){
a=0;

b++;
if (b>0){
calcs++;
//DDRC=0xFF;
//PORTC=calcs;
start_ultraljudmatning();
b=0;
} 
int tmp_distance=get_distance();

if (tmp_distance>0){
//lcd_clrscr();
//print("DIST ",tmp_distance,0);
//unsigned char Low=(tmp_distance&0xff);
//unsigned char High=(tmp_distance>>8);
if (tmp_distance<250){
sent_distance=(unsigned char)tmp_distance;
sendZigByte(0x00,sent_distance);
//PORTC=sent_distance;
send_can(sent_distance);
}else if (sent_distance!=255){
sent_distance=255;
sendZigByte(0x00,255);

}
}
}
//if (unread_from_zigge){
//unread_from_zigge=0;

if (get_latest_from_zigge_id()==ID_DRIVE){
L_Motor=get_latest_from_zigge_byte()>>4;
R_Motor=get_latest_from_zigge_byte()&0x0F;
PORTB=get_latest_from_zigge_byte();

if (L_Motor==0){
stop_motors();
//set_motor_A( MOTOR_A_STOP );
}else if (L_Motor==1){
start_motors();
set_motor_A(MOTOR_A_FORWARD);
}else{
start_motors();
set_motor_A(MOTOR_A_BACKWARDS);
}


if (R_Motor==0){
//set_motor_B(MOTOR_B_STOP);
stop_motors();
}else if (R_Motor==1){
start_motors();
set_motor_B(MOTOR_B_FORWARD);
}else{
start_motors();
set_motor_B(MOTOR_B_BACKWARDS);
}
}else if (get_latest_from_zigge_id()==ID_VACUUM){
if (latest_from_zigge_byte){
////Starta suga
PORTD|=(1<<7);
}else {
////sluta suga
PORTD&=~(1<<7);
 }
// }

}
}
}
