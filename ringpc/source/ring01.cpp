/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_bool done = SDL_FALSE;
//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;
SDL_Surface* screenbuffer = NULL;

//The font that's going to be used
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 255, 255, 255 };

const int WIDTH = 640;
const int HEIGHT = 480;

#define ALIGN_CENTER 0
#define ALIGN_LEFT   1
#define ALIGN_RIGHT  2
static int draw_string(SDL_Surface *dst, TTF_Font *font, SDL_Rect &rect, const char *str, SDL_Color &color, int align) {
	SDL_Rect  drect;

	if (!font) {
		return 0;
	}

    //SDL_Surface * text = TTF_RenderUTF8_Solid(font, str, color);
	SDL_Surface * text = TTF_RenderUTF8_Blended(font, str, color);
	
    if(align == ALIGN_CENTER){
		drect.x = rect.x + (rect.w - text->w) / 2;
		drect.y = rect.y + (rect.h - text->h) / 2;

	}else if(align == ALIGN_LEFT){
		drect.x = rect.x;
		drect.y = rect.y;
	}else{
		drect.x = rect.x + rect.w - text->w;
		drect.y = rect.y;
	}
	drect.w = text->w;
	drect.h = text->h;

    SDL_BlitSurface(text, NULL, dst, &drect);
    int textlen = text->w;
    SDL_FreeSurface(text);
	return textlen;
}

void init(void)
{   
    TTF_Init();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

  
    screen = SDL_SetVideoMode(WIDTH, HEIGHT,  32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
    if (screen == NULL) {
        printf("Couldn't initialize display: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    screenbuffer = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
   


    font = TTF_OpenFont("Garuda.ttf", 32);

    SDL_ShowCursor(SDL_DISABLE);
    background = IMG_Load("background.png");
   
}

void quit(void)
{
    if(background) SDL_FreeSurface(background);
    if(screenbuffer) SDL_FreeSurface(screenbuffer);
    if(screen) SDL_FreeSurface(screen);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}

void handle_keydown(SDLKey key)
{
    switch (key) {
    case SDLK_UP:
        done = SDL_TRUE;
        break;

    default:
        break;
    }
}

int main( int argc, char* args[] )
{
	 SDL_Rect rect;
    init();
    static SDL_Color white={0xF0, 0xF0, 0xF0, 0};
    
    rect.x = (WIDTH - background->w)/2;
    rect.y = (HEIGHT - background->h)/2;
    SDL_BlitSurface(background, NULL, screenbuffer, &rect);
    
      
    rect.x = 0;
    rect.y = 0;
    rect.w = WIDTH;    
    rect.h = TTF_FontHeight(font);
    
    draw_string(screenbuffer, font, rect, "Hello World!", white, ALIGN_RIGHT);
            

    
    while (!done) {
    	SDL_Event event;
		SDL_BlitSurface(screenbuffer, NULL, screen, NULL);
      SDL_Flip(screen);
      SDL_WaitEvent(&event);
      switch (event.type) {
        case SDL_KEYDOWN:
            handle_keydown(event.key.keysym.sym);
            break;
        default:
            break;
        }
      }
 	 quit();
    return EXIT_SUCCESS;
}
