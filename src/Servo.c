#include <avr/io.h>
#define SLOW_A 15
#define SLOW_B 24

//h3j
int main()
{
ICR1=20000;
//ICF1=20000;
DDRD=0xff;
TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(1<<COM1B0)|(1<<COM1B1)| (0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);
TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);



//OCR1B=18260; //Stands still #0
//OCR1A=1840; //Stands still #6

OCR1B=18260-SLOW_B; //Slow #0
OCR1A=1840-SLOW_A; //Slow #6

//OCR1A=1840;

while (1){}

} 


