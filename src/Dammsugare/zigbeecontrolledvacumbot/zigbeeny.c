#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "zigbeeny.h"

int zigbee_init(void) {

	//DDRB=0xFF; // C direction out
	//PORTB=0xFF; //Port C high
	unread_from_zigge = 1;
	UCSRA = 0x00; // Clear any previous RXC/TXC flags
	UCSRB |= (1 << RXEN) | (1 << TXEN); //  TX and RX enable
	UCSRC |= (1 << UCSZ0) | (1 << UCSZ1); // 8-bits in a byte

	UBRRL = BAUD_PRESCALE; // Low byte of the baud prescale
	UBRRH = (BAUD_PRESCALE >> 8); // High byte of the baud prescale

	UCSRB |= (1 << RXCIE); // Enable the USART Recieve Complete interrupt
	sei(); // Enable global interrupts
	//SREG|=1<<7;

	rxByteCount = 0; //Set variables to zero as a starting value
	rxPacketSize = 0;

	unsigned char ping[6]; //ping the remote transceiver

	int pos = 1;

	ping[pos++] = 0;
	ping[pos++] = 1;
	ping[pos++] = 0;
	ping[pos++] = 2;
	ping[pos++] = 160;
	ping[0] = pos - 1;

	pos = 1;
	unsigned char gensend[7];// Generic send packet
	gensend[pos++] = 0;
	gensend[pos++] = 1;
	gensend[pos++] = 0;
	gensend[pos++] = 2;
	gensend[pos++] = 208;
	gensend[pos++] = 0;
	gensend[0] = pos - 1;

	return 0;
}

int get_unread_from_zigge() {
	return unread_from_zigge;
}

int get_latest_from_zigge_byte() {
	return latest_from_zigge_byte;
}

int get_latest_from_zigge_id() {
	return latest_from_zigge_id;
}

void sendZigByte(unsigned char id, unsigned char byte) {

	unsigned char gensend[8] = { 7, 0, 1, 0, 0, 208, id, byte };
	txPacket(gensend, 7);

}

ISR(USART_RXC_vect) //Interrupt for received bytes
{

	unread_from_zigge++;

	if(rxByteCount==0) {
		rxPacketSize=UDR; rxBuffer[rxByteCount]=rxPacketSize; //The first byte sets the length of the rest (stored in a buffer)
	}
	if(rxByteCount <= rxPacketSize && rxByteCount != 0) {rxBuffer[rxByteCount]=UDR;}// Store the rest of the bytes

	rxByteCount++;

	if (rxByteCount>rxPacketSize) {rxByteCount=0; //full packet resets the counter


	}

	latest_from_zigge_id=rxBuffer[6];
	latest_from_zigge_byte=rxBuffer[7];
}

void ProcessPacket(void) {

}

void txPacket(unsigned char * Packet, char txSize) {
	unsigned char txByteNr = 0;

	while (txByteNr <= txSize) { // One byte at a time

		while (!(UCSRA & (1 << UDRE))) {
		} // transmit when USART is ready

		UDR = Packet[txByteNr]; //transmit byte


		txByteNr++;

	}
}
