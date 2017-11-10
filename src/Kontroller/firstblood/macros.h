#ifndef Oo_MACROS_H_oO
#define Oo_MACROS_H_oO
//***************************************************
//				Macros 
//***************************************************
//Bit manipulation 
#define SetBit(adr, bnr)	( (adr) |=  (1 << (bnr)) )
#define ClrBit(adr, bnr)	( (adr) &= ~(1 << (bnr)) )

//Chip Select enabel and disable LCD
#define	DOGENABLE  ClrBit(PORT_CS, OUT_CS)
#define	DOGDISABLE SetBit(PORT_CS, OUT_CS)

//Sending commands or data to LCD
#define	DOGCOMMAND ClrBit(PORT_A0, OUT_A0)
#define	DOGDATA    SetBit(PORT_A0, OUT_A0)


#endif
 
