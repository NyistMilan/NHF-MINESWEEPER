#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "grafika.h"

/*SDL inicializ�l�sa �s ablak megnyit�sa*/
void sdl_init(char const *text, int lenght, int height, SDL_Window **pwindow, SDL_Renderer **prenderer){
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(text, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, lenght, height, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

/*A Cell_Type adatszerkezettel haszn�lhat�. A f�ggv�ny a Minesweeper_Tiles.png-b�l kiv�gja a k�v�nt grafik�t �s megjelen�ti az ablakon*/
void draw_cell(SDL_Renderer *renderer, SDL_Texture *cell_image, Cell_Type which, int x, int y){
    SDL_Rect src = { (which % 12) * 20, (which / 12) * 20, 20, 20};
    SDL_Rect dest = {x, y, 20, 20};
    SDL_RenderCopy(renderer, cell_image, &src, &dest);
}

/*Adott sz�veget jelen�t meg az ablakon, megadhat� param�ter az elhelyezked�se a sz�vegnek �s maga a sz�veg*/
void draw_text(char *draw, int place, SDL_Surface *text, TTF_Font *font, SDL_Texture *text_t, SDL_Renderer *renderer, SDL_Rect *where){
    SDL_Color black = {0, 0, 0};
    text = TTF_RenderUTF8_Solid(font, draw, black);
    text_t = SDL_CreateTextureFromSurface(renderer, text);
    where->x = (500 - text->w) / 2;
    where->y = place;
    where->w = text->w;
    where->h = text->h;
    SDL_RenderCopy(renderer, text_t, NULL, where);
}
