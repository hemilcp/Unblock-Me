#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<math.h>
#include<stdlib.h>

#define FREE_BLOCK RED
#define WHITE_BLOCK 15

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
{	in.x.ax=3;
	int86(51,&in,&out);
	*click=out.x.bx;
	*xpos=out.x.cx;
	*ypos=out.x.dx;
}

struct node
{ int barpos[5]; 
  int barno;
  int diff;
  struct node *link;
}*TOP;
typedef struct node stack;

void push(int bars[5],int no,int d)
{ stack *NODE;
  int i;
  NODE=(stack*)malloc(sizeof(stack));
  NODE->barno=no;
  NODE->diff=d;
  for(i=0;i<5;i++)
  { NODE->barpos[i]=bars[i]; }
  if(TOP==NULL)
  {	NODE->link=NULL;
	TOP=NODE;
  }
  else
  { NODE->link=TOP;
	TOP=NODE;
  }
}

int pop(int bars[5],int *d)
{	int i,n=-1;
	stack *NODE;
	if(TOP!=NULL)
	{	NODE=TOP;
		for(i=0;i<5;i++)
		{ bars[i]=NODE->barpos[i]; }
		n=NODE->barno;
		*d=NODE->diff;
		TOP=TOP->link;
		free(NODE);
		return n;
	}
	else
	{ for(i=0;i<5;i++)
	  { bars[i]=0; }
	  *d=0;
	  return -1; 
	}
}

int verticalMove(int ddy,int arr[4])
{	int y1,y2,miny,maxy,bary1,bary2,dy,i,midx,oldy1,oldy2,col,bartype,move;
    oldy1=arr[1];	oldy2=arr[3];
	bary1=arr[1];	bary2=arr[3];
	midx=(arr[0]+arr[2])/2;
	if((arr[3]-arr[1])<=100)
	{ bartype=1; }
	else
	{ bartype=2; }
	dy=ddy/50;
	y1=bary1;	y2=bary2;
	if(dy>0 && dy!=0)
	{	maxy=100;
		for(i=bary2+25;i<400;i+=50)
		{	col=getpixel(midx,i);
			if(col==WHITE_BLOCK || col==FREE_BLOCK)
			{	maxy=i;	break;   }
		}
		bary2+=dy*50;
		if(maxy!=100 && bary2>maxy)
		{       bary2=y2;
				while(dy>0)
				{ 	dy--;
					y2=bary2+dy*50;
					if(y2<maxy)
					{ break; }
				}
				bary2=y2;
		}
		else if(maxy==100 && bary2>400)
		{	bary2=400;	}
		if(bartype==1)
		{  bary1=bary2-98; }
		else
		{  bary1=bary2-148; }
		setfillstyle(1,0);
		bar(arr[0],oldy1,arr[2],bary1);
	}
	else if(dy<0 && dy!=0)
	{	miny=400;
		for(i=bary1-25;i>100;i-=50)
		{	col=getpixel(midx,i);
			if(col==WHITE_BLOCK || col==FREE_BLOCK)
			{	miny=i;		break;		}
		}
		bary1+=dy*50;
		if(miny!=400 && bary1<miny)
		{   bary1=y1;
			while(dy<0)
			{ 	dy++;
				y1=bary1+dy*50;
				if(y1>miny)
				{ break; }
			}
			bary1=y1;
		}
		else if(miny==400 && bary1<100)
		{ bary1=100;	}
		if(bartype==1)
		{ bary2=bary1+98; }
		else
		{ bary2=bary1+148; }
		setfillstyle(1,0);
		bar(arr[0],bary2,arr[2],oldy2);
	}
	arr[1]=bary1;	arr[3]=bary2;
	setfillstyle(1,15);
	bar(arr[0],arr[1],arr[2],arr[3]);
	if(abs(dy)>0)
	{ return 1; }
	else
	{ return 0; }
}

