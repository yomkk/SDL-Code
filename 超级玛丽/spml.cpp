#include<iostream>
#include<time.h>
#include<pthread.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_ttf.h>
using namespace std;
pthread_t extid;
bool quit=false;
const int Walk_img_W=15;
const int Walk_img_H=36;
const int Jump_img_W=20;
const int Jump_img_H=36;
const int Death_img_W=30;
const int Death_img_H=40;

Uint8 *keystates = SDL_GetKeyState(NULL);
int map[3][8000];
static int cnt;
SDL_Surface* screen=NULL;
TTF_Font *font=NULL,*font2=NULL;
SDL_Surface *text=NULL;
SDL_Surface* img_walk=NULL;
SDL_Surface* img_jump=NULL;
SDL_Surface* img_bj=NULL;
SDL_Surface* img_death=NULL;

SDL_Event event;
SDL_Rect Walk_R[10];
SDL_Rect Walk_L[10];
SDL_Rect Jump_R[16];
SDL_Rect Jump_L[16];
SDL_Rect Squat,Stop[2],Death[7],Ground,Hill[2],Grass[3],Cloud[3],Town,Tree[2],Pipe,Water,Flag,Enemy_1[3],Coin[2];
void Setclips();
void init();
SDL_Surface *Load_image( string filename );
void Load_file();
void init_map();
void dis(int x,int y,SDL_Surface* tu,SDL_Surface* s,SDL_Rect * clip = NULL);
void showText(SDL_Surface *msg,int x,int y,int width,int height,SDL_Surface *destination);
SDL_Surface* loadMessage(char *text,SDL_Color &textColor,char *FontFile,int size);
void * gamexit(void * argc);
void gameover();
void cleanzy();
class Mario
{
	private:
		int x,y;//人物位置
		int offSet;//
		int speed_L,speed_R,speed_U,speed_D;//
		int frame;//地图移动的像素数
		int status;//跳跃前的方向
		int score;// 分数
	public:
		Mario();
		void move(int *flag);
		void show();
		void death(int *flag);
}body;

int main()
{
	int i,flag=0;
	init();
	pthread_create(&extid,NULL,gamexit,NULL);
	while(1)
	{
		body.move(&flag);
		if(flag==1 || quit==true)
			break;
		SDL_Delay(50);
	}
	gameover();
	return 0;
}

