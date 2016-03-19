#include<iostream>
#include<fstream>
#include<pthread.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_ttf.h>
#include<SDL/SDL_rotozoom.h>
#include<SDL/SDL_gfxPrimitives.h>
#define  LEN sizeof(struct file)
struct file * head=NULL;
pthread_t extid;
bool quit = false;
using namespace std;
void init();
void Load_file();
void clean();
void * softexit(void * argc);
void dis(int x,int y,SDL_Surface * tu,SDL_Surface * s,SDL_Rect * clip);
void play();
SDL_Surface *Load_image(string filename );

Uint8 *keystates = SDL_GetKeyState(NULL);
SDL_Surface *screen=NULL;
SDL_Surface *img=NULL;
SDL_Event event;
double zoom=1;
struct file
{
	string file_name;
	struct file* next;
	struct file*front;
};
int main()
{
	init();
	pthread_create(&extid,NULL,softexit,NULL);
	play();
	clean();
	return 0;
}

void init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	SDL_WM_SetCaption("图片浏览器",NULL);
	Load_file();
}
void Load_file()
{
	string tmpfile;
	struct file *p,*p1;
	system("ls *.jpg> file.txt"); 
	system("ls *.gif>> file.txt"); 
	system("ls *.bmp>> file.txt"); 
	system("ls *.png>> file.txt"); 
	ifstream is("file.txt",ios_base::in);
	if(!is)
	{
		return ;
	}
	head=NULL;
	while(is)
	{
		p1=p;
		p=new struct file;
		if(head==NULL)
		{
			head=p;
			p->front=head;
		}
		else
		{
			p1->next=p;
			p->front=p1;
		}
		is>>p->file_name;
		p->next=head;
		head->front=p;
	}
	delete p;
	p1->next=head;
	head->front=p1;
	is.close();
	
	
}
void dis(int x,int y,SDL_Surface * tu,SDL_Surface * s,SDL_Rect * clip)
{
	SDL_FillRect(screen,NULL,SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ));//填充背景
	SDL_Rect xx;
	xx.x=x;
	xx.y=y;
	SDL_BlitSurface(tu,clip,s,&xx);
	SDL_Flip(screen);
	SDL_FreeSurface(img);
}
SDL_Surface *Load_image(string filename )
{
	SDL_Surface* tmp,*tmp2;
	tmp=IMG_Load(filename.c_str());
	tmp2=SDL_DisplayFormat(tmp);
	SDL_FreeSurface( tmp );
	return tmp2;
}
void play()
{
	struct file *p;
	p=head;
	SDL_Rect rect;
	rect.x=0;rect.y=0;
	img=Load_image(p->file_name.c_str());
	dis(rect.x,rect.y,img,screen,NULL);
	int center_x,center_y,py_x,py_y;
	center_x=320;center_y=240;py_x=0;py_y=0;
	while(quit==false)
	{
		if(head!=NULL)
		{
			if( keystates[ SDLK_LEFT ] )	
			{
				cout<<"left"<<endl;
				p=p->front;
				SDL_WM_SetCaption(p->file_name.c_str(),NULL);
				zoom=1;py_x=0;py_y=0;
				img=Load_image(p->file_name.c_str());
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_RIGHT ] )
			{
				cout<<"right"<<endl;
				SDL_WM_SetCaption(p->file_name.c_str(),NULL);
				p=p->next;
				zoom=1;py_x=0;py_y=0;
				img=Load_image(p->file_name.c_str());
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_UP ] )	
			{
				cout<<"up"<<endl;
				img=Load_image(p->file_name.c_str());
				zoom+=0.05;
				img=zoomSurface(img, zoom , zoom, 1);
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_DOWN] )	
			{
				cout<<"down"<<endl;
				img=Load_image(p->file_name.c_str());
				if(zoom>0)
				{
					zoom-=0.05;
					img=zoomSurface(img, zoom , zoom, 1);
					rect.x=center_x-img->w/2+py_x;
					rect.y=center_y-img->h/2+py_y;
					dis(rect.x,rect.y,img,screen,NULL);
				}
			}
			
			if( keystates[ SDLK_s] )	
			{
				cout<<"s"<<endl;
				img=Load_image(p->file_name.c_str());
				img=zoomSurface(img, zoom , zoom, 1);
				py_y-=10;
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_w] )	
			{
				cout<<"w"<<endl;
				img=Load_image(p->file_name.c_str());
				img=zoomSurface(img, zoom , zoom, 1);
				py_y+=10;
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_a] )	
			{
				cout<<"a"<<endl;
				img=Load_image(p->file_name.c_str());
				img=zoomSurface(img, zoom , zoom, 1);
				py_x+=10;
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_d] )	
			{
				cout<<"d"<<endl;
				img=Load_image(p->file_name.c_str());
				img=zoomSurface(img, zoom , zoom, 1);
				py_x-=10;
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
		}
		SDL_Delay(100);
	}
	
}

void clean()
{
	//SDL_FreeSurface(img);
	SDL_FreeSurface(screen);
	SDL_Quit();
}
void * softexit(void * argc)
{
	
	while(quit==false)
	{
		while(SDL_WaitEvent(&event))
		{
			if(event.type==SDL_QUIT || event.key.keysym.sym==SDLK_ESCAPE)
			{
				quit=true;
				break;
			}
		}
	}
	clean();
	exit(0);
}
