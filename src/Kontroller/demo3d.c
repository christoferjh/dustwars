#include "touch.h"
#include "gui.h"
#include "lcd_engine.h"
#include "demo3d.h"
#include "macros.h"
#include "buttons.h"
#include <math.h>

//Point in 2d
typedef struct Point2d {
	int x, y;
} Point2d;

//Point in 3d
typedef struct Point3d {
	int x, y, z;

} Point3d;

//Vertice (edge/point), holds its point in 3d and the projectet 2d point
typedef struct Vertice {
	Point3d p3d;
	Point2d p2d;

} Vertice;

//Basicly the world, how the 3d points is projected, by manipulation this will rotate/zoom etc.
typedef struct PreCalc {
	int azimuth, elevation;
	double nearToObj;
	int scaleFactor;
	int near;
	double theta;
	double phi;
	double cosT, sinT;
	double cosP, sinP;
	double cosTcosP, cosTsinP, sinTcosP, sinTsinP;

} PreCalc;

//Calculate the projected 2d points from the 3d points and fit them to screen and the world matrix
void calcVertice(Vertice *V, PreCalc *PC) {

	// compute an orthographic projection
	double x1 = PC->cosT * V->p3d.x + PC->sinT * V->p3d.z;
	double y1 = -PC->sinTsinP * V->p3d.x + PC->cosP * V->p3d.y + PC->cosTsinP
			* V->p3d.z;

	// now adjust things to get a perspective projection
	double z1 = PC->cosTcosP * V->p3d.z - PC->sinTcosP * V->p3d.x - PC->sinP
			* V->p3d.y;
	x1 = x1 * PC->near / (z1 + PC->near + PC->nearToObj);
	y1 = y1 * PC->near / (z1 + PC->near + PC->nearToObj);
	//scale
	V->p2d.x = 64 + PC->scaleFactor * x1 + 0.5; //0.5 for int rounding error
	V->p2d.y = 32 - PC->scaleFactor * y1 + 0.5;

}

//Set the change of the cube, since it is specified in magnitude of 1
void setSizeCube(Vertice( Cube[8]), int size) {
	for (int i = 0; i < 8; i++) {
		Cube[i].p3d.x = Cube[i].p3d.x * size;
		Cube[i].p3d.y = Cube[i].p3d.y * size;
		Cube[i].p3d.z = Cube[i].p3d.z * size;
	}

}
//Calculate the whole cube to 2d points
void calcCube(Vertice( Cube[8]), PreCalc *PC) {
	for (int i = 0; i < 8; i++) {
		calcVertice(&Cube[i], PC);
	}
}
//Draw the cubes 2d points by lines, a wireframe
void drawCube(LCD_engine *lcd, Vertice( Cube[8]), int CubeEdges[24]) {
	for (int i = 0; i < 23; i += 2) {
		LCD_drawLine(lcd, Cube[CubeEdges[i]].p2d.x, Cube[CubeEdges[i]].p2d.y,
				Cube[CubeEdges[i + 1]].p2d.x, Cube[CubeEdges[i + 1]].p2d.y);
	}

}
//Rotate the world matrix
void RotatePreCalc(PreCalc *PC, int rot) {
	PC->azimuth += rot;

	PC->theta = 3.1415 * PC->azimuth / 180.0;
	PC->phi = 3.1415 * PC->elevation / 180.0;
	PC->cosT = cos(PC->theta);
	PC->sinT = sin(PC->theta);
	PC->cosP = cos(PC->phi);
	PC->sinP = sin(PC->phi);
	PC->cosTcosP = PC->cosT * PC->cosP;
	PC->cosTsinP = PC->cosT * PC->sinP;
	PC->sinTcosP = PC->sinT * PC->cosP;
	PC->sinTsinP = PC->sinT * PC->sinP;

}

