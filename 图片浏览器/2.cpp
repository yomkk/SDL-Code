#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include <string.h>
#include <SDL/SDL_gfxPrimitives.h>
 
#define MAX  16
#define BMP_NAME "icon.bmp"
#define ICON "sample.bmp"
#define TITLE_NAME "TestSDL"
#define ICON_NAME  "My WinFrame"        //窗体最小化name
 
 
#define TEXT "欢迎光临"             //文字设置
#define TEXT_SIZE 50                //设置字体大小
 
#define WIDTH  640
#define HEIGTH 480
 
char *picture[] = {
          "1.jpg"  ,  "2.jpg"  ,  "3.jpg"  ,  "4.jpg" , 
          "5.jpg"  ,  "6.jpg"  ,  "7.jpg"  ,  "8.jpg" ,
          "9.jpg"  ,  "10.jpg" ,  "11.jpg" ,  "12.jpg" ,
          "13.png" ,  "14.jpg" ,  "15.jpg" ,  "16.jpg"
          };
static SDL_Surface *screen = NULL;
static  SDL_Surface *backpng = NULL;
static  SDL_Surface *png = NULL;
     
static double zoomXY = 1.5;
static int i = 0;
 
 
int Init()                      //初始化SDL
{
    if((SDL_Init(SDL_INIT_VIDEO)|IMG_Init(IMG_INIT_PNG)|TTF_Init()) == -1)
    {
        fprintf(stderr,"SDL init error:%s",SDL_GetError());
        return -1;
    }
    return 0;
}
 
SDL_Surface *loadBMP(char *fileName)            //加载bmp图片
{
    SDL_Surface *bmp;
    bmp = SDL_LoadBMP(fileName);
    if(bmp == NULL)
    {
        fprintf(stderr,"Could not load %s: %s",fileName,SDL_GetError());
        exit(1);
    }
    return bmp;
}
 
void creatScreen(int width , int height , int bpp , Uint32 flags)       //创建一个VideoMode
{
    screen = SDL_SetVideoMode(width , height,  bpp , flags);
    if(screen == NULL)
    {
        fprintf(stderr,"Could not Creat a Screen!:%s",SDL_GetError());
        exit(1);
    }
    return ;
}
 
void show_Pic(SDL_Surface *bmp ,SDL_Rect *rect) //显示bmp图片
{
    SDL_BlitSurface(bmp , NULL , screen , rect);
    SDL_UpdateRect(screen , 0 , 0 , 0 , 0 );
    SDL_FreeSurface(bmp);
    return ;
}
 
