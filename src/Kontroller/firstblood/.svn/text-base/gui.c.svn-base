#include "touch.h"
#include "gui.h"
#include "lcd_engine.h"
#include <stdlib.h>
#include "macros.h"
#include "buttons.h"
#include "debug.h"
//Declare function
int Buttons_meny_private(LCD_engine *lcd, TouchCalibration *TC,
				ButtonsMeny *BM);

//Return length of the string
int length(char str[]) {
	int l = 0;
	if (!str)
		return -1;
	// check the string until a null-terminator
	while (*str) {
		*str++;
		l++;
	}
	return l;
}

//Wrapper to add a button to the meny
void addMenyButton(ButtonsMeny *BM, MenyButton *newMenyButton) {
	//MenyButton
	if (BM->RootButton == 0) {
		//if (BM->number_of_buttons==0){
		newMenyButton->next = 0;
		BM->RootButton = newMenyButton;
	} else {
		newMenyButton->next = BM->RootButton;
		BM->RootButton = newMenyButton;
	}
	//BM->number_of_buttons++;

}

//Draw the menybutton
void drawMenyButton(LCD_engine *lcd, MenyButton *button) {
	//Button is down
	if (button->down) {
		pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
		drawRect(lcd, button->x, button->y, button->w - 1, button->h - 1);
		fillRect(lcd, button->x + button->w / 2 - button->text_pos, button->y
				+ button->h / 2 - 4, button->text_pos * 2, 8);

		pixelSetOrClearOrInvert(lcd, SET_PIXELS);
		drawRect(lcd, button->x + 1, button->y + 1, button->w - 1, button->h
				- 1);
		printfStr(lcd, button->text, button->x + button->w / 2
				- button->text_pos + 1, button->y + button->h / 2 - 4 + 1);
	} else {//Button is up
		pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
		drawRect(lcd, button->x + 1, button->y + 1, button->w - 1, button->h
				- 1);
		fillRect(lcd, button->x + button->w / 2 - button->text_pos + 1,
				button->y + button->h / 2 - 4, button->text_pos * 2, 8);

		pixelSetOrClearOrInvert(lcd, SET_PIXELS);
		drawRect(lcd, button->x, button->y, button->w - 1, button->h - 1);
		LCD_drawLine(lcd, button->x + 1, button->y + button->h, button->x
				+ button->w, button->y + button->h);
		LCD_drawLine(lcd, button->x + button->w, button->y + 1, button->x
				+ button->w, button->y + button->h);
		printfStr(lcd, button->text, button->x + button->w / 2
				- button->text_pos, button->y + button->h / 2 - 4);
	}
}

//Draw a button
void drawButton(LCD_engine *lcd, Button *button) {

	if (button->down) {
		pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
		drawRect(lcd, button->x, button->y, button->w - 1, button->h - 1);
		fillRect(lcd, button->x + button->w / 2 - length(button->text) * 6 / 2,
				button->y + button->h / 2 - 4, length(button->text) * 6, 8);

		pixelSetOrClearOrInvert(lcd, SET_PIXELS);
		drawRect(lcd, button->x + 1, button->y + 1, button->w - 1, button->h
				- 1);
		printfStr(lcd, button->text, button->x + button->w / 2 - length(
				button->text) * 6 / 2 + 1, button->y + button->h / 2 - 4 + 1);
	} else {
		pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
		drawRect(lcd, button->x + 1, button->y + 1, button->w - 1, button->h
				- 1);
		fillRect(lcd, button->x + button->w / 2 - length(button->text) * 6 / 2
				+ 1, button->y + button->h / 2 - 4, length(button->text) * 6, 8);

		pixelSetOrClearOrInvert(lcd, SET_PIXELS);
		drawRect(lcd, button->x, button->y, button->w - 1, button->h - 1);
		LCD_drawLine(lcd, button->x + 1, button->y + button->h, button->x
				+ button->w, button->y + button->h);
		LCD_drawLine(lcd, button->x + button->w, button->y + 1, button->x
				+ button->w, button->y + button->h);
		printfStr(lcd, button->text, button->x + button->w / 2 - length(
				button->text) * 6 / 2, button->y + button->h / 2 - 4);
	}
}


