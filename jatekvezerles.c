#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdbool.h>

#include "jatekvezerles.h"
#include "debugmalloc.h"

/*Kirajzolja a fájlból olvasott pályát*/
void play_loaded_game(SDL_Renderer *renderer, SDL_Texture *image, Board *board)
{
    boxRGBA(renderer, 0, 0, 500, 500, 189, 189, 189, 255);
    int x_pixel = 50;
    int y_pixel = 50;

    for(int i=0; i<board->height; i++)
    {
        for(int j=0; j<board->lenght; j++)
        {
            board->cell[i][j].x_coord = x_pixel;
            board->cell[i][j].y_coord = y_pixel;

            if(board->cell[i][j].revealed)
            {
                switch (board->cell[i][j].type)
                {
                    case Mine:
                        draw_cell(renderer, image, Mine, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    case NotRevealed:
                        draw_cell(renderer, image, Empty, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    case Empty:
                        draw_cell(renderer, image, Empty, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    case Number_1:
                        draw_cell(renderer, image, Number_1, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    case Number_2:
                        draw_cell(renderer, image, Number_2, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    case Number_3:
                        draw_cell(renderer, image, Number_3, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    case Number_4:
                        draw_cell(renderer, image, Number_4, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    case Number_5:
                        draw_cell(renderer, image, Number_5, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    case Number_6:
                        draw_cell(renderer, image, Number_6, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    case Number_7:
                        draw_cell(renderer, image, Number_7, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    case Number_8:
                        draw_cell(renderer, image, Number_8, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                draw_cell(renderer, image, NotRevealed, x_pixel, y_pixel);
                SDL_RenderPresent(renderer);
            }

            if(board->cell[i][j].flag)
            {
                draw_cell(renderer, image, Flag, x_pixel, y_pixel);
                SDL_RenderPresent(renderer);
            }
            x_pixel += 20; //1 cella 20 pixel hosszu
        }
        x_pixel = 50;  //minden sor az 50. pixelnel kezdodjon
        y_pixel += 20; //1 cella 20 pixel magas
    }
}

/*Rekurzív függvény amely egy klikktõl felfedi az összes Empty típusú cellát, amég nem talál más típust*/
void floodfill(SDL_Renderer *renderer, SDL_Texture *tiles, int i, int j, Board *board)
{
    if(board->cell[i][j].type != NotRevealed)
    {
        for(int x=1; x<12; x++)
        {
            if(board->cell[i][j].type == x)
            {
                board->cell[i][j].revealed = true;
                draw_cell(renderer, tiles, x, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
                SDL_RenderPresent(renderer);
            }
        }
        return;
    }

    board->cell[i][j].type = Empty;
    board->cell[i][j].revealed = true;
    draw_cell(renderer, tiles, Empty, board->cell[i][j].x_coord, board->cell[i][j].y_coord);
    SDL_RenderPresent(renderer);

    if(i-1>=0 && board->cell[i-1][j].type != Empty)
        floodfill(renderer, tiles, i-1, j, board);

    if(i+1 <= board->height-1 && board->cell[i+1][j].type != Empty)
        floodfill(renderer, tiles, i+1, j, board);

    if(j+1 <= board->lenght-1 && board->cell[i][j+1].type != Empty)
        floodfill(renderer, tiles, i, j+1, board);

    if(j-1>=0 && board->cell[i][j-1].type != Empty)
        floodfill(renderer, tiles, i, j-1, board);
}

/*Megnézi, hogy a felhasználó megnyerte-e a játékot. Ha zászlók száma == aknák, felfedett cellák == tábla mérete - aknák*/
bool game_check_win(Board *board)
{
    int count_flags = 0;
    int count_revealed = 0;
    for(int i=0; i<board->height; i++)
    {
        for(int j=0; j<board->lenght; j++)
        {
            if (board->cell[i][j].flag)
            {
                count_flags++;
            }
            if(board->cell[i][j].revealed)
            {
                count_revealed++;
            }
        }
    }

    if(count_flags == board->mines && count_revealed == ((board->lenght*board->height)-board->mines))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*Lefoglal a tábla magasságának és szélességének megfelelő Cell típusú 2D-s dinamikus tömböt*/
static void make2D(Board *board)
{
    Cell **n;

    n = (Cell**) malloc(board->height * sizeof(Cell*));
    n[0] = (Cell*) malloc(board->lenght * board->height * sizeof(Cell));
    for (int y = 1; y < board->height; ++y)
    {
        n[y] = n[0] + y * board->lenght;
    }

    board->cell = n;
}

/*A függvénynek adott Game_Mode változóval kiválaszthatjuk, milyen játéktípust akarunk játszani. Visszatér igennel, ha játszunk és nemmel, ha nem*/
bool choose_game_mode(Game_Mode game_mode, Board *board, SDL_Renderer *renderer, SDL_Texture *image, FILE *fp)
{
    switch(game_mode)
    {
        case quit:
            SDL_Quit();
            return true;
        case easy:
            board->mines = 20;
            board->lenght = 20;
            board->height = 20;
            make2D(board);
            play_game(renderer, image, board);
            return false;
        case medium:
            board->mines = 40;
            board->lenght = 20;
            board->height = 20;
            make2D(board);
            play_game(renderer, image, board);
            return false;
        case hard:
            board->mines = 70;
            board->lenght = 20;
            board->height = 20;
            make2D(board);
            play_game(renderer, image, board);
            return false;
        case custom:
            printf("Well, let's customize it then...\n");
            printf("Board lenght (max 21):");
            scanf("%d", &board->lenght);
            printf("Board height (max 21):");
            scanf("%d", &board->height);
            printf("Number of mines:");
            scanf("%d", &board->mines);
            printf("Good Luck!\n");
            make2D(board);
            play_game(renderer, image, board);
            return false;
        case load:
            fp = fopen("save.txt", "r");
            int size_of_file = 0;

            if(fp != NULL)
            {
                fseek(fp, 0, SEEK_END);
                size_of_file = ftell(fp);
                rewind(fp);
            }
            else
            {
                printf("File reading error.");
                return false;
            }

            if(size_of_file == 0)
            {
                printf("The file is empty.");
                fclose(fp);
                return true;
            }
            else
            {
                fscanf(fp, "%d, %d, %d, %d", &board->lenght, &board->height, &board->mines, &board->time);
                make2D(board);
                for(int i=0; i<board->height; i++)
                {
                    for(int j=0; j<board->lenght; j++)
                    {
                            fscanf(fp, "%u ", &board->cell[i][j].type);
                    }
                    fscanf(fp, "\n");
                }
                fscanf(fp, "\n");
                int temp;
                for(int i=0; i<board->height; i++)
                {
                    for(int j=0; j<board->lenght; j++)
                    {
                        fscanf(fp, "%d ", &temp);
                        board->cell[i][j].revealed = temp;
                    }
                    fscanf(fp, "\n");
                }
                for(int i=0; i<board->height; i++)
                {
                    for(int j=0; j<board->lenght; j++)
                    {
                        fscanf(fp, "%d ", &temp);
                        board->cell[i][j].flag = temp;
                    }
                    fscanf(fp, "\n");
                }
                fclose(fp);
                play_loaded_game(renderer, image, board);
                SDL_RenderPresent(renderer);
            }
            return false;
    }
    return true;
}

/*kirajzolja a játszható pályát és a cellák tulajdonságait alaphelyzetbe állítja*/
static void generate_board(SDL_Renderer *renderer, SDL_Texture *image, Board *board)
{
    int x_pixel = 50;
    int y_pixel = 50;

    for(int i=0; i<board->height; i++)
    {
        for(int j=0; j<board->lenght; j++)
        {
            board->cell[i][j].neighbours = 0;
            board->cell[i][j].x_coord = x_pixel;
            board->cell[i][j].y_coord = y_pixel;
            board->cell[i][j].type = NotRevealed;
            board->cell[i][j].flag = false;
            board->cell[i][j].revealed = false;
            draw_cell(renderer, image, NotRevealed, x_pixel, y_pixel);
            x_pixel += 20; //1 cella 20 pixel hosszu

        }
        x_pixel = 50;  //minden sor az 50. pixelnel kezdodjon
        y_pixel += 20; //1 cella 20 pixel magas
    }
}

/*Random szétszór aknákat a játékpályán*/
static void generate_mines(Board *board)
{
    if(board->lenght*board->height < board->mines){
        SDL_Log("Tobb aknat helyeztel el mint amennyi mezo talalhato.");
        exit(1);
    }

    int counter = 0;
    while(counter < board->mines)
    {
        int x = rand()%board->height;
        int y = rand()%board->lenght;
        if(board->cell[x][y].type != Mine)
        {
            board->cell[x][y].type = Mine;
            counter++;
        }
    }
}

/*Megszámolja hány szomszédja van egy cellának*/
static void calculate_neighbours(Board *board)
{
    for (int i = 0; i < board->height; i++)
    {
        for (int j = 0; j < board->lenght; j++)
        {
            if(board->cell[i][j].type != Mine)
            {
                for (int x = -1; x <= 1; x++)
                {
                    for (int y = -1; y <= 1; y++)
                    {
                        int x_position = x + i;
                        int y_position = y + j;

                        if (y_position < 0 || y_position > board->lenght-1 || x_position < 0 || x_position > board->height-1);
                        else if(board->cell[x_position][y_position].type == Mine)
                            board->cell[i][j].neighbours += 1;
                    }
                }
            }
        }
    }
}

/*A megszámlált szomszédok alapján kap egy Cell_Type típust minden cella*/
static void neighbour_type(Board *board)
{
    for (int i = 0; i < board->height; i++)
    {
        for (int j = 0; j < board->lenght; j++)
        {
            if(board->cell[i][j].neighbours == 1)
                board->cell[i][j].type = Number_1;
            if(board->cell[i][j].neighbours == 2)
                board->cell[i][j].type = Number_2;
            if(board->cell[i][j].neighbours == 3)
                board->cell[i][j].type = Number_3;
            if(board->cell[i][j].neighbours == 4)
                board->cell[i][j].type = Number_4;
            if(board->cell[i][j].neighbours == 5)
                board->cell[i][j].type = Number_5;
            if(board->cell[i][j].neighbours == 6)
                board->cell[i][j].type = Number_6;
            if(board->cell[i][j].neighbours == 7)
                board->cell[i][j].type = Number_7;
            if(board->cell[i][j].neighbours == 8)
                board->cell[i][j].type = Number_8;
        }
    }

}

/*Összefuzi az összes Cella típus besoroló és tábla rajzoló függvényt. Ezután már játszható a játék*/
void play_game(SDL_Renderer *renderer, SDL_Texture *image, Board *board)
{
    boxRGBA(renderer, 0, 0, 500, 500, 189, 189, 189, 255);
    generate_board(renderer, image, board);
    generate_mines(board);
    calculate_neighbours(board);
    neighbour_type(board);
    SDL_RenderPresent(renderer);
}