int horizontalMove(int ddx,int arr[4],int barcol)
{	int x1,x2,dx,minx,maxx,barx1,barx2,i,midy,oldx1,oldx2,col,bartype;
    oldx1=arr[0];	oldx2=arr[2];
	barx1=arr[0];	barx2=arr[2];
	if((arr[2]-arr[0])<=100)
	{ bartype=1; }
	else
	{ bartype=2; }
	dx=ddx/50;
	x1=barx1;	x2=barx2;
	midy=(arr[1]+arr[3])/2;
	if(dx!=0 && dx>0)
	{	maxx=150;
		for(i=barx2+25;i<450;i+=50)
		{	col=getpixel(i,midy);
			if(col==WHITE_BLOCK)
			{ maxx=i;	break;	}
		}
		barx2+=dx*50;
		if(maxx!=150 && barx2>maxx)
		{	barx2=x2;
			while(dx>0)
			{	dx--;
				x2=barx2+dx*50;
				if(x2<maxx)
				{ break; }
			}
			barx2=x2;
		}
		else if(maxx==150 && barx2>450)
		{	if(barcol==FREE_BLOCK)
			{ barx2=550;}
			else
			{ barx2=450; }
		}
		if(bartype==1)
		{ barx1=barx2-98; }
		else
		{ barx1=barx2-148; }
		setfillstyle(1,0);
		bar(oldx1,arr[1],barx1,arr[3]);	
	}
	else if(dx!=0 && dx<0)
	{	minx=450;
		for(i=barx1-25;i>150;i-=50)
		{	col=getpixel(i,midy);
		    if(col==WHITE_BLOCK)
			{ minx=i;	break;	}
		}
		barx1+=dx*50;
		if(minx!=450 && barx1<minx)
		{	barx1=x1;
			while(dx<0)
			{	dx++;
				x1=barx1+dx*50;
				if(x1>=minx)
				{ break; }
			}
			barx1=x1;
		}
		else if(minx==450 && barx1<150)
		{	barx1=150;		}
		if(bartype==1)
		{  barx2=barx1+98; }
		else
		{ barx2=barx1+148; }
		setfillstyle(1,0);
		bar(barx2,arr[1],oldx2,arr[3]);
	}
	arr[0]=barx1;	arr[2]=barx2;
	setfillstyle(1,barcol);
	bar(arr[0],arr[1],arr[2],arr[3]);
	if(abs(dx)>0)
	{ return 1; }
	else
	{ return 0; }
}


void runGame(int bars[][5],int n)
{	int cl,prcl,x,y,i,x1,y1,dx,dy,barcol,moves=0,no,arr[5]={0,0,0,0,0},op;
	callmouse();
	op=1;
	while(!kbhit())
	{	mouseposi(&x,&y,&cl);
	    if(cl==prcl && prcl==1)
		{	x1=x;	y1=y;
		    if(x1>=500 && x1<=550 && y1>=300 && y1<=350)
			{   if(op==1)
				{	no=pop(arr,&dx);
					dx=(-1)*dx;
					if(no!=-1)
					{   for(i=0;i<4;i++)
						{ bars[no][i]=arr[i]; }
						if(bars[no][4]==1)
						{	if(no==n-1)
							{ barcol=FREE_BLOCK; }
							else
							{ barcol=WHITE_BLOCK; }
							horizontalMove(dx,bars[no],barcol);
						}
						else
						{	verticalMove(dx,bars[no]); }
						moves--;
					}
					op=0;
					delay(500);
				}
			}
			else
			{	for(i=0;i<n;i++)
				{	if(x1>=bars[i][0] && x1<=bars[i][2] && y1>=bars[i][1] && y1<=bars[i][3])
					{	while(cl!=0)
						{ mouseposi(&x,&y,&cl); }
						dx=x-x1;	dy=y-y1;
						x1=bars[i][0]; y1=bars[i][1];
						if(bars[i][4]==1)
						{ 	if(i==n-1)
							{	if(horizontalMove(dx,bars[i],FREE_BLOCK))
								{	dx=bars[i][0]-x1;
									push(bars[i],i,dx);
									moves++;
								}
							}
							else
							{	if(horizontalMove(dx,bars[i],WHITE_BLOCK))
								{	dx=bars[i][0]-x1;
								    push(bars[i],i,dx);
									moves++;
								}
							}
						}
						else
						{	if(verticalMove(dy,bars[i]))
							{	dy=bars[i][1]-y1;
								push(bars[i],i,dy);
								moves++;
							}
						}
						break;
					}
				}
			}
		}
		if(bars[n-1][2]==550)
		{ break; }
		if(op==0)
		{ prcl=0; cl=0; op=1;}
		gotoxy(45,5);
		printf(" MOVES : %02d",moves);
		prcl=cl;
	}
}

