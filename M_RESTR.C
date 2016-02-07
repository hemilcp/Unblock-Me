#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>
union REGS in,out;

int callmouse()
{
	 in.x.ax=1;
	 int86(51,&in,&out);
	 return 1;
}
void restrictmouseptr(int x1,int y1,int x2,int y2)
{
	 in.x.ax=7;
	 in.x.cx=x1;
	 in.x.dx=x2;
	 int86(51,&in,&out);
	 in.x.ax=8;
	 in.x.cx=y1;
	 in.x.dx=y2;
	 int86(51,&in,&out);
}
int main()
{ 
	 int x,y,cl,a,b;
	 int g=DETECT,m;
	 initgraph(&g,&m,"..\\bgi");
	 cleardevice();
	 rectangle(100,100,550,400);
	 callmouse();
	 restrictmouseptr(100,100,550,400);
	 getch();
	 return 1;
}