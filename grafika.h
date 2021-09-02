#ifndef GRAFIKA_H_INCLUDED
#define GRAFIKA_H_INCLUDED

/*A cellák kirajzolásához nyújt segítségét, az Empty a rekurzív floodfill függvénynél játszik fontos szerepet*/
typedef enum Cell_Type {NotRevealed, Flag, Empty, Mine, Number_1, Number_2, Number_3, Number_4, Number_5, Number_6, Number_7, Number_8} Cell_Type;

void sdl_init(char const *text, int length, int height, SDL_Window **pwindow, SDL_Renderer **prenderer);
void draw_cell(SDL_Renderer *renderer, SDL_Texture *cell_image, Cell_Type which, int x, int y);
void draw_text(char *draw, int place, SDL_Surface *text, TTF_Font *font, SDL_Texture *text_t, SDL_Renderer *renderer, SDL_Rect *where);

#endif // GRAFIKA_H_INCLUDED
