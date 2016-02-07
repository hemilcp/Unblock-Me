#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<math.h>
#include<stdlib.h>

#define FREE_BLOCK RED
#define WHITE_BLOCK 15

union REGS in,out;
int LEVEL=1;
const int l1n=9,l2n=12,l3n=12,l4n=11,l5n=10;
const int l1bars[][5]={{152,101,198,199,2},{302,101,348,199,2},{302,201,348,299,2},{202,301,248,399,2},{402,201,448,349,2},{151,252,299,298,1},{251,302,349,348,1},{251,352,349,398,1},{151,202,249,248,1}};
const int l2bars[][5]={{152,101,198,199,2},{402,101,448,199,2},{252,201,298,299,2},{302,301,348,399,2},{352,151,398,249,2},{201,102,299,148,1},{301,102,399,148,1},{251,152,349,198,1},{151,252,249,298,1},{301,252,399,298,1},{151,302,299,348,1},{151,202,249,248,1}};
const int l3bars[][5]={{152,151,198,249,2},{152,251,198,349,2},{202,101,248,199,2},{252,251,298,399,2},{302,101,348,199,2},{302,201,348,299,2},{402,301,448,399,2},{301,302,399,348,1},{301,352,399,398,1},{351,152,449,198,1},{351,252,449,298,1},{201,202,299,248,1}};
const int l4bars[][5]={{202,101,248,199,2},{352,101,398,199,2},{252,201,298,299,2},{252,301,298,399,2},{302,201,348,349,2},{152,251,198,399,2},{251,102,349,148,1},{251,152,349,198,1},{351,302,449,348,1},{301,352,449,398,1},{151,202,249,248,1}};
const int l5bars[][5]={{152,101,198,249,2},{252,101,298,199,2},{302,151,348,249,2},{402,201,448,349,2},{301,102,449,148,1},{151,252,249,298,1},{151,302,249,348,1},{251,252,349,298,1},{251,302,349,348,1},{201,202,299,248,1}};

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

void drawStars(int moves)
{	int stars[][22]={{230,415,225,435,210,435,220,450,215,470,230,460,245,470,240,450,250,435,235,435,230,415},{300,415,295,435,280,435,290,450,285,470,300,460,315,470,310,450,320,435,305,435,300,415},{370,415,365,435,350,435,360,450,355,470,370,460,385,470,380,450,390,435,375,435,370,415}};
	int lstr[][2]={{15,20},{20,25},{26,36},{28,38},{32,47}};
	setfillstyle(1,YELLOW);
	if(moves==lstr[LEVEL-1][0])
	{	fillpoly(11,stars[0]);
		sound(500);
		delay(500);
		nosound();
		fillpoly(11,stars[1]);
		sound(1000);
		delay(500);
		nosound();
		fillpoly(11,stars[2]);
		sound(1500);
		delay(500);
		nosound();
	}
	else if(moves>lstr[LEVEL-1][0] && moves<=lstr[LEVEL-1][1])
	{ 	fillpoly(11,stars[0]);
		sound(500);
		delay(500);
		nosound();
		fillpoly(11,stars[1]);
		sound(1000);
		delay(500);
		nosound();
	}
	else
	{	fillpoly(11,stars[0]);
		sound(500);
		delay(500);
		nosound();
	}
}


int runGame(int bars[][5],int n)
{	int cl=0,prcl=0,x,y,i,x1,y1,dx,dy,barcol,moves=0,no,arr[5]={0,0,0,0,0},op;
	callmouse();
	op=1;
	gotoxy(25,5);
	printf(" LEVEL : %d",LEVEL);
	while(1)
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
			else if(x1>=500 && x1<=550 && y1>=360 && y1<=410)
			{ return -1;	}
			else if(x1>=150 && x1<=200 && y1>=415 && y1<=465)
			{ return 2; }
			else if(x1>=415 && x1<=465 && y1>=415 && y1<=465)
			{ return 1;	}
			else if(x1>=580 && x1<=620 && y1>=10 && y1<=50)
			{ return 0; }
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
		gotoxy(40,5);
		printf(" MOVES : %02d",moves);
		prcl=cl;
	}
	gotoxy(40,5);
	printf(" MOVES : %02d",moves);
	drawStars(moves);
	delay(1000);
	return 1;
}

