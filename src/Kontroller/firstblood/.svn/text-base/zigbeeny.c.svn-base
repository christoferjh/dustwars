#include <avr/io.h> 
		#include <avr/interrupt.h> 
		#include <util/delay.h>


		#include "zigbeeny.h"


		int zigbee_init (void) 
		{ 





		DDRB=0xFF; // C direction out
		PORTB=0xFF; //Port C high

		UCSR0A = 0x00; // Clear any previous RXC/TXC flags
		   UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   //  TX and RX enable
		   UCSR0C |=  (1 << UCSZ00) | (1 << UCSZ01); // 8-bits in a byte 

		   UBRR0L = BAUD_PRESCALE; // Low byte of the baud prescale 
		   UBRR0H = (BAUD_PRESCALE >> 8); // High byte of the baud prescale

		   UCSR0B |= (1 << RXCIE0); // Enable the USART Recieve Complete interrupt
		   sei(); // Enable global interrupts
		   //SREG|=1<<7;

		rxByteCount=0; //Set variables to zero as a starting value
		rxPacketSize=0;
		latest_from_zigge_byte=255;
		return 0;
		}
		
		void sendZigByte(unsigned char id,unsigned char byte){

			unsigned char gensend[8]={7,0,2,0,0,208,id,byte};
			txPacket(gensend,7);

		}
		
		
		void ledTest(){
		



//_delay_ms(1000);


	//	txPacket(ping, 5);
unsigned char i;
i=0;
			int	pos=1;
				unsigned char gensend[7];// Generic send packet
				gensend[pos++]=0;
				gensend[pos++]=2;
				gensend[pos++]=0;
				gensend[pos++]=0;
				gensend[pos++]=208;
				gensend[pos++]=i;
				gensend[0]=pos-1;




		   for (;;) // Loop forever 
		   {

			
					_delay_ms(3000);
				txPacket(gensend,6); 
				//ProcessPacket();
				i++;
				gensend[6]=i;

		}

} 




		ISR(USART0_RX_vect) //Interrupt for received bytes
		{

		
unread_from_zigge++;
			if(rxByteCount==0){
				rxPacketSize=UDR0; rxBuffer[rxByteCount]=rxPacketSize; //The first byte sets the length of the rest (stored in a buffer)
				}
			if(rxByteCount <= rxPacketSize && rxByteCount != 0){rxBuffer[rxByteCount]=UDR0;}// Store the rest of the bytes

		rxByteCount++;


			if (rxByteCount>rxPacketSize){ rxByteCount=0; //full packet resets the counter


		
			} 

   latest_from_zigge_id=rxBuffer[6];
			latest_from_zigge_byte=rxBuffer[7];
		}


		void ProcessPacket(void) {
			PORTB=rxBuffer[6];
		

		}

		void txPacket(unsigned char * Packet, char txSize) {
			unsigned char txByteNr=0;
	
	
			while (txByteNr <= txSize) { // One byte at a time
		
		
		

		
				while (!(UCSR0A & (1<<UDRE0))) {} // transmit when USART is ready
	
		
				UDR0 = Packet[txByteNr]; //transmit byte

		
				txByteNr++;	
		
			}
		}
int get_unread_from_zigge(){
return unread_from_zigge;
}

int get_latest_from_zigge_byte(){
return latest_from_zigge_byte;
}

int get_latest_from_zigge_id(){
return latest_from_zigge_id;
}