//Starting the 3dDemo
int start_3dDemo(LCD_engine *lcd, TouchCalibration *TC) {
	//Setup LCD
	LCD_CLR();
	LCD_RAM_CLR(lcd);
	resetConnection();
	setDubbelbuffer(lcd, 1);
	pixelSetOrClearOrInvert(lcd, SET_PIXELS);
	resetConnection();

	pushRAM(lcd); //Not really needed here


	int width = 128;//Not really needed to be a variable
	int rotSpeed = 1;//Initial rotation speed

	PreCalc PC; //This "World"
	PC.azimuth = 35; //Angular measurement in a spherical coordinate system
	PC.elevation = 30; //height above reference point (center)
	PC.nearToObj = 5; // distance from near plane to center of object

	PC.scaleFactor = width / 4; //Scalefactor to fit screen
	PC.near = 3; // distance from eye to near plane

	//Setup the world
	PC.theta = 3.1415 * PC.azimuth / 180.0;
	PC.phi = 3.1415 * PC.elevation / 180.0;
	PC.cosT = cos(PC.theta);
	PC.sinT = sin(PC.theta);
	PC.cosP = cos(PC.phi);
	PC.sinP = sin(PC.phi);
	PC.cosTcosP = PC.cosT * PC.cosP;
	PC.cosTsinP = PC.cosT * PC.sinP;
	PC.sinTcosP = PC.sinT * PC.cosP;
	PC.sinTsinP = PC.sinT * PC.sinP;

	//Setup a Cube
	Vertice Cube[8];
	Cube[0].p3d.x = 1;
	Cube[0].p3d.y = 1;
	Cube[0].p3d.z = -1;
	Cube[1].p3d.x = 1;
	Cube[1].p3d.y = 1;
	Cube[1].p3d.z = 1;
	Cube[2].p3d.x = -1;
	Cube[2].p3d.y = 1;
	Cube[2].p3d.z = 1;
	Cube[3].p3d.x = -1;
	Cube[3].p3d.y = 1;
	Cube[3].p3d.z = -1;
	Cube[4].p3d.x = -1;
	Cube[4].p3d.y = -1;
	Cube[4].p3d.z = -1;
	Cube[5].p3d.x = 1;
	Cube[5].p3d.y = -1;
	Cube[5].p3d.z = -1;
	Cube[6].p3d.x = 1;
	Cube[6].p3d.y = -1;
	Cube[6].p3d.z = 1;
	Cube[7].p3d.x = -1;
	Cube[7].p3d.y = -1;
	Cube[7].p3d.z = 1;
	//Set the edges, where first is from Vertice and second is to Vertice etc.
	int CubeEdges[24] = { 
			0, 1, 
			0, 5, 
			0, 3, 
			4, 3, 
			4, 5, 
			4, 7, 
			2, 3, 
			2, 1, 
			2, 7,
			6, 5, 
			6, 1, 
			6, 7 
	};
	//set size 1... Wel, does nothing. Still, like it to be here just to show it can be changede this way
	setSizeCube(Cube, 1);
	calcCube(Cube, &PC); //Calculate the cubes projection
	//draw the cube
	drawCube(lcd, Cube, CubeEdges);

	//for rotation by dragging on the screen
	int downX = 0;
	int downA = PC.azimuth;

	while (1) {//Main loop

		if (START_isPressedAndRel()) {
			return 5; //Exit
		}
		pixelSetOrClearOrInvert(lcd, CLEAR_PIXELS);
		drawCube(lcd, Cube, CubeEdges);
		//Scrollers
		fillRect(lcd, 122, 30 - rotSpeed, 5, 4);
		fillRect(lcd, PC.nearToObj * 6, 58, 4, 5);
		//Scrollers - end

		if (TOUCH_STATE == TOUCH_STATE_PRESSED) {
			Point p = getTouch(TC);
			if (p.down_count > 8) {//Pressing


				if (p.x >= 122) {
					rotSpeed = 32 - p.y;
					if (rotSpeed > 29)
						rotSpeed = 29;
					if (rotSpeed < -29)
						rotSpeed = -29;
				} else if (p.y > 55) {
					PC.nearToObj = p.x / 6;

				} else {//free moving
					if (downX == 0) {
						downX = p.x;
						downA = PC.azimuth;
					}
					rotSpeed = 0;
					PC.azimuth = downA + (downX - p.x);
				}

			}

		} else {
			downX = 0;
		}

		RotatePreCalc(&PC, rotSpeed);
		calcCube(Cube, &PC);
		pixelSetOrClearOrInvert(lcd, SET_PIXELS);
		drawRect(lcd, 122, 0, 5, 63);
		drawRect(lcd, 0, 58, 121, 5);
		fillRect(lcd, 122, 30 - rotSpeed, 5, 4);
		fillRect(lcd, PC.nearToObj * 6, 58, 4, 5);
		drawCube(lcd, Cube, CubeEdges);

		pushRAM(lcd);

	}

	return 0;

}
