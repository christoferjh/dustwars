
#include "CAN_macro.h"

/*!int diodlist[] = {0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x01,0x00};
 unsigned int i = 0;*/

void snd(uint8_t byte) {
	SPDR = byte;
	while (!(SPSR & (1 << SPIF)))
		;
}

ISR(SPI_STC_vect)
{
	/*!
	 dataRec[x] = SPDR;
	 x++;
	 if (x==7)x=0;
	 */

}

void MCP_BitModify(uint8_t adress, uint8_t mask, uint8_t data) {
	CS_low;
	snd(MCP_ModifyBit);
	snd(adress);
	snd(mask);
	snd(data);
	CS_high;
}

void can_init(void) {
	DDRB = (1 << DDB5) | (1 << DDB7) | (1 << DDB4); //|MOSI,Clock & SS set to outputs
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPIE); //|Enable SPI, master, SPI interrupt

	CS_high;

	//      Resets the MCP
	//----------------------------------------------
	CS_low;
	dl;
	snd(MCP_reset);
	dl;
	CS_high;

	//      Set the Tx-register settings
	//---------------------------------------------------------------------------------
	CS_low;
	snd(loadTx0_SIDH);
	snd(0x00);
	snd(0x00);
	CS_high;
	dl;

	CS_low;
	snd(loadTx1_SIDH);
	snd(0x00);
	snd(0x00);
	CS_high;
	dl;

	CS_low;
	snd(loadTx2_SIDH);
	snd(0x00);
	snd(0x00);
	CS_high;
	dl;

	//      Set the Rx-register settings
	//-----------------------------------------------------------------------------------

	MCP_BitModify(RXB0CTRL, 0x64, 0x64); //Setting the buffer to accept all messages, (mask, filter, rollover mode)


	//      Sets the MCP CAN bit timing
	//----------------------------------------------

	MCP_BitModify(CNF1, 0xFF, kvaser_CNF1);

	MCP_BitModify(CNF2, 0xFF, kvaser_CNF2);

	MCP_BitModify(CNF3, 0x01, kvaser_CNF3);

	//      Set the operating mode to normal
	//----------------------------------------------

	MCP_BitModify(CANCTRL, 0xFF, 0x00);

	//      Initialize all interrupts
	//-----------------------------------------------------------

	MCP_BitModify(CANINTE, 0xFF, 0xFF);

	sei();

}

uint8_t check_MCP_interrupts(void) {
	CS_low;
	snd(MCP_read);
	snd(CANINTF);
	snd(dontCare);
	CS_high;

	return SPDR;

}

void clear_MCP_interrupts(uint8_t CANINTF_mask) {

	CS_low;
	snd(MCP_ModifyBit);
	snd(CANINTF);
	snd(CANINTF_mask);
	snd(0x00); //Clear all interrupts
	CS_high;
}

void SPI_writeReg(uint8_t adress, uint8_t data) {
	//If data on adress is occupied (use MCP_ReadStatus), write to another buffer
	//---------------------------------------------------------------------------
	CS_low;
	snd(MCP_write);
	snd(adress);
	snd(data);
	CS_high;
}

uint8_t check_TxBuffer(void) {
	//Check if the current TxBuffer is empty ( bit 3 in TXBnCTRL)
	//-----------------------------------------------------------------
	CS_low;
	snd(MCP_readStatus); //Sending readstatus command
	snd(dontCare); //Send don't Care data while simultaniously collecting status on MISO line
	CS_high;
	return SPDR; //Return the latest stored element of data in the dataRec array
}

void load_TxBuffer(uint8_t can_mess) {
	//      Checks how many bytes the frame will contain, clear bit 6 for data frame
	//-------------------------------------------------------------------------------------

	uint8_t dataLength = 0x01;

	uint8_t instruction;

	dataLength &= ~(1 << 6);

	//      Waits until TxBuffer 0 is ready before writing to it
	//-------------------------------------------------------------------------------------

	while ((check_TxBuffer() & 0x04) == 0x04) {
	}
	instruction = 0x41;

	//      Sets DLC register
	//-------------------------------------------------------------------------------------

	SPI_writeReg(TXB0DLC, dataLength);

	//      Loads the buffer data registers with data
	//-----------------------------------------------------------------------

	CS_low;
	snd(instruction);
	snd(can_mess);
	CS_high;

}

void CAN_sendMsg(void) {
	CS_low;
	snd(0x81); //Send buffer 0
	CS_high;
}

void send_can(uint8_t data) {

	load_TxBuffer(data);

	CAN_sendMsg();

	MCP_BitModify(CANINTF, 0x1C, 0x00);

}

