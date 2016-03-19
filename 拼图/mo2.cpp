#include<iostream>
#include"SDL/SDL_image.h"
#include"SDL/SDL_ttf.h"
#include"SDL/SDL.h"
#include<time.h>
#include <string>
#include<sstream>
#define img_w 600
#define img_h 450
using namespace std;
SDL_Surface* scr=NULL;
SDL_Surface* img=NULL;
SDL_Surface* back=NULL;
SDL_Surface* mes=NULL;
SDL_Surface* mes2=NULL;
TTF_Font* font=NULL;
TTF_Font* font2=NULL;
SDL_Color textcolor={187,227,216};
SDL_Color textcolor2={255,0,0};
SDL_Rect clips[9];
SDL_Event event;
int n=0;
int map[3][3]={0,1,2,3,4,5,6,7,8};
void init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	scr=SDL_SetVideoMode(840,450,32,SDL_SWSURFACE);
	SDL_WM_SetCaption("拼图",NULL);
	clips[0].x = 0;
	clips[0].y = 0;
	clips[0].w = img_w/3;
	clips[0].h = img_h/3;
	clips[1].x = img_w/3;
	clips[1].y = 0;
	clips[1].w = img_w/3;
	clips[1].h = img_h/3;
	clips[2].x = 2*img_w/3;
	clips[2].y = 0;
	clips[2].w = img_w/3;
	clips[2].h = img_h/3;

	clips[3].x = 0;
	clips[3].y = img_h/3;
	clips[3].w = img_w/3;
	clips[3].h = img_h/3;
	clips[4].x = img_w/3;
	clips[4].y = img_h/3;
	clips[4].w = img_w/3;
	clips[4].h = img_h/3;
	clips[5].x = 2*img_w/3;
	clips[5].y = img_h/3;
	clips[5].w = img_w/3;
	clips[5].h = img_h/3;

	clips[6].x = 0;
	clips[6].y = 2*img_h/3;
	clips[6].w = img_w/3;
	clips[6].h = img_h/3;
	clips[7].x = img_w/3;
	clips[7].y = 2*img_h/3;
	clips[7].w = img_w/3;
	clips[7].h = img_h/3;
	clips[8].x = 2*img_w/3;
	clips[8].y = 2*img_h/3;
	clips[8].w = img_w/3;
	clips[8].h = img_h/3;

	font=TTF_OpenFont("lazy.ttf",28);
	font=TTF_OpenFont("lazy.ttf",50);
	//mes=TTF_RenderText_Solid(font,t.str().c_str(),textcolor);
	SDL_Surface* tmp=NULL;
	tmp=IMG_Load("q1.jpg");
	back=IMG_Load("q1-2.jpg");
	img=SDL_DisplayFormat(tmp);
	back=SDL_DisplayFormat(back);
	SDL_FreeSurface(tmp);
	Uint32 colorkey=SDL_MapRGB(img->format,0,0xFF,0xFF);
	SDL_SetColorKey(img,SDL_SRCCOLORKEY,colorkey);
}
void dis(int x,int y,SDL_Surface* tu=NULL,SDL_Surface* s=NULL,SDL_Rect* f=NULL)
{
	SDL_Rect xx;
	xx.x=x;
	xx.y=y;
	SDL_BlitSurface(tu,f,s,&xx);
}
void draw()
{
	SDL_FillRect(scr,NULL,SDL_MapRGB( scr->format, 0x00, 0x00, 0x00 ));
	for(int i=0;i<9;i++)
	{
			switch (i)
			{
				case 0:if(map[0][0]!=8){dis(0, 0, img, scr, &clips[map[0][0]]);}break;
				case 1:if(map[0][1]!=8){dis(img_w/3, 0, img, scr, &clips[map[0][1]]);}break;
				case 2:if(map[0][2]!=8){dis(2*img_w/3, 0, img, scr,&clips[map[0][2]]);}break;
				case 3:if(map[1][0]!=8){dis(0, img_h/3, img, scr, &clips[map[1][0]]);}break;
				case 4:if(map[1][1]!=8){dis(img_w/3, img_h/3, img, scr,&clips[map[1][1]]);}break;
				case 5:if(map[1][2]!=8){dis(2*img_w/3, img_h/3, img, scr,&clips[map[1][2]]);}break;
				case 6:if(map[2][0]!=8){dis(0, 2*img_h/3, img, scr,&clips[map[2][0]]);}break;
				case 7:if(map[2][1]!=8){dis(img_w/3, 2*img_h/3, img, scr,&clips[map[2][1]]);}break;
				case 8:if(map[2][2]!=8){dis(2*img_w/3, 2*img_h/3, img,scr, &clips[map[2][2]]);}break;
			}
	}
	dis(620,0,back,scr,NULL);
	//dis(600,360,mes,scr,NULL);
	SDL_Flip(scr);
}

int main()
{
	srand(time(0));
	bool quit=false;
	bool win=false;
	int i,j,x,y,m;
	init();
	x=y=2;

	for(i=0;i<300;i++)
	{
		m=rand()%4;
		switch(m)
		{
			case 0 : 
			{
				if( x>0 )
				{
					map[x][y]=map[x-1][y];map[x-1][y]=8;x--;
				}
			}break;
			case 1 : 
			{
				if( x<2 )
				{
					map[x][y]=map[x+1][y];map[x+1][y]=8;x++;
				}
			}break;
			case 2 : 
			{
				if( y>0 )
				{
					map[x][y]=map[x][y-1];map[x][y-1]=8;y--;
				}
			}break;
			case 3 : 
			{
				if( y<2 )
				{
					map[x][y]=map[x][y+1];map[x][y+1]=8;y++;
				}
			}break;

		}
	}
	draw();
	while(quit==false)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type==SDL_QUIT)
			quit=true;
			if(event.type==SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_UP:
					{					
						if( x<2 )
						{
							map[x][y]=map[x+1][y];map[x+1][y]=8;x++;
							printf("(%d,%d) %d UP\n",x,y,map[x][y]);n++;
						}
					}break;
					case SDLK_DOWN:
					{
						if( x>0 )
						{
							map[x][y]=map[x-1][y];map[x-1][y]=8;x--;
							printf("(%d,%d) %d DOWN\n",x,y,map[x][y]);n++;
						}
					}break;
					case SDLK_LEFT:
					{
						if( y<2 )
						{
							map[x][y]=map[x][y+1];map[x][y+1]=8;y++;
							printf("(%d,%d) %d LEFT\n",x,y,map[x][y]);n++;
						}
					}break;
					case SDLK_RIGHT:
					{
						if( y>0 )
						{
							map[x][y]=map[x][y-1];map[x][y-1]=8;y--;
							printf("(%d,%d) %d RIGHT\n",x,y,map[x][y]);n++;
						}
					}break;
				}
			}
		}
/*
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			printf("(%d,%d) %d ",i,j,map[i][j]);
			printf("\n");
		}
*/		
		draw();
		SDL_Delay(100);
	}
	SDL_FreeSurface(img);
	SDL_FreeSurface(back);
	return 0;
}
