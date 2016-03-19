#include<stdio.h>
#include<pthread.h>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_ttf.h>
#include<SDL/SDL_rotozoom.h>
#include<SDL/SDL_gfxPrimitives.h>
#define  LEN sizeof(struct file)
struct file * head=NULL;
pthread_t extid;
bool quit;
void init();
void Load_file();
void clean();
void * softexit(void * argc);
void dis(int x,int y,SDL_Surface * tu,SDL_Surface * s,SDL_Rect * clip);
void play();
SDL_Surface *Load_image(char* filename );

Uint8 *keystates = SDL_GetKeyState(NULL);
SDL_Surface *screen=NULL;
SDL_Surface *img=NULL;
SDL_Event event;
double zoom=1;
struct file
{
	char* file_name;
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
	quit=false;
	screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	SDL_WM_SetCaption("图片浏览器",NULL);
	Load_file();
}
void Load_file()
{
	char ch;
	FILE *fp;
	struct file *p,*p1,*temp;
	system("ls *.jpg> file.txt"); 
	system("ls *.gif>> file.txt"); 
	system("ls *.bmp>> file.txt"); 
	system("ls *.png>> file.txt"); 
        fp=fopen("file.txt","r+");
        p=malloc(LEN);
        head=p;
        p->front=head;
        p->next=head;
        while(!feof(fp))
        {
           fscanf(fp,"%s ",&p->file_name);
           p1=malloc(LEN);
           temp=p;
           p->next=p1;
           p1->front=p;
           p=p1;
       }
       temp->next=head;
       head->front=temp;
       free(p1);
       fclose(fp);
       return ;
	
	
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
SDL_Surface *Load_image(char* filename )
{
	SDL_Surface* tmp,*tmp2;
	tmp=IMG_Load(filename);
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
	img=Load_image(p->file_name);
	dis(rect.x,rect.y,img,screen,NULL);
	int center_x,center_y,py_x,py_y;
	center_x=320;center_y=240;py_x=0;py_y=0;
	while(quit==false)
	{
		if(head!=NULL)
		{
			if( keystates[ SDLK_LEFT ] )	
			{
				printf("left");
				p=p->front;
				SDL_WM_SetCaption(p->file_name,NULL);
				zoom=1;py_x=0;py_y=0;
				img=Load_image(p->file_name);
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_RIGHT ] )
			{
				printf("right");
				SDL_WM_SetCaption(p->file_name.c_str(),NULL);
				p=p->next;
				zoom=1;py_x=0;py_y=0;
				img=Load_image(p->file_name);
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_UP ] )	
			{
				printf("up");
				img=Load_image(p->file_name);
				zoom+=0.05;
				img=zoomSurface(img, zoom , zoom, 1);
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_DOWN] )	
			{
				printf("down");
				img=Load_image(p->file_name);
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
				printf("s");
				img=Load_image(p->file_name);
				img=zoomSurface(img, zoom , zoom, 1);
				py_y-=10;
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_w] )	
			{
				printf("w");
				img=Load_image(p->file_name);
				img=zoomSurface(img, zoom , zoom, 1);
				py_y+=10;
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_a] )	
			{
				printf("a");
				img=Load_image(p->file_name);
				img=zoomSurface(img, zoom , zoom, 1);
				py_x+=10;
				rect.x=center_x-img->w/2+py_x;
				rect.y=center_y-img->h/2+py_y;
				dis(rect.x,rect.y,img,screen,NULL);
			}
			if( keystates[ SDLK_d] )	
			{
				printf("d");
				img=Load_image(p->file_name);
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