Mario::Mario()
{
	x=0;
	y=300;
	speed_L=0;speed_R=0,speed_U=0,speed_D=0;
	frame=200;
	status=1;
	offSet=0;
	score=0;
}
void Mario::move(int *flag)
{
	int i,tmp=4,j,end=3280;
	if( keystates[ SDLK_UP ] )
	{
		if(status==1)
		{
			for(i=0;i<8;i++)
			{
				if((x<208 && frame<=200)||(x<624 && frame>=end))
				{
					x+=4;
					offSet+=4;
				}
				if(x>=208 && frame<end)
				{
					frame+=4;
					offSet+=4;
				}
				SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3C, 0xBC, 0xFF ) );
				show();
				dis(x,(y-i*12),img_jump,screen,&Jump_R[i]);
				SDL_Flip(screen);
				SDL_Delay(40);
			}
			for(i=8;i<16;i++)
			{
				if((x<208 && frame<=200)||(x<624 && frame>=end))
				{
					x+=4;
					offSet+=4;
				}
				if(x>=208 && frame<end)
				{
					frame+=4;
					offSet+=4;
				}
				if(map[1][offSet+215]==20 || map[1][offSet+215]==8)
				{
				y=265;
				break;
				}
				SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3C, 0xBC, 0xFF ) );
				show();
				dis(x,((y-12*16)+i*12),img_jump,screen,&Jump_R[i]);
				SDL_Flip(screen);
				SDL_Delay(40);
			}
		}
		if(status==0)
		{
			for(i=1;i<8;i++)
			{
				
				if((frame<=200 && x>0)||(x>208 && frame>=end))
				{
					x-=4;
					offSet-=4;
				}
				if((frame>200 && x==208)&&frame<=end)
				{
					frame-=4;
					offSet-=4;
				}
				SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3C, 0xBC, 0xFF ) );
				show();
				dis(x,(y-i*12),img_jump,screen,&Jump_L[i]);
				SDL_Flip(screen);
				SDL_Delay(40);
			}
			for(i=8;i<16;i++)
			{
				
				if((frame<=200 && x>0)||(x>208 && frame>=end))
				{
					x-=4;
					offSet-=4;
				}
				if((frame>200 && x==208)&&frame<=end)
				{
					frame-=4;
					offSet-=4;
				}
				if(map[1][offSet+215]==20 || map[1][offSet+215]==8)
				{
					y=265;
					break;
				}
				SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3C, 0xBC, 0xFF ) );
				show();
				dis(x,((y-192)+i*12),img_jump,screen,&Jump_L[i]);
				SDL_Flip(screen);
				SDL_Delay(40);
			}
		}
		if(map[2][offSet+200]==0 && map[2][offSet+215]==0)
		{
			death(flag);
		}
	}
	else if( keystates[ SDLK_DOWN ] )
	{
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3C, 0xBC, 0xFF ) );
		show();
		dis(x,(y+14),img_bj,screen,&Squat);
		SDL_Flip(screen);
	}
	else if( keystates[ SDLK_LEFT ] )	
	{
		status=0;
		for(i=0;i<10;i++)
		{
			if( keystates[ SDLK_LEFT ] )	
			{
				if(y==300)
				{
					if(((frame<=200 && x>0)||(x>208 && frame>=end))&& map[1][offSet+200]!=20)
					{
						x-=4;
						offSet-=4;
					}
					if((frame>200 && x==208)&&frame<=end &&  map[1][offSet+200]!=20)
					{
						frame-=4;
						offSet-=4;
					}
				}else
				{
					if((frame<=200 && x>0)||(x>208 && frame>=end))
					{
						x-=4;
						offSet-=4;
					}
					if((frame>200 && x==208)&&frame<=end )
					{
						frame-=4;
						offSet-=4;
					}
					if(map[1][offSet+200]!=20 && map[1][offSet+200]!=8 && map[1][offSet+215]!=20 && map[1][offSet+215]!=8)
					{
						y=300;
					}
				}
				SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3C, 0xBC, 0xFF ) );
				show();
				dis(x,y,img_walk,screen,&Walk_L[i]);
				SDL_Flip(screen);
				SDL_Delay(50);
				if(map[2][offSet+200]==0 && map[2][offSet+215]==0)
				{
					death(flag);
				}
				if(map[1][offSet+200]==11 || map[1][offSet+215]==11)
				{
					score+=100;
					if(map[1][offSet+200]==11)
						map[1][offSet+200]=0;
					else 
						map[1][offSet+215]=0;
				}
			}else break;
			if( keystates[ SDLK_UP ] )
				break;
		}
	}
	else if( keystates[ SDLK_RIGHT ] )
	{
		status=1;
		for(i=0;i<10;i++)
		{
			 if( keystates[ SDLK_RIGHT ] )
			{
				if(y==300)
				{
					if(((x<208 && frame<=200)||(x<624 && frame>=end))&& map[1][offSet+215]!=20)
					{
						x+=4;
						offSet+=4;
					}
					if((x>=208 && frame<end)&&  map[1][offSet+215]!=20)
					{
						frame+=4;
						offSet+=4;
					}
				}else
				{
					if((x<208 && frame<=200)||(x<624 && frame>=end))
					{
						x+=4;
						offSet+=4;
					}
					if(x>=208 && frame<end)
					{
						frame+=4;
						offSet+=4;
					}
					if(map[1][offSet+200]!=20 && map[1][offSet+200]!=8 && map[1][offSet+215]!=20 && map[1][offSet+215]!=8)
					{
						y=300;
					}
				}
				SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3C, 0xBC, 0xFF ) );
				show();
				dis(x,y,img_walk,screen,&Walk_R[i]);
				SDL_Flip(screen);
				SDL_Delay(50);
				if(map[2][offSet+200]==0 && map[2][offSet+215]==0)
				{
					death(flag);
				}
				if(map[1][offSet+200]==11 || map[1][offSet+215]==11 )
				{
					score+=100;
					if(map[1][offSet+200]==11)
						map[1][offSet+200]=0;
					else 
						map[1][offSet+215]=0;
				}
			}
			else break;
			if( keystates[ SDLK_UP ] )
				break;
		}
	}
	else
	{
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3C, 0xBC, 0xFF ) );
		show();
		if(map[1][offSet+200]!=20 && map[1][offSet+200]!=8 && map[1][offSet+215]!=20 && map[1][offSet+215]!=8)
		{
			y=300;
		}
		if(status==0)
			dis(x,(y+2),img_bj,screen,&Stop[0]);
		if(status==1)
			dis(x,(y+2),img_bj,screen,&Stop[1]);
		SDL_Flip(screen);
		SDL_Delay(50);
	}
	if(map[1][offSet+168]==10)
	{
		SDL_Color textColor = {255,0,0};
		SDL_FillRect(screen,NULL,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));//填充背景
		text = loadMessage("You Win!",textColor,"yayuan.ttf",30);
		showText(text,240,240,0,0,screen);
		SDL_Flip(screen);
		SDL_Delay(2000);
		cleanzy();
	}
}