//Check if a koordinate is inside the button
char insideButton(Button *button, int x, int y) {

	return (button->x < x && button->x + button->w > x && button->y < y
			&& button->y + button->h > y);
}
//Check if a koordinate is inside the menybutton
char insideMenyButton(MenyButton *button, int x, int y) {

	return (button->x < x && button->x + button->w > x && button->y < y
			&& button->y + button->h > y);
}

//Meny
MenyButton makeButton(char *text, int id) {
	MenyButton MB;
	MB.text = text;
	//Position
	MB.x = 0;
	MB.y = 0;
	//Size
	MB.w = 0;
	MB.h = 0;
	//Id, returned when klicked
	MB.id = id;
	MB.down = 0;
	//Next, its a linked list
	MB.next = 0;
	//Pre calculated to increase speed
	MB.text_pos = 0;
	return MB;
}

//Wrapper to create a Menybutton
MenyButton creatButton(char *text, int id, int x, int y, int w, int h) {
	MenyButton MB;
	MB.text = text;
	MB.x = x;
	MB.y = y;
	MB.w = w;
	MB.h = h;
	MB.id = id;
	MB.down = 0;
	MB.next = 0;
	MB.text_pos = length(MB.text) * 6 / 2; //For speed optimization
	return MB;
}

//This take care of it all, including layout
int Buttons_meny(LCD_engine *lcd, TouchCalibration *TC, ButtonsMeny *BM,
		int rows) {
	//First we check how many buttons there are
	MenyButton *button = BM->RootButton;
	int maxW = 0;
	int number_of_buttons = 0;
	do {
		if (length(button->text) > maxW) {
			maxW = length(button->text);
		}

		number_of_buttons++;
	} while ((button = button->next));

	button = BM->RootButton;
	maxW = maxW * 6 + 8; //Not really used the way it was intended... Space for improvment
	if (rows > number_of_buttons)
		rows = number_of_buttons;
	int rowB[rows]; //Holding each rows number of buttons
	rowB[0] = (number_of_buttons / rows);
	int bLeft = number_of_buttons;
	for (int i = 1; i < rows; i++) {
		rowB[i] = ((bLeft - rowB[i - 1]) / (rows - i));
		bLeft -= rowB[i - 1];
	}

	maxW = 120 / (number_of_buttons / rows); //Fill the screen with the buttons

	//Layout the buttons
	int i = 0;
	int r = 0;
	int buttons_on_row = rowB[0];
	do {
		button->w = maxW - 1;
		button->h = 50 / rows;
		button->x = i * (maxW + 1) + (120 - maxW * buttons_on_row) / 2 + 3;
		button->y = 9 + r * 54 / rows;
		button->down = 0;
		button->text_pos = length(button->text) * 6 / 2;
		i++;

		if (i >= (buttons_on_row)) {
			r++;
			buttons_on_row = rowB[r];
			maxW = 120 / ((buttons_on_row));
			i = 0;
		}
	} while ((button = button->next));

	//Then return the choosen button
	return Buttons_meny_private(lcd, TC, BM);
}