void drawLevel(int bars[][5],int n)
{	int i,undob[]={500,300,550,350},undocx,undocy,rad,reloadb[]={500,360,550,410},reloadcx,reloadcy;
	TOP=NULL;
	setcolor(15);
	//TITLE
	settextstyle(3,0,4);
	outtextxy(180,20,"UNBLOCK ME FREE");
	
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

	//RELOAD BUTTON
	setfillstyle(1,15);
	bar(reloadb[0],reloadb[1],reloadb[2],reloadb[3]);
	setcolor(0);
	reloadcx=(reloadb[0]+reloadb[2])/2;	reloadcy=(reloadb[1]+reloadb[3])/2;
	circle(reloadcx,reloadcy,rad);
	line(reloadcx+rad,reloadcy-2,reloadcx+rad-7,reloadcy+5);
	line(reloadcx+rad,reloadcy-2,reloadcx+rad+7,reloadcy+5);
	line(reloadcx-rad,reloadcy+2,reloadcx-rad-7,reloadcy-5);
	line(reloadcx-rad,reloadcy+2,reloadcx-rad+7,reloadcy-5);
	
	//NEXT AND PREVIOUS LEVEL BUTTON
	setfillstyle(1,15);
	bar(150,415,200,465);
	bar(400,415,450,465);
	setcolor(0);
	line(155,440,195,440);
	line(155,440,165,430);
	line(155,440,165,450);
	line(405,440,445,440);
	line(445,440,435,430);
	line(445,440,435,450);
	
	//CLOSE THE GAME	
	setfillstyle(1,15);
	bar(580,10,620,50);
	setcolor(0);
	line(585,15,615,45);
	line(585,45,615,15);
	
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

void resetBars(int bars[][5])
{       int i,j;
	switch(LEVEL)
	{	case 1: for(i=0;i<l1n;i++)
				{ 	for(j=0;j<5;j++)
					{ bars[i][j]=l1bars[i][j]; }
				}
				break;
		case 2: for(i=0;i<l2n;i++)
				{ 	for(j=0;j<5;j++)
					{ bars[i][j]=l2bars[i][j]; }
				}
				break;
		case 3: for(i=0;i<l3n;i++)
				{ 	for(j=0;j<5;j++)
					{ bars[i][j]=l3bars[i][j]; }
				}
				break;
		case 4: for(i=0;i<l4n;i++)
				{ 	for(j=0;j<5;j++)
					{ bars[i][j]=l4bars[i][j]; }
				}
				break;
		case 5: for(i=0;i<l5n;i++)
				{ 	for(j=0;j<5;j++)
					{ bars[i][j]=l5bars[i][j]; }
				}
				break;
	}
}

int main()
{ 	int n,fin;
	int bars[12][5];
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"..\\bgi");
	
	LEVEL=1;
	setbkcolor(9);
	do
	{	cleardevice();
		setposi(10,10);
		fin=0;
		resetBars(bars);
		switch(LEVEL)
		{	case 1:	drawLevel(bars,l1n);
					fin=runGame(bars,l1n);
					break;
			case 2: drawLevel(bars,l2n);
					fin=runGame(bars,l2n);
					break;
			case 3: drawLevel(bars,l3n);
					fin=runGame(bars,l3n);
					break;
			case 4: drawLevel(bars,l4n);
					fin=runGame(bars,l4n);
					break;
			case 5: drawLevel(bars,l5n);
					fin=runGame(bars,l5n);
					break;
		}
		freeMemory();
		if(fin==1)
		{ 	if(LEVEL<5)
			{ LEVEL++; }
			else
			{ LEVEL=5; }
		}
		else if(fin==2)
		{	if(LEVEL>1)
			{ LEVEL--; }
			else
			{ LEVEL=1; }
		}
		else if(fin==0)
		{ break; }
	}while(1);
	cleardevice();
	return 0;
}