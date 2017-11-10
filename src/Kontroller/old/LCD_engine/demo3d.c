#include "demo3d.h"
#include "touch.h"
#include "gui.h"
#include "lcd_engine.h"
#include "macros.h"
#include "buttons.h"
#include <math.h>



typedef struct Point2d{
int x,y;

}Point2d;

typedef struct Point3d{
int x,y,z;

}Point3d;

typedef struct Vertice{
Point3d p3d;
Point2d p2d;

}Vertice;

typedef struct PreCalc{
int azimuth,elevation;
double nearToObj;
int scaleFactor;
int near;
//int w_half,h_half;
double theta;
double phi ;
double cosT, sinT ;
double cosP , sinP ;
double cosTcosP , cosTsinP , sinTcosP, sinTsinP;

}PreCalc;

void calcVertice(Vertice *V,PreCalc *PC){

// compute an orthographic projection
         double x1 = PC->cosT*V->p3d.x + PC->sinT*V->p3d.z;
         double y1 = -PC->sinTsinP*V->p3d.x + PC->cosP*V->p3d.y + PC->cosTsinP*V->p3d.z;

         // now adjust things to get a perspective projection
         double z1 = PC->cosTcosP*V->p3d.z - PC->sinTcosP*V->p3d.x - PC->sinP*V->p3d.y;
         x1 = x1*PC->near/(z1+PC->near+PC->nearToObj);
         y1 = y1*PC->near/(z1+PC->near+PC->nearToObj);
		
		V->p2d.x=64 + PC->scaleFactor*x1 + 0.5;
        V->p2d.y=32 - PC->scaleFactor*y1 + 0.5;
		//V->p2d.x=width/2 + PC->scaleFactor*x1 + 0.5;
        //V->p2d.y=height/2 - PC->scaleFactor*y1 + 0.5;
}

void setSizeCube(Vertice (Cube[8]),int size){
for (int i=0;i<8;i++){
Cube[i].p3d.x=Cube[i].p3d.x*size;
Cube[i].p3d.y=Cube[i].p3d.y*size;
Cube[i].p3d.z=Cube[i].p3d.z*size;
}

}

void calcCube(Vertice (Cube[8]),PreCalc *PC){
for (int i=0;i<8;i++){
calcVertice(&Cube[i],PC);

}

}
void drawCube(LCD_engine *lcd,Vertice (Cube[8]),int CubeEdges[24]){
for (int i=0;i<23;i+=2){
LCD_drawLine(lcd,Cube[CubeEdges[i]].p2d.x,Cube[CubeEdges[i]].p2d.y,Cube[CubeEdges[i+1]].p2d.x,Cube[CubeEdges[i+1]].p2d.y);
}

}

void RotatePreCalc(PreCalc *PC,int rot){
PC->azimuth+=rot;

      PC->theta = 3.1415 * PC->azimuth / 180.0;
      PC->phi = 3.1415 * PC->elevation / 180.0;
      PC->cosT = cos( PC->theta );
	   PC->sinT = sin( PC->theta );
      PC->cosP = cos( PC->phi );
	   PC->sinP = sin( PC->phi );
      PC->cosTcosP = PC->cosT*PC->cosP;
	  PC->cosTsinP = PC->cosT*PC->sinP;
       PC->sinTcosP = PC->sinT*PC->cosP; 
	   PC->sinTsinP = PC->sinT*PC->sinP;

}