void init()
{
	int i;
	TTF_Init();
	SDL_Init(SDL_INIT_EVERYTHING);
	screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	SDL_WM_SetCaption("超级玛丽",NULL);
	Setclips();
	Load_file();
	init_map();
}
void init_map()
{
	int i,j;
	for(i=0;i<4000;i+=400)
		map[0][i]=3;
	for(i=0;i<4000;i++)
	{
		map[2][i]=1;
	}
	for(i=0;i<4000;i+=16)
	{
		map[2][i]=2;
	}
	
	for(i=1296;i<1328;i++)
	{
		map[2][i]=0;
	}
	for(i=2400;i<2432;i++)
	{
		map[2][i]=0;
	}
	
	for(i=0;i<4000;i+=725)
	{
		map[1][i]=8;
		for(j=i+1;j<=i+32;j++)
		{
			map[1][j]=20;
		}
		
	}
	for(i=0;i<4000;i+=500)
	{
		map[1][i+100]=1;
		map[1][i+200]=2;
		map[1][i+500]=5;
	}
	for(i=0;i<4000;i+=800)
	{
		map[1][i+100]=6;
		map[1][i+400]=7;
	}
	map[1][3800]=10;
	for(i=460;i<590;i+=32)
	{
		map[1][i]=11;
	}
	for(i=860;i<990;i+=32)
	{
		map[1][i]=11;
	}
	for(i=1660;i<1790;i+=32)
	{
		map[1][i]=11;
	}
	
}
void dis(int x,int y,SDL_Surface * tu,SDL_Surface * s,SDL_Rect * clip)
{
	SDL_Rect xx;
	xx.x=x;
	xx.y=y;
	SDL_BlitSurface(tu,clip,s,&xx);
}
SDL_Surface *Load_image(string filename )
{
	SDL_Surface* tmp,*tmp2;
	tmp=IMG_Load(filename.c_str());
	tmp2=SDL_DisplayFormat(tmp);
	SDL_FreeSurface( tmp );
	 SDL_SetColorKey( tmp2, SDL_SRCCOLORKEY, SDL_MapRGB( tmp2->format, 0xFF, 0xFF, 0xFF ) );
	return tmp2;
}
void Load_file()
{
	img_walk=Load_image("walk.png");
	img_jump=Load_image("jump.png");
	img_bj=Load_image("bj.png");
	img_death=Load_image("death.png");
	font = TTF_OpenFont("yayuan.ttf",20);
}
void Setclips()
{
	int i;
	for(i=0;i<10;i++)
	{
		Walk_R[i].x=Walk_img_W*i;
		Walk_R[i].y=0;
		Walk_R[i].w=Walk_img_W;
		Walk_R[i].h=Walk_img_H;
	}
	for(i=0;i<10;i++)
	{
		Walk_L[i].x=300-Walk_img_W*(i+1);
		Walk_L[i].y=0;
		Walk_L[i].w=Walk_img_W;
		Walk_L[i].h=Walk_img_H;
	}
	for(i=0;i<16;i++)
	{
		Jump_R[i].x=Jump_img_W*i;
		Jump_R[i].y=0;
		Jump_R[i].w=Jump_img_W;
		Jump_R[i].h=Jump_img_H;
	}
	for(i=0;i<16;i++)
	{
		Jump_L[i].x=640-Jump_img_W*(i+1);
		Jump_L[i].y=0;
		Jump_L[i].w=Jump_img_W;
		Jump_L[i].h=Jump_img_H;
	}
	for(i=0;i<7;i++)
	{
		Death[i].x=Death_img_W*i;
		Death[i].y=0;
		Death[i].w=Death_img_W;
		Death[i].h=Death_img_H;
	}
	Squat.x=165;Squat.y=173;Squat.w=16;Squat.h=22;//蹲下
	Stop[0].x=208;Stop[0].y=168;Stop[0].w=16;Stop[0].h=34;//停止zuo
	Stop[1].x=185;Stop[1].y=168;Stop[1].w=16;Stop[1].h=34;//停止you
	Ground.x=187;Ground.y=59;Ground.w=16;Ground.h=16;//地面
	Town.x=38;Town.y=114;Town.w=80;Town.h=79;//城堡
	Hill[0].x=2;Hill[0].y=22;Hill[0].w=48;Hill[0].h=19;//小山
	Hill[1].x=53;Hill[1].y=6;Hill[1].w=80;Hill[1].h=35;//大山
	Grass[0].x=7;Grass[0].y=95;Grass[0].w=32;Grass[0].h=16;//一丛草
	Grass[1].x=107;Grass[1].y=95;Grass[1].w=48;Grass[1].h=16;//两丛草
	Grass[2].x=41;Grass[2].y=95;Grass[2].w=64;Grass[2].h=16;//三丛草
	Cloud[0].x=118;Cloud[0].y=42;Cloud[0].w=32;Cloud[0].h=24;//一片云
	Cloud[1].x=2;Cloud[1].y=42;Cloud[1].w=48;Cloud[1].h=24;//两片云
	Cloud[2].x=52;Cloud[2].y=42;Cloud[2].w=64;Cloud[2].h=24;//三片云
	Tree[0].x=157;Tree[0].y=22;Tree[0].w=14;Tree[0].h=30;//小树
	Tree[1].x=174;Tree[1].y=6;Tree[1].w=16;Tree[1].h=46;//大树
	Pipe.x=123;Pipe.y=114;Pipe.w=32;Pipe.h=36;//水管
	Water.x=123;Water.y=169;Water.w=32;Water.h=11;//水
	Flag.x=231;Flag.y=23;Flag.w=24;Flag.h=168;//旗子
	Enemy_1[0].x=166;Enemy_1[0].y=133;Enemy_1[0].w=16;Enemy_1[0].h=16;//螃蟹
	Enemy_1[1].x=187;Enemy_1[1].y=133;Enemy_1[1].w=16;Enemy_1[1].h=16;//螃蟹
	Enemy_1[2].x=207;Enemy_1[2].y=140;Enemy_1[2].w=16;Enemy_1[2].h=8;//螃蟹踩扁
	Coin[0].x=165;Coin[0].y=81;Coin[0].w=10;Coin[0].h=14;
	Coin[1].x=165;Coin[1].y=98;Coin[1].w=10;Coin[1].h=14;
}
void Mario:: show()
{
	int i,j=0;
	char text1[10];
	SDL_Color textColor = {255,255,255};
	for(i=frame-120;i<frame+720;i++)
	{
		if(map[0][i]==1)
			dis(i-frame,200,img_bj,screen,&Cloud[0]);
		if(map[0][i]==2)
			dis(i-frame,200,img_bj,screen,&Cloud[1]);
		if(map[0][i]==3)
			dis(i-frame,200,img_bj,screen,&Cloud[2]);//云
			
		if(map[1][i]==1)
			dis(i-frame,306,img_bj,screen,&Tree[0]);
		if(map[1][i]==2)
			dis(i-frame,290,img_bj,screen,&Tree[1]);//树
		if(map[1][i]==3)
		
			dis(i-frame,319,img_bj,screen,&Grass[0]);
		if(map[1][i]==4)
			dis(i-frame,319,img_bj,screen,&Grass[1]);
		if(map[1][i]==5)
			dis(i-frame,319,img_bj,screen,&Grass[2]);//草
		
		if(map[1][i]==6)
			dis(i-frame,316,img_bj,screen,&Hill[0]);
		if(map[1][i]==7)
			dis(i-frame,300,img_bj,screen,&Hill[1]);//山
		
		if(map[1][i]==8)
			dis(i-frame,299,img_bj,screen,&Pipe);//水管
		
		if(map[1][i]==9)
			dis(i-frame,167,img_bj,screen,&Flag);//旗
		
		if(map[1][i]==10)
			dis(i-frame,256,img_bj,screen,&Town);//城堡
		
		if(map[1][i]==11)
			dis(i-frame,310,img_bj,screen,&Coin[0]);//金币
		
		if(map[2][i]==2)
			dis(i-frame,334,img_bj,screen,&Ground);
	}
	sprintf(text1,"%d",score);
	text = TTF_RenderText_Solid(font,"Score: ",textColor);
	showText(text,450,50,0,0,screen);
	text =TTF_RenderText_Solid(font,text1,textColor);
	showText(text,530,50,0,0,screen);
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
void * gamexit(void * argc)
{
	while(quit==false)
	{
		while(SDL_WaitEvent(&event))
		{
			if(event.type==SDL_QUIT || event.key.keysym.sym==SDLK_ESCAPE)
			quit=true;
		}
	}
	gameover();
	exit(0);
}
void Mario::death(int *flag)
{
	int i;
	y+=30;
	for(i=0;i<3;i++)
	{
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3C, 0xBC, 0xFF ) );
		show();
		dis(x,y,img_death,screen,&Death[i]);
		SDL_Flip(screen);
		SDL_Delay(100);
		y-=30;
	}
	for(i=3;i<7;i++)
	{
	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3C, 0xBC, 0xFF ) );
	show();
	dis(x,y,img_death,screen,&Death[i]);
		SDL_Flip(screen);
		SDL_Delay(100);
		y+=30;
	}
	*flag=1;
	quit=true;
	SDL_Delay(3000);
}
void gameover()
{
	SDL_Color textColor = {255,0,0};
	SDL_FillRect(screen,NULL,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));//填充背景
	text = loadMessage("GAME OVER!",textColor,"yayuan.ttf",30);
	showText(text,240,240,0,0,screen);
	SDL_Flip(screen);
	SDL_Delay(2000);
	cleanzy();
}
void cleanzy()
{
	if(cnt==0)
	{
	cnt++;
	SDL_FreeSurface(img_walk);
	SDL_FreeSurface(img_jump);
	SDL_FreeSurface(img_bj);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(text);
	TTF_CloseFont(font);
	SDL_Quit();
	exit(0);
	}
}
