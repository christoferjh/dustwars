#ifndef Oo_CONNECTIONS_H_oO
#define Oo_CONNECTIONS_H_oO


//***************************************************
//				Connections for ATMega1284p
//***************************************************
//----------------------------------------------------
//					LCD
//----------------------------------------------------
#define DD_MOSI 	DDB5 	//Master out Data Direction pin
#define DDR_SPI 	DDRB 	//SPI on port B
#define DD_SCK 		DDB7 	//Serial clock pin
#define DD_CS 		DDB4	//Chip Select pin
#define DD_A0 		DDB2	//Chip Select pin

#define DD_RE 		DDB3	//Chip Select pin
#define OUT_CS 		PORTB4	//Chip Select output out pin
#define PORT_CS 	PORTB	//Chip Select Port data register
#define PORT_RE 	PORTB	//Reset Port data register
#define PORT_A0 	PORTB	//Reset Port data register
#define OUT_A0 		PORTB2	//Reset Port data register
#define OUT_RE		PORTB3	//Reset output out pin
//				Buttons
#define DD_BUTTONS 	DDRD //Buttons port
#define P_BUTTONS	PORTD
#define R_BUTTONS 	PIND //Buttons port
#define B_UP		PIND5 //Button up
#define B_DOWN		PIND3 //Button up
#define B_LEFT		PIND4 //Button left
#define B_RIGHT		PIND2 //Button right
#define B_A			PIND0 //Button A
#define B_B			PIND1 //Button B
#define B_START		PIND6 //Button B
//----------------------------------------------------
//					Touch
//----------------------------------------------------

/*
(AL) = Active Low
 

***************************************************
				Connection descriptions
***************************************************
DOGL				 |		ATMEGA16
---------------------------------------------------
36 Serial data in 	<->		MOSI PB5
---------------------------------------------------
37 Serial clock 	<->		Serial clock PB7
---------------------------------------------------
40 Chip Select (AL)	<->		Binary out, PB4, set as out manualy, DDB4 = 1 in DDRB
---------------------------------------------------
39 Reset (AL)		<->		Binary out, PB3, set as out manualy, DDB3 = 1 in DDRB
---------------------------------------------------
38 A0 				<->		Command or Data PB2, set as out manualy, DDB2 = 1 in DDRB

*/
#endif