int start_3dDemo(LCD_engine *lcd){
LCD_CLR();
LCD_RAM_CLR(lcd);
resetConnection();
setDubbelbuffer(lcd,1);
pixelSetOrClearOrInvert(lcd,SET_PIXELS);
resetConnection();

//printfStr(lcd,"hello3d",0,0);


pushRAM(lcd);

int width=128;
int rotSpeed=1;
//int height=64;


//Point3d p3d[]={1,2,3};


//Point2d p1_c;
PreCalc PC;
PC.azimuth = 35;
PC.elevation = 30;
PC.nearToObj = 5;  // distance from near plane to center of object

PC.scaleFactor = width/4;
PC.near = 3;  // distance from eye to near plane
      //double nearToObj = 1.5; 

      PC.theta = 3.1415 * PC.azimuth / 180.0;
      PC.phi = 3.1415 * PC.elevation / 180.0;
      PC.cosT = cos( PC.theta );
	   PC.sinT = sin( PC.theta );
      PC.cosP = cos( PC.phi );
	   PC.sinP = sin( PC.phi );
      PC.cosTcosP = PC.cosT*PC.cosP;
	  PC.cosTsinP = PC.cosT*PC.sinP;
       PC.sinTcosP = PC.sinT*PC.cosP; 
	   PC.sinTsinP = PC.sinT*PC.sinP;

Vertice Cube[8];
Cube[0].p3d.x=1;Cube[0].p3d.y=1;Cube[0].p3d.z=-1;
Cube[1].p3d.x=1;Cube[1].p3d.y=1;Cube[1].p3d.z=1;
Cube[2].p3d.x=-1;Cube[2].p3d.y=1;Cube[2].p3d.z=1;
Cube[3].p3d.x=-1;Cube[3].p3d.y=1;Cube[3].p3d.z=-1;
Cube[4].p3d.x=-1;Cube[4].p3d.y=-1;Cube[4].p3d.z=-1;
Cube[5].p3d.x=1;Cube[5].p3d.y=-1;Cube[5].p3d.z=-1;
Cube[6].p3d.x=1;Cube[6].p3d.y=-1;Cube[6].p3d.z=1;
Cube[7].p3d.x=-1;Cube[7].p3d.y=-1;Cube[7].p3d.z=1;

int CubeEdges[24]={0,1,0,5,0,3,4,3,4,5,4,7,2,3,2,1,2,7,6,5,6,1,6,7};

setSizeCube(Cube,1);
calcCube(Cube,&PC);

drawCube(lcd,Cube,CubeEdges);






		 // the 0.5 is to round off when converting to int
         //points[j] = new int [] {(int)(width/2 + scaleFactor*x1 + 0.5),(int)(height/2 - scaleFactor*y1 + 0.5)};


/*


Vertice Origo;
Origo.p3d.x=0;
Origo.p3d.y=0;
Origo.p3d.z=0;
calcVertice(&Origo,&PC);

Vertice Xpos;
Xpos.p3d.x=4;
Xpos.p3d.y=0;
Xpos.p3d.z=0;
calcVertice(&Xpos,&PC);

Vertice Xneg;
Xneg.p3d.x=-8;
Xneg.p3d.y=0;
Xneg.p3d.z=0;
calcVertice(&Xneg,&PC);

Vertice Ypos;
Ypos.p3d.x=0;
Ypos.p3d.y=3;
Ypos.p3d.z=0;
calcVertice(&Ypos,&PC);

Vertice Yneg;
Yneg.p3d.x=0;
Yneg.p3d.y=-8;
Yneg.p3d.z=0;
calcVertice(&Yneg,&PC);

Vertice Zpos;
Zpos.p3d.x=0;
Zpos.p3d.y=0;
Zpos.p3d.z=4;
calcVertice(&Zpos,&PC);

Vertice Zneg;
Zneg.p3d.x=0;
Zneg.p3d.y=0;
Zneg.p3d.z=-4;
calcVertice(&Zneg,&PC);
//X line
LCD_drawPixel(lcd,Xneg.p2d.x,Xneg.p2d.y);
LCD_drawPixel(lcd,Xpos.p2d.x,Xpos.p2d.y);
LCD_drawLine(lcd,Xneg.p2d.x,Xneg.p2d.y,Xpos.p2d.x,Xpos.p2d.y);
//Y line
LCD_drawPixel(lcd,Yneg.p2d.x,Yneg.p2d.y);
LCD_drawPixel(lcd,Ypos.p2d.x,Ypos.p2d.y);
LCD_drawLine(lcd,Yneg.p2d.x,Yneg.p2d.y,Ypos.p2d.x,Ypos.p2d.y);
//Z line
LCD_drawPixel(lcd,Zneg.p2d.x,Zneg.p2d.y);
LCD_drawPixel(lcd,Zpos.p2d.x,Zpos.p2d.y);
LCD_drawLine(lcd,Zneg.p2d.x,Zneg.p2d.y,Zpos.p2d.x,Zpos.p2d.y);
*/
int downX=0;
int downA=PC.azimuth;
while (1){//Main loop

if (START_isPressedAndRel()){
		return 5;
		}
pixelSetOrClearOrInvert(lcd,CLEAR_PIXELS);
//printfStr(lcd,"    ",80,2);
drawCube(lcd,Cube,CubeEdges);

fillRect(lcd,122,30-rotSpeed,5,4);
fillRect(lcd,PC.nearToObj*6,58,4,5);

	if (TOUCH_STATE==TOUCH_STATE_PRESSED){
		Point p=getTouch();
		if (p.down_count>8){
			
			
			if (p.x>=122){
			rotSpeed=32-p.y;
			if (rotSpeed>29)
				rotSpeed=29;
			if (rotSpeed<-29)
				rotSpeed=-29;
			}else if (p.y>55){
			PC.nearToObj=p.x/6;
			
			}else {//free moving
				if (downX==0){
					downX=p.x;
					downA=PC.azimuth;
					}
				rotSpeed=0;
				PC.azimuth=downA+(downX-p.x);
			}

		}
		
		}else {downX=0;}




RotatePreCalc(&PC,rotSpeed);
calcCube(Cube,&PC);
pixelSetOrClearOrInvert(lcd,SET_PIXELS);
drawRect(lcd,122,0,5,63);
drawRect(lcd,0,58,121,5);
fillRect(lcd,122,30-rotSpeed,5,4);
fillRect(lcd,PC.nearToObj*6,58,4,5);
drawCube(lcd,Cube,CubeEdges);

//printfInt(lcd,rotSpeed,80,2);
pushRAM(lcd);
//LCD_setPage(0);
//LCD_setColumn(80);
//rprintfInt(rotSpeed);rprintfStr("  ");

}


return 0;

}
