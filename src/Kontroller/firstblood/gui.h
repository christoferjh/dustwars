#ifndef Oo_GUI_H_oO
#define Oo_GUI_H_oO
//A button...
typedef struct Button{
int x,y,w,h;
char *text;
char down;
}Button;
 
typedef struct MenyButton MenyButton;
//A button used in a meny, not the most elegant solution but good enaugh
struct MenyButton{
int x,y,w,h;
char *text;
char down;
char id;
char text_pos;
MenyButton *next; //This makes it a likned list
};
//A Meny full with buttons
typedef struct ButtonsMeny{
MenyButton *RootButton;
char *title;
unsigned char number_of_buttons; //Number of buttons
}ButtonsMeny;

void drawMenyButton(LCD_engine *lcd,MenyButton *button);
void addMenyButton(ButtonsMeny *BM, MenyButton *newMenyButton);
void drawButton(LCD_engine *lcd,Button *button);
char insideButton(Button *button,int x,int y);
char insideMenyButton(MenyButton *button,int x,int y);
int Buttons_meny(LCD_engine *lcd,TouchCalibration *TC,ButtonsMeny *BM,int rows);
MenyButton creatButton(char *text,int id,int x,int y,int w, int h);
MenyButton makeButton(char *text,int id);
int gui_meny_demo(LCD_engine *lcd,TouchCalibration *TC);

#endif