int getRightPic(int a)
{
     
    a++;
    if(a > 15 )
    {
        a = 0;
    }
    return a; 
}
int  getLeftPic(int a)
{
    a--;
    if(a < 0)
    {
        a = 15;
    }
    return a; 
}
void zoomBig(double *a)
{   
    *a -= 0.25;
    if(*a <= 1.5)
    {
        *a = 1.5;
    }
     
}
void zoomSmall(double *a)
{
 
    *a += 0.25; 
    if(*a >= 5)
    {
        *a = 5;
    }   
}
int mytext(char *string , int size , SDL_Rect *rect)
{
         
    SDL_Rect text_rect;
    //double zoomx , zoomy;
     
    SDL_Color color = {0 , 0 , 0};
    TTF_Font  *font = TTF_OpenFont("simfang.ttf", size);    
    if(font == NULL)
    {
        fprintf( stderr, "font open failed: %s\n",TTF_GetError()  );
        exit(1);
 
    }
    SDL_Surface  *text = TTF_RenderUTF8_Solid(font , string  , color );
    if(text == NULL)
    {
        fprintf(stderr , "text failed: %s\n" , TTF_GetError() );
    }
     
    /*
    //将字体缩放到按钮的大小，但是字体显得不是那么圆润。
    zoomx = (double)((double)rect->w / (double)text->w);
    zoomy = (double)((double)rect->h / (double)text->h);
    printf("%lf\n" , zoomy);
    text = zoomSurface(text, zoomx, zoomy, 1);
    */
     
    //将字体放在按钮的中间位置
    text_rect.x =rect->x+(rect->w / 2 -text->w / 2);
    text_rect.y = rect->y + (rect->h / 2 - text->h /2);
     
    show_Pic(text , &text_rect);
    //SDL_FreeSurface(text);
    TTF_CloseFont(font);
         
    return 0;
}
void draw_button(char *str , int flag)
{   
    SDL_Rect myrect;
    if(strcmp(str , "left") == 0)
    {
         
        myrect.x = 160 ;
        myrect.y = 420 ; 
        myrect.w = 40;
        myrect.h = 40;
        boxColor(screen, myrect.x , myrect.y , myrect.x + myrect.w , myrect.y + myrect.h, 0xf3f5ffff);
        if(flag == 1)
        {
            hlineColor( screen, myrect.x , myrect.x + myrect.w , myrect.y + myrect.h , 0x000000ff);
            vlineColor( screen, myrect.x + myrect.w , myrect.y , myrect.y + myrect.h , 0x000000ff);
        }
        mytext("<<" , 20 , &myrect);
    }
    if(strcmp(str , "right") == 0)
    {   
         
        myrect.x = 438;
        myrect.y = 420;
        myrect.w = 40;
        myrect.h = 40;
        boxColor(screen, myrect.x , myrect.y , myrect.x + myrect.w , myrect.y + myrect.h, 0xf3f5ffff);
        if(flag == 1)
        {
            hlineColor( screen, myrect.x , myrect.x + myrect.w , myrect.y + myrect.h , 0x000000ff);
            vlineColor( screen, myrect.x + myrect.w , myrect.y , myrect.y + myrect.h , 0x000000ff);
        }
        mytext(">>" , 20 , &myrect);
    }
    if(strcmp(str , "big") == 0)
    {
        myrect.x = 160;
        myrect.y = 20;
        myrect.w = 40;
        myrect.h = 40;
        boxColor(screen, myrect.x , myrect.y , myrect.x + myrect.w , myrect.y + myrect.h, 0xf3f5ffff);
        if(flag == 1)
        {
            hlineColor( screen, myrect.x , myrect.x + myrect.w , myrect.y + myrect.h , 0x000000ff);
            vlineColor( screen, myrect.x + myrect.w , myrect.y , myrect.y + myrect.h , 0x000000ff);
        }
        mytext("+" , 20 , &myrect);
    }
    if(strcmp(str , "small") == 0)
    {
        myrect.x = 438;
        myrect.y = 20;
        myrect.w = 40;
        myrect.h = 40;
        boxColor(screen, myrect.x , myrect.y , myrect.x + myrect.w , myrect.y + myrect.h, 0xf3f5ffff);
        if(flag == 1)
        {
            hlineColor( screen, myrect.x , myrect.x + myrect.w , myrect.y + myrect.h , 0x000000ff);
            vlineColor( screen, myrect.x + myrect.w , myrect.y , myrect.y + myrect.h , 0x000000ff);
        }
        mytext("-" , 20 , &myrect);
    }
    show_Pic( screen , &(screen->clip_rect));
    return ;
}
int ShowPic(char *string)
{
    double zoom_x,zoom_y;
    SDL_Rect fillRect;
     
    fillRect.x = 107;
    fillRect.y = 80;
    fillRect.w = 426;
    fillRect.h = 320;
    if(strcmp(string , "left") == 0)
    {   
        i = getLeftPic(i);  
    }
    else
        i = getRightPic(i);
    backpng = IMG_Load(picture[i]);
    if(!backpng)
    {
        fprintf(stderr,"Could not load %s: %s\n",picture[i],SDL_GetError());
        exit(1);    
    }
    zoom_x = (double)((screen->w) / (double)(backpng->w));
    zoom_y = (double)((screen->h) / (double)(backpng->h));
     
    png = zoomSurface(backpng , (zoom_x / zoomXY), (zoom_y / zoomXY) , 1);
    SDL_Rect rect;
    rect.x =  (screen->w - png->w ) / 2;
    rect.y =  (screen->h - png->h ) / 2;
                 
    rect.w = png->w;
    rect.h = png->h;
    printf("%d , %d , %d , %d\n" , rect.x, rect.y, rect.w , rect.h );
    SDL_FillRect(screen ,&fillRect , 0xffffffff);
    SDL_UpdateRect(screen , 0 , 0 , 0 , 0 );
    show_Pic(png , &rect);
    SDL_FreeSurface(backpng);
                     
    return 0;
}
int getNowPic()
{
    double zoom_x,zoom_y;
    SDL_Rect fillRect;
     
    fillRect.x = 107;
    fillRect.y = 80;
    fillRect.w = 426;
    fillRect.h = 320;
         
    backpng = IMG_Load(picture[i]);
    if(!backpng)
    {
        fprintf(stderr,"Could not load %s: %s\n",picture[i],SDL_GetError());
        exit(1);    
    }
    zoom_x = (double)((screen->w) / (double)(backpng->w));
    zoom_y = (double)((screen->h) / (double)(backpng->h));
     
    png = zoomSurface(backpng , (zoom_x / zoomXY), (zoom_y / zoomXY) , 1);
    SDL_Rect rect;
    rect.x =  (screen->w - png->w ) / 2;
    rect.y =  (screen->h - png->h ) / 2;
                 
    rect.w = png->w;
    rect.h = png->h;
    printf("%d , %d , %d , %d\n" , rect.x, rect.y, rect.w , rect.h );
    SDL_FillRect(screen ,&fillRect , 0xffffffff);
    SDL_UpdateRect(screen , 0 , 0 , 0 , 0 );
    show_Pic(png , &rect);
    SDL_FreeSurface(backpng);
}
int chooseButton(char *string)
{   
     
    draw_button(string , 1) ;
    ShowPic(string);
         
    return 0;
}
int do_Button(SDL_Event *event)
{
    if((((event->button).x >= 160) && ((event->button).x <= 200 )) && (((event->button).y >= 420) && ((event->button).y <= 460 )))
    {
        chooseButton("left");
    }
    if( ((event->button).x >= 438 && (event->button).x <= 478 ) && ((event->button).y >= 420 && (event->button).y <= 460 ) )
    {
        chooseButton("right");
    }
    if( ((event->button).x >= 160 && (event->button).x <= 200 ) && ((event->button).y >= 20 && (event->button).y <= 60 ) )
    {
        zoomBig(&zoomXY);
        draw_button("big" , 1) ;
        getNowPic();
         
    }
    if( ((event->button).x >= 438 && (event->button).x <= 478 ) && ((event->button).y >= 20 && (event->button).y <= 60 ) )
    {
        zoomSmall(&zoomXY);
        draw_button("small" , 1) ;
        getNowPic();
    }
    return 0;
}
int background()
{
    double zoom_x , zoom_y;
    backpng = IMG_Load(picture[0]);
    if(!backpng)
    {
        fprintf(stderr,"Could not load %s: %s\n",picture[0],SDL_GetError());
        exit(1);    
    }
    zoom_x = (double)((screen->w) / (double)(backpng->w));
    zoom_y = (double)((screen->h) / (double)(backpng->h));
 
    png = zoomSurface(backpng , zoom_x , zoom_y , 1);
    show_Pic(png ,&(screen->clip_rect));
    SDL_FreeSurface(backpng);
    return 0;
}
 