void drawLevel(int bars[][5],int n)
{	int i,undob[]={500,300,550,350},undocx,undocy,rad;
	TOP=NULL;
	//BORDER
	setfillstyle(1,15);
	bar(140,90,460,98);
	bar(140,402,460,410);
	bar(140,90,148,410);
	bar(452,90,460,200);
	bar(452,250,460,410);

	//UNDO BUTTON
	setfillstyle(1,15);
	bar(undob[0],undob[1],undob[2],undob[3]);
	
	setcolor(0);
	undocx=(undob[0]+undob[2])/2;
	undocy=(undob[1]+undob[3])/2;
	rad=17;
	arc(undocx,undocy,150,90,rad);
	line(undocx,undocy-rad,undocx+10,undocy-rad-5);
	line(undocx,undocy-rad,undocx+5,undocy-rad+8);
	
	setfillstyle(1,WHITE_BLOCK);
	for(i=0;i<n-1;i++)
	{	bar(bars[i][0],bars[i][1],bars[i][2],bars[i][3]);	}
	setfillstyle(1,FREE_BLOCK);
	bar(bars[i][0],bars[i][1],bars[i][2],bars[i][3]);
}

void freeMemory()
{	stack *PTR,*NODE;
    NODE=TOP;
	PTR=TOP;
	while(NODE!=NULL)
	{	PTR=PTR->link;
		free(NODE);
		NODE=PTR;
	}
}

void drawStars()
{	int stars[][22]={{220,415,215,435,200,435,210,450,205,470,220,460,235,470,230,450,240,435,225,435,220,415},{290,415,285,435,270,435,280,450,275,470,290,460,305,470,300,450,310,435,295,435,290,415},{360,415,355,435,340,435,350,450,345,470,360,460,375,470,370,450,380,435,365,435,360,415}};
	setfillstyle(1,YELLOW);
    drawpoly(11,stars[0]);
	drawpoly(11,stars[1]);
	drawpoly(11,stars[2]);
    floodfill(220,440,15);
	floodfill(290,440,15);
	floodfill(360,440,15);
}

int main()
{ 	int i,l1n=9,l2n=12,l3n=12,l4n=11,l5n=10;
	//int l1bars[][5]={{152,101,198,199,2},{302,101,348,199,2},{302,201,348,299,2},{202,301,248,399,2},{402,201,448,349,2},{151,252,299,298,1},{251,302,349,348,1},{251,352,349,398,1},{151,202,249,248,1}};
	//int l2bars[][5]={{152,101,198,199,2},{402,101,448,199,2},{252,201,298,299,2},{302,301,348,399,2},{352,151,398,249,2},{201,102,299,148,1},{301,102,399,148,1},{251,152,349,198,1},{151,252,249,298,1},{301,252,399,298,1},{151,302,299,348,1},{151,202,249,248,1}};
	//int l3bars[][5]={{152,151,198,249,2},{152,251,198,349,2},{202,101,248,199,2},{252,251,298,399,2},{302,101,348,199,2},{302,201,348,299,2},{402,301,448,399,2},{301,302,399,348,1},{301,352,399,398,1},{351,152,449,198,1},{351,252,449,298,1},{201,202,299,248,1}};
	//int l4bars[][5]={{202,101,248,199,2},{352,101,398,199,2},{252,201,298,299,2},{252,301,298,399,2},{302,201,348,349,2},{152,251,198,399,2},{251,102,349,148,1},{251,152,349,198,1},{351,302,449,348,1},{301,352,449,398,1},{151,202,249,248,1}};
	int l5bars[][5]={{152,101,198,249,2},{252,101,298,199,2},{302,151,348,249,2},{402,201,448,349,2},{301,102,449,148,1},{151,252,249,298,1},{151,302,249,348,1},{251,252,349,298,1},{251,302,349,348,1},{201,202,299,248,1}};
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"..\\bgi");
	cleardevice();
	setbkcolor(9);
	setposi(10,10);
	
	//DRAW LEVEL 1
	drawLevel(l5bars,l5n);
	runGame(l5bars,l5n);
	gotoxy(1,10);
	printf("\nEND OF LEVEL 2");
	freeMemory();
	getch();
	return 0;
}