int Buttons_meny_private(LCD_engine *lcd, TouchCalibration *TC, ButtonsMeny *BM) {
	LCD_RAM_CLR(lcd);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	Point p;
	Point old_points[3];

	char useTouch = 1; //Using touch to start with
	int activeButton = 0; //For when using the controller

	//Checking how many buttons, again. Maby we should beleve the MenyButton's variable number_of_buttons. But it is not
	//demanded by the creator of the 'MenyButton' to use our wrapper that calculates that when doing layout. So here it is again.
	MenyButton *button = BM->RootButton;
	char number_of_buttons = 0;
	do {
		button->down = 0;
		number_of_buttons++;
	} while ((button = button->next));

	//Reset old touch points
	for (int i = 0; i < 3; i++) {
		old_points[i].x = 0;
		old_points[i].y = 0;
		old_points[i].down_count = 0;
	}
	//Print the title
	printfStr(lcd, BM->title, 64 - length(BM->title) * 6 / 2, 1);
	redrawRAM(lcd); //Redraw the buffer

	while (1) {


		
		
		if (START_isPressedAndRel()) { //Exit, nothing was pressed
			check_debug(lcd,TC);
			return -1;
		}



		if (TOUCH_STATE == TOUCH_STATE_PRESSED) {
			useTouch = 1;
			p = getTouch(TC);
			if (p.down_count > 2) {//Accurat touch enaugh
				if (p.down_count != old_points[0].down_count) { //Its a new point
				//save old
					for (int i = 2; i > 0; i--) {
						old_points[i].x = old_points[i - 1].x;
						old_points[i].y = old_points[i - 1].y;
					}
					old_points[0].x = p.x;
					old_points[0].y = p.y;
					old_points[0].down_count = p.down_count;
				}

				//Check which button is pressed down
				MenyButton *button = BM->RootButton;
				do {
					if (insideMenyButton(button, old_points[1].x,
							old_points[1].y)) {
						button->down = 1;
					} else {
						button->down = 0;

					}
				} while ((button = button->next));

			} else {
				//well, reset
				MenyButton *button = BM->RootButton;
				do {

					button->down = 0;

				} while ((button = button->next));

			}

		} else if (useTouch) {

			//Check if the last accurate touch point was insde a button, if so, return the id
			MenyButton *button = BM->RootButton;
			do {
				if (insideMenyButton(button, old_points[2].x, old_points[2].y)) {
					return button->id;

				} else {
					button->down = 0;
				}
				//button->down=0;
			} while ((button = button->next));
			//Reset old points
			for (int i = 0; i < 3; i++) {
				old_points[i].x = 0;
				old_points[i].y = 0;
				old_points[i].down_count = 0;
			}

		}


		//CHANGE THIS!!!! PRONTO!!! ENTERNAL LOOPS, some of the buttons on C is always pressed?!
		if (useTouch && any_isPressed()) {//Switch over to use controller
			useTouch = 0;

		}

		if (!useTouch) {//Using controllers buttons too navigate

			if (UP_isPressedAndRel() || LEFT_isPressedAndRel()) {//Move back in the linked list
				activeButton--;
				if (activeButton < 0) {
					activeButton = number_of_buttons - 1;
				}
			} else if (DOWN_isPressedAndRel() || RIGHT_isPressedAndRel()) {//Move forward in the linked list
				activeButton++;
				if (activeButton >= number_of_buttons) {

					activeButton = 0;

				}
			} else if (B_isPressedAndRel()) {//Move to first button
				activeButton = 0;
			}

			if (A_isPressedAndRel()) {//We have a winner!
				MenyButton *button = BM->RootButton;
				do {
					if (button->down) {
						return button->id; //Return it, this is the same as 'activeButton'
					}

				} while ((button = button->next));

			}

			//Show which button that is selected at the time 
			MenyButton *button = BM->RootButton;
			char i = 0;
			do {
				if (activeButton == i) {
					button->down = 1;
				} else {
					button->down = 0;
				}
				i++;
			} while ((button = button->next));

		}
		//Draw the buttons
		MenyButton *button = BM->RootButton;
		do {
			drawMenyButton(lcd, button);
		} while ((button = button->next));

		//LCD_drawPixel(lcd,old_points[2].x,old_points[2].y); //For debugging, too see if the point is accurate
		pushRAM(lcd); //Draw to screen
	}

}

//This is the meny demo, showing difrent layouts, but not all there is
int gui_meny_demo(LCD_engine *lcd, TouchCalibration *TC) {
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();
	LCD_setPage(0);
	LCD_setColumn(0);
	rprintfStr("meny");

	ButtonsMeny BMA;
	BMA.title = "hello";
	BMA.RootButton = 0;
	MenyButton b[9];
	b[0] = makeButton("1", 1);
	b[1] = makeButton("2", 2);
	b[2] = makeButton("3", 3);
	b[3] = makeButton("4", 4);
	b[4] = makeButton("5", 5);
	b[5] = makeButton("6", 6);
	b[6] = makeButton("7", 7);
	b[7] = makeButton("8", 8);
	b[8] = makeButton("9", 9);
	addMenyButton(&BMA, &b[0]);
	addMenyButton(&BMA, &b[1]);
	int valde = 0;
	int add = 2;

	while (1) {

		valde = Buttons_meny(lcd, TC, &BMA, 4);
		if (valde == 9) {
			return 0;
		}
		char str[10];
		BMA.title = itoa(valde, str, 10);
		if (add < 9) {
			if (add == 8) {
				b[add].text = "exit";
			}

			addMenyButton(&BMA, &b[add]);
			add++;
		}

	}
	return 0;
}
