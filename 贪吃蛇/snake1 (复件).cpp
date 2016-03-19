#include<iostream>
#include <string.h>
#include<time.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_ttf.h>
#define LEN sizeof(struct Snake)

using namespace std;

SDL_Surface* screen=NULL;
TTF_Font *font=NULL;
SDL_Surface *text=NULL;
SDL_Surface* img_h=NULL;
SDL_Surface* img_h_up=NULL;
SDL_Surface* img_h_down=NULL;
SDL_Surface* img_h_left=NULL;
SDL_Surface* img_h_right=NULL;

SDL_Surface* img_e=NULL;
SDL_Surface* img_e_up=NULL;
SDL_Surface* img_e_down=NULL;
SDL_Surface* img_e_left=NULL;
SDL_Surface* img_e_right=NULL;

SDL_Surface* img_b=NULL;
SDL_Surface* img_fd=NULL;
SDL_Surface* img_bk=NULL;
SDL_Event event;

int speed=500;
int dir=4;
int map[16][16]={0};
int food[2]={3,3};
int SN=0;//分数

void setfood();
void init();
SDL_Surface* loadMessage(char *text,SDL_Color &textColor,char *FontFile,int size);
void showText(SDL_Surface *msg,int x,int y,int width,int height,SDL_Surface *destination);
void dis(int x,int y,SDL_Surface* tu,SDL_Surface* s);
void set();
void action();
void gameover();
void draw();
bool restart();
struct Snake
{
	int x,y; 
	Snake* next;
};
Snake* head=NULL;

void init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	SDL_WM_SetCaption("贪吃蛇",NULL);
	img_fd=IMG_Load("apple.png");
	img_fd=SDL_DisplayFormat(img_fd);
	img_b=IMG_Load("body.png");
	img_b=SDL_DisplayFormat(img_b);
	img_bk=IMG_Load("bk.png");
	img_bk=SDL_DisplayFormat(img_bk);

	img_h_up=IMG_Load("head_u.png");
	img_h_up=SDL_DisplayFormat(img_h_up);
	img_h_down=IMG_Load("head_d.png");
	img_h_down=SDL_DisplayFormat(img_h_down);
	img_h_left=IMG_Load("head_l.png");
	img_h_left=SDL_DisplayFormat(img_h_left);
	img_h_right=IMG_Load("head_r.png");
	img_h_right=SDL_DisplayFormat(img_h_right);

	img_e_up=IMG_Load("end_u.png");
	img_e_up=SDL_DisplayFormat(img_e_up);
	img_e_down=IMG_Load("end_d.png");
	img_e_down=SDL_DisplayFormat(img_e_down);
	img_e_left=IMG_Load("end_l.png");
	img_e_left=SDL_DisplayFormat(img_e_left);
	img_e_right=IMG_Load("end_r.png");
	img_e_right=SDL_DisplayFormat(img_e_right);
}
SDL_Surface* loadMessage(char *text,SDL_Color &textColor,char *FontFile,int size)
{
	font = TTF_OpenFont(FontFile,size);
	if (font  == NULL)
	{
		printf("打开字体失败！\n");
		return 0;
	}
	return TTF_RenderText_Solid(font,text,textColor);
}
void showText(SDL_Surface *msg,int x,int y,int width,int height,SDL_Surface *destination)
{
	SDL_Rect textPosition;

	textPosition.x = x;
	textPosition.y = y;
	textPosition.w = (width == 0) ? msg->w:width;
	textPosition.h = (height == 0) ? msg->h:height;
	SDL_BlitSurface(msg,NULL,destination,&textPosition);
}
void set()
{
	Snake *p;
	head=new Snake;
	head->x=7;
	head->y=7;
	p=new Snake;
	head->next=p;
	p->x=6;p->y=7;
	p->next=new Snake;
	p=p->next;
	p->x=5;p->y=7;
	img_h=img_h_right;
	img_e=img_e_left;
}
void action()
{
	Snake* p,*p1;
	p=new Snake;
	p->next=head;
	if(dir==1)
	{
		p->x=head->x;
		p->y=head->y-1;
	}

	if(dir==2)
	{
		p->x=head->x;
		p->y=head->y+1;
	}

	if(dir==3)
	{
		p->x=head->x-1;
		p->y=head->y;
	}

	if(dir==4)
	{
		p->x=head->x+1;
		p->y=head->y;
	}

	head=p;
	while(p->next!=NULL)
	{
		p1=p;
		p=p->next;
	}
	if(head->x==food[0]  &&  head->y==food[1])
	{
		SN++;
		if(speed>=100)
			speed-=50;
		setfood();
	}
	else if(p->next==NULL)
	{
		free(p);
		p1->next=NULL;
	}
	
}

