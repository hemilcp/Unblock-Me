#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>

union REGS in,out;

int callmouse()
{	in.x.ax=1;
	int86(51,&in,&out);
	return 1;
}
void setposi(int xp,int yp)
{	in.x.ax=4;
	in.x.cx=xp;
	in.x.dx=yp;
	int86(51,&in,&out);
}
void mouseposi(int *xpos,int *ypos,int *click)
{ 	  in.x.ax=3;
	  int86(51,&in,&out);
	  *click=out.x.bx;
	  *xpos=out.x.cx;
	  *ypos=out.x.dx;
}

int main()
{
	int x,y,cl,prcl,x1,y1;
	int g=DETECT,m;
	initgraph(&g,&m,"..\\bgi");
	cleardevice();
	setposi(10,10);
	callmouse();
	prcl=0;
	do
	{   mouseposi(&x,&y,&cl);
	    if(cl==prcl && prcl==1)
		{ 	x1=x; y1=y;
			moveto(x1,y1);
			while(cl!=0)
			{ mouseposi(&x,&y,&cl); }
			lineto(x,y);
		    //  rectangle(x1,y1,x,y);
		       circle(x1,y1,sqrt((long int)(x-x1)*(x-x1)+(long int)(y-y1)*(y-y1)));
		}
		gotoxy(1,1);
		printf("\n\tMouse Position is: %03d,%03d",x,y);
		prcl=cl;
	}while(!kbhit());
	getch();
	return 1;
}