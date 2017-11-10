#ifndef Oo_ZIGBEENY_H_oO
#define Oo_ZIGBEENY_H_oO

void txPacket(unsigned char * Packet, char txSize) ;
void ProcessPacket(void);
int zigbee_init(void);

		#define FOSC 8000000UL


		#define USART_BAUDRATE 9600 
		#define BAUD_PRESCALE (((FOSC / (USART_BAUDRATE * 16UL))) - 1) 
		#define BUFFER_SIZE 128


		volatile unsigned char rxBuffer[BUFFER_SIZE]; // A text buffer for incoming text 
		volatile unsigned char rxByteCount; // A byte counter for the incoming text 
		volatile unsigned char rxPacketSize; // Used to identify the size of the packet (first byte)
	
int latest_from_zigge_byte;
int latest_from_zigge_id;
int unread_from_zigge;
int get_unread_from_zigge();
int get_latest_from_zigge_byte();
int get_latest_from_zigge_id();
void sendZigByte(unsigned char id,unsigned char byte);
#endif
