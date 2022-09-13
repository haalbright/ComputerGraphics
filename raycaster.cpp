#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533

//variables we will need:
float pa;//camera/perspective angle
float px, py;//camera/perspective x/y coordinates
int dof;//depth of field(whole grid's height/width)
int gridSize = 64;//the # of pixels in each grid square(e.g. 64)
int mapX, mapY, mapS;//world's grid height/width/# of blocks
int map[];//2d array representing a world (e.g. 1=wall, 0=empty block)

//a function to return the distance between the camera/perspective
//and the ray's endpoint
float dist(float ax, float ay, float bx, float by, float ang)
{
	return(sqrt((bx-ax)*(bx-ax)+(by-ay)*(by-ay)));
}

//casting/generating rays
void drawRays()
{
	int r,mx,my,mp,tempDOF; float rx,ry,ra,xo,yo,disT;
	//setting up # of rays and their angle
	ra=pa-DR*30; if(ra<0){ ra+=2*PI;} if(ra>2*PI){ ra-=2*PI;}
	for(r=0;r<60;r++)
	{
		//checking for horizontal grid lines
		tempDOF=0;
		float disH=1000000,hx=px,hy=py;
		float aTan=-1/tan(ra);
		//finding x/y values where the ray will hit the
		//closest horizontal line.
		//first we need to find out if the ray is looking up or down
		//by looking at the ray's angle. >PI(>180DR) = facing downward
		if(ra>PI)//looking downward
		{
			//we need to round the ray's y position to the nearest gridSize's value
			ry=(((int)py>>6)<<6)-0.0001;
			//the ray's x value is the distance between the camera and the 
			//ray's y position * tangent inverse + camera's x position
			rx=(py-ry)*aTan+px;
			//after we found the first hit position we need the next x and y
			//offset. for the y offset we will subtract whatever grid size is
			yo=-gridSize;
			//the x offset is y offset value * tangent inverse
			xo=-yo*aTan;
		}
		//<PI(180DR) = facing upward
		if(ra<PI)//looking upward
		{
			//all the values are the same as for looking downward except that 
			//y offset becomes positive
			ry=(((int)py>>6)<<6)+gridSize;
			rx=(py-ry)*aTan+px;
			yo=gridSize;
			xo=-yo*aTan;
		}
		//when facing strictly left or right it is impossible for the ray to
		//ever hit a horizontal line
		if(ra==0 || ra==PI){ rx=px; ry=py; tempDOF = dof; }//looking left/right
		//we don't want to check forever and get caught in a loop, so we set
		//depth of field to maximum(height/width of the whole grid) in case there is no wall
		while(tempDOF<dof)
		{
			mx=(int)(rx)>>6;my=(int)(ry)>>6;mp=my*mapX+mx;
			if(mp>0 && mp<mapX*mapY && map[mp]==1)//hit wall
			{
				hx=rx;
				hy=ry;
				disH=dist(px,py,hx,hy,ra);
				tempDOF=dof;
			}
			//if we didn't hit a wall then check the next
			//horizontal line by adding x/y offsets
			else{ rx+=xo; ry+=yo; tempDOF+=1;}//next line
		}
		
		//checking for vertical grid lines
		tempDOF=0;
		float disV=1000000,vx=px,vy=py;
		float nTan=-tan(ra);
		//when checking for vertical lines we need to check whether
		//we are facing left/right and swap all the x/y values
		if(ra>P2 && ra<P3)//looking left
		{
			rx=(((int)px>>6)<<6)-0.0001;
			ry=(px-rx)*nTan+py;
			xo=-gridSize;
			yo=-xo*nTan;
		}
		if(ra<P2 || ra>P3)//looking right
		{
			rx=(((int)px>>6)<<6)+gridSize;
			ry=(px-rx)*nTan+py;
			xo=gridSize;
			yo=-xo*nTan;
		}
		//when facing strictly up/down it is impossible for the ray to
		//ever hit a vertical line
		if(ra==0 || ra==PI){ rx=px; ry=py; tempDOF=dof;}//looknig up/down
		while(tempDOF<dof)
		{
			mx=(int)(rx)>>6; my=(int)(ry)>>6; mp=my*mapX+mx;
			if(mp>0 && mp<mapX*mapY && map[mp]==1)//hit wall
			{
				vx=rx;
				vy=ry;
				disV=dist(px,py,vx,vy,ra);
				tempDOF=dof;
			}
			else{ rx+=xo; ry+=yo; tempDOF+=1;}
		}
		//vertical wall hit
		if(disV<disH){ rx=vx; ry=vy; disT=disV; glColor3f(0.9,0,0);}
		//horizontal wall hit
		if(disH<disV){ rx=hx; ry=hy; disT=disH; glColor3f(0.7,0,0);}
		glLineWidth(3);
		glBegin(GL_LINES);
		glVertex2i(px,py);
		glVertex2i(rx,ry);
		glEnd();
		
		//drawing the scene
		//fixing fisheye
		float ca=pa-ra; if(ca<0){ ca+=2*PI;} if(ca>2*PI){ ca-=2*PI;}
		disT=disT*cos(ca);
		float lineH=(mapS*320)/disT; if(lineH>320){lineH=320;}//line height
		float lineO=160-lineH/2;//line offset
		glLineWidth(8);
		glBegin(GL_LINES);
		glVertex2i(r*dof,lineO);
		glVertex2i(r*dof,lineH+lineO);
		glEnd();
		ra+=DR; if(ra<0){ra+=2*PI;}if(ra>2*PI){ra-=2*PI;}
	}
}