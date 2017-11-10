#ifndef CAN_MACRO_H_INCLUDED
#define CAN_MACRO_H_INCLUDED



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

uint8_t x = 0;
volatile int dataRec[8];

#define dontCare        0xFF
#define LEDs            PORTD
#define NO_LED          0xFF
#define I_LED           0xFE
#define II_LED          0xFC
#define III_LED         0xF8
#define IV_LED          0xF0
#define V_LED           0xE0
#define VI_LED          0xC0
#define VII_LED         0x80
#define VIII_LED        0x00

#define I               0x7F
#define II              0x3F
#define III             0x1F
#define IV              0x0F
#define V               0x07
#define VI              0x03
#define VII             0x01
#define VIII            0x00

#define wait            while(!(SPSR & (1<<SPIF)))
#define dly             _delay_ms(5000)
#define dl              _delay_ms(20)
#define dli             _delay_ms(200)


/*
        MCP 2515 SPI Instructions
---------------------------------------------
*/

#define MCP_reset       0xC0
#define MCP_read        0x03
#define MCP_write       0x02
#define MCP_readStatus  0xA0
#define MCP_RXstatus    0xB0
#define MCP_ModifyBit   0x05

/*
        MCP 2515 CAN instructions
---------------------------------------------
*/

#define loadTx0_SIDH    0x40
#define loadTx0_D0      0x41
#define loadTx1_SIDH    0x42
#define loadTx1_D0      0x43
#define loadTx2_SIDH    0x44
#define loadTx2_D0      0x45
#define Tx0send         0x81
#define Tx1send         0x82
#define Tx2send         0x84

#define loadRx0_SIDH
#define loadRx0_D0
#define loadRx1_SIDH
#define loadRx1_D0

#define recBuff_0       0x92
#define recBuff_1       0x96



/*
        MCP CAN register adresses
---------------------------------------------
*/

#define CANINTF         0x2C
#define CNF1            0x2A
#define CNF2            0x29
#define CNF3            0x28
#define CANCTRL         0x0F
#define CANINTE         0x2B

//Tx
#define TXB0SIDL        0x32
#define TXB0SIDH        0x31
#define TXB1SIDL        0x42
#define TXB1SIDH        0x41
#define TXB2SIDL        0x52
#define TXB2SIDH        0x51
#define TXB0DLC         0x35
#define TXB1DLC         0x45
#define TXB2DLC         0x55


//Rx
#define RXB0CTRL        0x60
#define RXB1CTRL        0x70
#define BFPCTRL         0x0C
#define RXB0SIDH        0x61
#define RXB1SIDH        0x71
#define RXB0SIDL        0x62
#define RXB1SIDL        0x72
#define RXB0EID8        0x63
#define RXB1EID8        0x73
#define RXB0EID0        0x64
#define RXB1EID0        0x74
#define RXB0DLC         0x65
#define RXB1DLC         0x75




/*
        MCP CAN Interrupts
----------------------------------------------
*/
#define MERFF           0x80
#define WAKIF           0x40
#define ERRIF           0x20
#define TX2IF           0x10
#define TX1IF           0x08
#define TX0IF           0x04
#define RX1IF           0x02
#define RX0IF           0x01


/*
        MCP CAN Bit timing commands
----------------------------------------------
*/

#define SJW_2TQ             CNF1 |= 0x40
#define TQ_40Fosc           CNF1 |= 0x19
#define set_CNF1            0x44           //Sets TQ 500 ns and SJW to 2TQ


#define PS1_2TQ             CNF2 |= 0x10
#define PropSegment_2TQ     CNF2 |= 0x01
#define set_CNF2            0x2C           //Sets phase1 to 5TQ, PRSEG to 4 TQ

#define kvaser_CNF1         0x41
#define kvaser_CNF2         0x91
#define kvaser_CNF3         0x01

#define CNF1_20             0x53
#define CNF2_20             0x91
#define CNF3_20             0x01

/*
        ATMega16 Ports & Registers
---------------------------------------------
*/

#define CS_low          PORTB &= ~(1<<PORTB4)
#define CS_high         PORTB |= (1<<PORTB4)




#endif // CAN_MACRO_H_INCLUDED
