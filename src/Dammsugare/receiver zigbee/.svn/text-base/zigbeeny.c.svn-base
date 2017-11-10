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
		   //sei(); // Enable global interrupts
		   SREG|=1<<7;

		rxByteCount=0; //Set variables to zero as a starting value
		rxPacketSize=0;
		
		
		
				unsigned char ping[6]; //ping the remote transceiver
		
				int pos=1;

				ping[pos++]=0;
				ping[pos++]=1;
				ping[pos++]=0;
				ping[pos++]=2;
				ping[pos++]=160;
				ping[0]=pos-1;



//_delay_ms(1000);


	//	txPacket(ping, 5);


				pos=1;
				unsigned char gensend[7];// Generic send packet
				gensend[pos++]=0;
				gensend[pos++]=1;
				gensend[pos++]=0;
				gensend[pos++]=2;
				gensend[pos++]=208;
				gensend[pos++]=0;
				gensend[0]=pos-1;




		   for (;;) // Loop forever 
		   {

			
					_delay_ms(3000);
				//txPacket(gensend,6); 
				ProcessPacket();




		}
return 0;	} 

		ISR(USART0_RX_vect) //Interrupt for received bytes
		{

		

			if(rxByteCount==0){
				rxPacketSize=UDR0; rxBuffer[rxByteCount]=rxPacketSize; //The first byte sets the length of the rest (stored in a buffer)
				}
			if(rxByteCount <= rxPacketSize && rxByteCount != 0){rxBuffer[rxByteCount]=UDR0;}// Store the rest of the bytes

		rxByteCount++;


			if (rxByteCount>rxPacketSize){ rxByteCount=0; //full packet resets the counter


		
			} 

   
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
