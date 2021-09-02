#ifndef JATEKVEZERLES_H_INCLUDED
#define JATEKVEZERLES_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include "grafika.h"
#include "menu.h"

/*A pályán lévõ celláknak tartalmazza különbözõ tulajdonságait*/
typedef struct Cell
{
    int x_coord, y_coord;
    int neighbours;
    bool flag;
    bool revealed;
    Cell_Type type;
} Cell;

/*Pálya adatstruktúra ami tartalmazza a Cell-t is, továbbá a pálya méreteit, aknák számát és a játékidõt*/
typedef struct Board
{
    int lenght;
    int height;
    int mines;
    Uint32 time;
    Cell **cell;

} Board;

void play_loaded_game(SDL_Renderer *renderer, SDL_Texture *image, Board *board);
void floodfill(SDL_Renderer *renderer, SDL_Texture *tiles, int i, int j, Board *board);
bool game_check_win(Board *board);
bool choose_game_mode(Game_Mode game_mode, Board *board, SDL_Renderer *renderer, SDL_Texture *image, FILE *fp);
void play_game(SDL_Renderer *renderer, SDL_Texture *image, Board *board);

#endif // JATEKVEZERLES_H_INCLUDED
