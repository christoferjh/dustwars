#include <avr/io.h>

//hej
//tja
int main(void){
DDRB = 0xff;
TCCR0A |= ((1<<COM0A0) | (1<<COM0B0) | (1<<WGM00));
TCCR0B |= ((1<<WGM02) | (1<<CS00) | (1<<COM0B0) | (1<<COM0B1));


while(1){
OCR0A = 5;
OCR0B = 130;

}

}