int appMode()
{
     
    int width = WIDTH;
    int heigth = HEIGTH;
    int bpp = 0;
 
    Init();
    creatScreen(width , heigth, bpp , SDL_SWSURFACE);
 
    background();
    SDL_WM_SetCaption(TITLE_NAME, ICON_NAME);               //设置窗口标题
    SDL_WM_SetIcon(loadBMP(ICON) , NULL);   
     
    return 0;
}
int appEvent()
{
     
    double zoom_x,zoom_y;
    SDL_Event event;
 
    while(SDL_WaitEvent(&event))            
    {   
        switch(event.type)
        {
            case SDL_KEYDOWN:
                printf("Key Down......\n");
                if(event.key.keysym.sym == SDLK_ESCAPE)
                {
                    goto loop;
                }
                if(event.key.keysym.sym == SDLK_SPACE)
                {
                    background();       
                }   
                break;
                case SDL_KEYUP:
                    /*
                    *keyborad test
                    */
                    printf("Key up......\n");
                    printf("key : %d\n" , event.key.keysym.sym );
                    if(event.key.keysym.sym == 269)
                    {
                        zoomSmall(&zoomXY); 
                        getNowPic();                
                    }
                    if(event.key.keysym.sym == 270)
                    {
                        zoomBig(&zoomXY);
                        getNowPic();
                    }
                    break;
                case SDL_MOUSEMOTION:
 
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    draw_button("big" , 0);
                    draw_button("small" , 0);
                    draw_button("left" , 0) ;
                    draw_button("right" , 0) ; 
                     
                     
                    do_Button(&event);
                     
                    break;
                case SDL_MOUSEBUTTONUP: 
                    draw_button("left" , 0) ;
                    draw_button("right" , 0) ;
                    draw_button("big" , 0);
                    draw_button("small" , 0);
                    break;
                case SDL_QUIT:
                    printf("quit\n");
                    goto loop;
                    break;
            }
        }
loop:
    IMG_Quit();
    SDL_FreeSurface(screen);
    SDL_Quit();     
    return 0;
}
 
int main(int argc,char **argv)
{
    appMode();
    appEvent();     
    return 0;
}