void dis(int x,int y,SDL_Surface* tu,SDL_Surface* s)
{
	SDL_Rect xx;
	xx.x=x;
	xx.y=y;
	SDL_BlitSurface(tu,NULL,s,&xx);
}
void draw()
{
	int i,j;
	Snake *p,*p1;
	char Score[10];
	SDL_Color textColor = {255,255,255};
	SDL_FillRect(screen,NULL,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));//填充背景
	for(i=0;i<16;i++)
	{
		for(j=0;j<16;j++)
		{
			if(i==0 ||  j==0 || i==15 || j==15)
				dis(i*30,j*30,img_bk,screen);
		}
	}
	dis(food[0]*30,food[1]*30,img_fd,screen);
	SDL_Delay(1);
	p=head->next;
	dis(head->x*30,head->y*30,img_h,screen);
	while(p->next!=NULL)
	{
		dis(p->x*30,p->y*30,img_b,screen);
		p1=p;
		p=p->next;
	}
	if(p->next==NULL)
	{
		if((p1->x-p->x)==0) 
		{
			if((p1->y-p->y)==1)
				img_e=img_e_down;
			else  img_e=img_e_up;
		}
		if((p1->x-p->x)==1) 
		{
			img_e=img_e_right;
		}
		if((p1->x-p->x)==-1) 
		{
			img_e=img_e_left;
		}
		dis(p->x*30,p->y*30,img_e,screen);
	}
	sprintf(Score,"%d",SN);
	text = loadMessage("Score",textColor,"yayuan.ttf",20);
	showText(text,500,50,0,0,screen);
	text = loadMessage(Score,textColor,"yayuan.ttf",20);
	showText(text,500,100,0,0,screen);
	SDL_Flip(screen);
}
void setfood()
{
	struct Snake *p;
	p=head;
	srand((unsigned)clock());//设定随机数种子 
	food[1]=rand()%12+3;//随机产生食物坐标 
	srand((unsigned)clock());
	food[0]=rand()%14+1;
	do
	{
		if (food[0]==p->x&&food[1]==p->y)
		{
			setfood();
		} 
		p=p->next;
	} 
	while(p!=NULL);
}
void gameover()
{
	Snake *p,*p1;
	p=head;p1=p->next;
	while(p1!=NULL)
	{
		p1=p->next;
		delete p;
		p=p1;
	}

	SDL_FreeSurface(screen);
	SDL_FreeSurface(img_h_up);
	SDL_FreeSurface(img_h_down);
	SDL_FreeSurface(img_h_left);
	SDL_FreeSurface(img_h_right);
	SDL_FreeSurface(img_e_up);
	SDL_FreeSurface(img_e_down);
	SDL_FreeSurface(img_e_left);
	SDL_FreeSurface(img_e_right);
	SDL_FreeSurface(img_b);
	SDL_FreeSurface(img_fd);
	SDL_FreeSurface(img_bk);
	TTF_CloseFont(font);
}
bool restart()
{
	SDL_Color textColor = {255,0,0};
	SDL_FillRect(screen,NULL,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));//填充背景
	text = loadMessage("GAME OVER!",textColor,"yayuan.ttf",30);
	showText(text,250,250,0,0,screen);
	SDL_Flip(screen);
	SDL_Delay(2000);
}
int main()
{
	init();
	bool quit=false;
	int n=0;
	bool _break=false;
	Snake snake;
	Snake *p;
	set();
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
							printf("(%d,%d) %d UP\n",head->x,head->y,n);n++;
							if(dir!=2)
							{
								dir=1;
								img_h=img_h_up;
							}
					}break;
					case SDLK_DOWN:
					{
							printf("(%d,%d) %d DOWN\n",head->x,head->y,n);n++;
							if(dir!=1)
							{
								dir=2;
								img_h=img_h_down;
							}
					}break;
					case SDLK_LEFT:
					{
							printf("(%d,%d) %d LEFT\n",head->x,head->y,n);n++;
							if(dir!=4)
							{
								dir=3;
								img_h=img_h_left;
							}
					}break;
					case SDLK_RIGHT:
					{
							printf("(%d,%d) %d RIGHT\n",head->x,head->y,n);n++;
							if(dir!=3)
							{
								dir=4;
								img_h=img_h_right;
							}
					}break;
				}
			}
		}
		action();
		if(head->x==0 || head->x==15 || head->y==0 || head->y==15)
		{
			_break=true;
		}
		else
		{
			p=head->next;
			while(p!=NULL)
			{
				if(p->x==head->x  &&  p->y==head->y)
				{
				_break=true;
				break;
				}
				p=p->next;
			}
		}
		if(_break==true)
		break;
		draw();
		SDL_Delay(speed);
	}
	restart();
	gameover();
	SDL_Quit();
	return 0;
}
