#include <avr/io.h>
#include <util/delay.h>

#define ACC_PORT_CS 		PORTD7
#define ACC_DD 				DDRD
#define ACC_PIN 			PIND
#define ACC_PORT 			PORTD
#define ACC_PORT_IO 		PORTD6
#define ACC_PIN_IO 			PIND6
#define ACC_DD_IO 			DDD6
#define ACC_PORT_CLK 		PORTD5

void CLKHI(){
ACC_PORT|=(1<<ACC_PORT_CLK);
}

void CLKLO(){
ACC_PORT&=~(1<<ACC_PORT_CLK);
}

void clock(){
	CLKLO();
	_delay_us(10);
	CLKHI();
	_delay_us(10);
}

void CSHI(){
	ACC_PORT|=(1<<ACC_PORT_CS);
}

void CSLO(){
	ACC_PORT&=~(1<<ACC_PORT_CS);
}

void DIOOUT(){
ACC_DD|=(1<<ACC_DD_IO);
}

void DIOIN(){
ACC_DD&=~(1<<ACC_DD_IO);
}

void DIOHI(){
ACC_PORT|=(1<<ACC_PORT_IO);
}

void DIOLO(){
ACC_PORT&=~(1<<ACC_PORT_IO);
}

void init_acc(){
	DDRD=0xFF;
	CSHI();
	DIOOUT(); //DONT NEED, ALL IS ALREADY OUT...
}

void shift_out(char axis[5]){
	DIOOUT(); // PC6 sätts som utgång
		for (int i=0; i<5; i++){ 
		
			if (axis[i]==1){
				DIOHI();
			}
			else {
				DIOLO();
			}
				clock();
		}
}

int Bin2Dec(char binary[12])
{
     int total = 0;
     int k = 1;
     

     
     for(int i=11;i>=0;i--)
     {
          total += binary[i]*k;
          k = k * 2;
          }

     return total;
}

int shift_in(){
	DIOIN(); // PC6 förbereds för att ta emot information = Sätts till ingång
	char ad_binary[12];
	int ad_decimal;
	clock(); // Startbit
	clock(); // Nullbit

		for (int i=0; i<12; i++){
			clock();
			ad_binary[i]=((ACC_PIN&(1<<ACC_PIN_IO))); // Talet som kommer in läses av på PINC6
		}
	ad_decimal=Bin2Dec(ad_binary);
	return ad_decimal;
}

int degrees_x(int decimal){
	int x;
	int grader_x;
	x = decimal;

	grader_x=(x-2053)/5.06667; // Ekvation för att beräkna grader från ADC resultatet

		//Vi vill ha negativa vinklar...
		//if (grader_x < 0){ // För att undvika negativa vinklar
		//	grader_x = grader_x*(-1);
		//}

	return grader_x;
}

int getAccFromVect(char axis[]){
	CSLO(); // Aktivera AD-omvandlingen

	shift_out(axis);
	int	ad_value=shift_in();
	ad_value=degrees_x(ad_value);

	CSHI(); // Avaktivera AD-omvandlingen
	return ad_value;
}

int getAccX(){

	char x_axis[5]={1,1,0,0,0};

return getAccFromVect(x_axis);
}

int getAccY(){
char y_axis[5]={1,1,0,0,1};

return getAccFromVect(y_axis);
}

int getAccZ(){

	char z_axis[5]={1,1,0,1,0};

return getAccFromVect(z_axis);
}
