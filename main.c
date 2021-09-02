#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "debugmalloc.h"
#include "grafika.h"
#include "jatekvezerles.h"
#include "menu.h"

/*másodperceket percekre váltja*/
void convert_time(int *seconds, int *minutes)
{
    while(*seconds >= 60)
    {
        *minutes += 1;
        *seconds -= 60;
    }
    return;
}

int main(int argc, char *argv[]) {

    srand(time(0));
    FILE* fp = NULL;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Surface* icon = IMG_Load("MineSweeper_WindowIcon.png");
    SDL_SetWindowIcon(window, icon);
    sdl_init("MINESWEEPER", 500, 500, &window, &renderer);

    SDL_Texture *tiles = IMG_LoadTexture(renderer, "Minesweeper_Tiles.png");
    if (tiles == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("ALBA.ttf", 60);
    if (!font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    TTF_Font *font1 = TTF_OpenFont("ALBA.ttf", 15);
    if (!font1) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    Board board;
    board.time = 0;
    Uint32 end_game_time = 0;
    Uint32 start_game_time = 0;
    SDL_Rect where = { 0, 0, 0, 0 };

    Game_Mode game_mode = start_menu(font, font1, renderer, &where, tiles);
    bool quit = choose_game_mode(game_mode, &board, renderer, tiles, fp);
    /*ez a flag elindít egy időmérőt, ha a játékos klikkel a pályán*/
    int start_game_flag = 1;
    /*nullára vált, ha a játékos aknára lép, tehát veszít*/
    int game_lost_flag = 1;
    /*Ha kilép a felhasználó felszabadul a tömb és igazra vált*/
    bool cell_freed = false;

    while (!quit)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch(event.type)
        {   /*Ha a felhasználó balklikkel, kiszámolja hova klikkelt és felfedi a cellát*/
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    int x = event.button.x;
                    int y = event.button.y;

                    for(int i=0; i<board.height; i++)
                    {
                        for(int j=0; j<board.lenght; j++)
                        {
                            if(x>board.cell[i][j].x_coord && x < board.cell[i][j].x_coord + 20 && y>board.cell[i][j].y_coord && y<board.cell[i][j].y_coord + 20)
                            {
                                if(start_game_flag == 1) //elkezdi az időmérőt
                                {
                                    start_game_time = SDL_GetTicks();
                                    start_game_flag = 0;
                                }

                                switch(board.cell[i][j].type)
                                {
                                    case Mine:
                                        board.cell[i][j].revealed = true;
                                        draw_cell(renderer, tiles, Mine, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                        SDL_RenderPresent(renderer);
                                        game_lost_flag = 0; //a játékos vesztett
                                        break;
                                    case NotRevealed:
                                        floodfill(renderer, tiles, i, j, &board); //rekurzió hívása
                                        break;
                                    case Number_1:
                                        board.cell[i][j].revealed = true;
                                        draw_cell(renderer, tiles, Number_1, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                        SDL_RenderPresent(renderer);
                                        break;
                                    case Number_2:
                                        board.cell[i][j].revealed = true;
                                        draw_cell(renderer, tiles, Number_2, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                        SDL_RenderPresent(renderer);
                                        break;
                                    case Number_3:
                                        board.cell[i][j].revealed = true;
                                        draw_cell(renderer, tiles, Number_3, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                        SDL_RenderPresent(renderer);
                                        break;
                                    case Number_4:
                                        board.cell[i][j].revealed = true;
                                        draw_cell(renderer, tiles, Number_4, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                        SDL_RenderPresent(renderer);
                                        break;
                                    case Number_5:
                                        board.cell[i][j].revealed = true;
                                        draw_cell(renderer, tiles, Number_5, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                        SDL_RenderPresent(renderer);
                                        break;
                                    case Number_6:
                                        board.cell[i][j].revealed = true;
                                        draw_cell(renderer, tiles, Number_6, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                        SDL_RenderPresent(renderer);
                                        break;
                                    case Number_7:
                                        board.cell[i][j].revealed = true;
                                        draw_cell(renderer, tiles, Number_7, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                        SDL_RenderPresent(renderer);
                                        break;
                                    case Number_8:
                                        board.cell[i][j].revealed = true;
                                        draw_cell(renderer, tiles, Number_8, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                        SDL_RenderPresent(renderer);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                }
                /*ha jobb klikkel egy cellára a felhasználó zászlót rajzolunk, ha már zászló, levesszük*/
                if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    int x = event.button.x;
                    int y = event.button.y;

                    for(int i=0; i<board.height; i++)
                    {
                        for(int j=0; j<board.lenght; j++)
                        {
                            if(x>board.cell[i][j].x_coord && x < board.cell[i][j].x_coord + 20 && y>board.cell[i][j].y_coord && y<board.cell[i][j].y_coord + 20)
                            {
                                if(!board.cell[i][j].revealed)
                                {
                                    switch(board.cell[i][j].flag)
                                    {
                                        case true:
                                            board.cell[i][j].flag = false;
                                            draw_cell(renderer, tiles, NotRevealed, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                            SDL_RenderPresent(renderer);
                                            break;

                                        case false:
                                            board.cell[i][j].flag = true;
                                            draw_cell(renderer, tiles, Flag, board.cell[i][j].x_coord, board.cell[i][j].y_coord);
                                            SDL_RenderPresent(renderer);
                                            break;
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            case SDL_QUIT:
                /*ha kilép a felhasználó és nem végzett a játékkal, akkor fájlba mentés*/
                if(!game_check_win(&board) && game_lost_flag == 1)
                {
                    end_game_time = SDL_GetTicks();
                    board.time = end_game_time - start_game_time; //játszott idő

                    fp = fopen("save.txt", "w");
                    fprintf(fp, "%d, %d, %d, %d\n", board.lenght, board.height, board.mines, board.time);

                    for(int i=0; i<board.height; i++)
                    {
                        for(int j=0; j<board.lenght; j++)
                        {
                            fprintf(fp, "%d ", board.cell[i][j].type);
                        }
                        fprintf(fp, "\n");
                    }
                    fprintf(fp, "\n");
                    for(int i=0; i<board.height; i++)
                    {
                        for(int j=0; j<board.lenght; j++)
                        {
                            fprintf(fp, "%d ", board.cell[i][j].revealed);
                        }
                        fprintf(fp, "\n");
                    }
                    fprintf(fp, "\n");
                    for(int i=0; i<board.height; i++)
                    {
                        for(int j=0; j<board.lenght; j++)
                        {
                            fprintf(fp, "%d ", board.cell[i][j].flag);
                        }
                        fprintf(fp, "\n");
                    }
                    fclose(fp);
                }
                free(board.cell[0]);
                free(board.cell);
                quit = true;
                cell_freed = true;
                break;
        }
        /*ha a felhasználó nyert*/
        if(!cell_freed && game_check_win(&board))
        {
            end_game_time = SDL_GetTicks(); //játék végé
            if(game_mode == load)
            {
                board.time = board.time + (end_game_time - start_game_time);
            }
            else
            {
                board.time = end_game_time - start_game_time;
            }

            int seconds = (int)board.time/1000;
            int minutes = 0;
            convert_time(&seconds, &minutes);
            free(board.cell[0]);
            free(board.cell);
            printf("It only took: %02d:%02d\n", minutes, seconds); //konzolra kiírja hány perc alatt nyert a felhasználó
            /*visszatérés a főmenübe opció*/
            bool button_pressed = false;
            while(!button_pressed)
            {
                boxRGBA(renderer, 200, 250, 300, 280, 123, 123, 123, 255);
                draw_text("YOU WON", 255, 0, font1, 0, renderer, &where);
                SDL_RenderPresent(renderer);

                SDL_Event menu_event;
                SDL_WaitEvent(&menu_event);
                switch(menu_event.type)
                {
                    case SDL_MOUSEBUTTONDOWN:
                        if(menu_event.button.button == SDL_BUTTON_LEFT)
                        {
                            int get_x = menu_event.button.x;
                            int get_y = menu_event.button.y;

                            if(get_x>200 && get_x<300 && get_y>250 && get_y<280)
                            {
                                game_mode = start_menu(font, font1, renderer, &where, tiles);
                                quit = choose_game_mode(game_mode, &board, renderer, tiles, fp);
                                start_game_flag = 1;
                                button_pressed = true;
                            }
                        }
                        break;
                    case SDL_QUIT:
                        button_pressed = true;
                        quit = true;
                        break;
                }
            }
        }
        /*ha a játékos veszett*/
        if(game_lost_flag == 0)
        {
            free(board.cell[0]);
            free(board.cell);
            /*visszatérés a főmenübe opció*/
            bool button_pressed = false;
            while(!button_pressed)
            {
                boxRGBA(renderer, 200, 250, 300, 280, 123, 123, 123, 255);
                draw_text("YOU LOST", 255, 0, font1, 0, renderer, &where);
                SDL_RenderPresent(renderer);

                SDL_Event menu_event;
                SDL_WaitEvent(&menu_event);
                switch(menu_event.type)
                {
                case SDL_MOUSEBUTTONDOWN:
                    if(menu_event.button.button == SDL_BUTTON_LEFT)
                    {
                        int get_x = menu_event.button.x;
                        int get_y = menu_event.button.y;

                        if(get_x>200 && get_x<300 && get_y>250 && get_y<280)
                        {
                            button_pressed = true;
                            game_mode = start_menu(font, font1, renderer, &where, tiles);
                            quit = choose_game_mode(game_mode, &board, renderer, tiles, fp);
                            start_game_flag = 1;
                            game_lost_flag = 1;
                        }
                    }
                    break;
                case SDL_QUIT:
                    button_pressed = true;
                    quit = true;
                break;
                }
            }
        }
    }

    SDL_DestroyTexture(tiles);
    SDL_FreeSurface(icon);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_CloseFont(font1);
    SDL_Quit();
    return 0;
}
