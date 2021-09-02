#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "menu.h"
#include "grafika.h"

/*A start_menu meghívja, ha a felhasználó a főmenübe szeretne menni. A függvény kirajzolja a főmenüt és visszatér a következő menüvel*/
static Menu_Name show_main_menu(TTF_Font *font, TTF_Font *font1, SDL_Renderer *renderer, SDL_Rect *where)
{
    boxRGBA(renderer, 0, 0, 500, 500, 189, 189, 189, 255);
    draw_text("MINESWEEPER", 20, 0, font, 0, renderer, where);

    boxRGBA(renderer, 200, 250, 300, 280, 255, 255, 255, 255);  //új játék
    draw_text("Quick Game", 255, 0, font1, 0, renderer, where);

    boxRGBA(renderer, 200, 300, 300, 330, 255, 255, 255, 255);  //custom game
    draw_text("Custom Game", 305, 0, font1, 0, renderer, where);

    boxRGBA(renderer, 200, 350, 300, 380, 255, 255, 255, 255);  //load game
    draw_text("Load Game", 355, 0, font1, 0, renderer, where);

    boxRGBA(renderer, 200, 400, 300, 430, 255, 255, 255, 255);  //kilépés
    draw_text("Quit", 405, 0, font1, 0, renderer, where);

    SDL_Event menu_event;
    SDL_WaitEvent(&menu_event);

    switch (menu_event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (menu_event.button.button == SDL_BUTTON_LEFT) {
                int x = menu_event.button.x;
                int y = menu_event.button.y;

                if(x>200 && x<300 && y>250 && y<280)
                {
                    return new_game;
                }
                if(x>200 && x<300 && y>300 && y<330)
                {
                    return custom_game;
                }
                if(x>200 && x<300 && y>350 && y<380)
                {
                    return load_game;
                }
                if(x>200 && x<300 && y>400 && y<430)
                {
                    return exit_game;
                }
            }
            break;
        case SDL_QUIT:
            return exit_game;
    }

    return main_menu;
}

/*A start_menu meghívja, ha a felhasználó az új játék menübe szeretne menni. A függvény kirajzolja az új játék menüt és visszatér a következő menüvel vagy elkezdi a játékot*/
static Menu_Name show_new_game_menu(TTF_Font *font1, SDL_Renderer *renderer, SDL_Rect *where, Game_Mode *game_mode)
{
    boxRGBA(renderer, 0, 0, 500, 500, 189, 189, 189, 255);

    boxRGBA(renderer, 200, 200, 300, 230, 255, 255, 255, 255);  //új játék
    draw_text("Easy", 205, 0, font1, 0, renderer, where);

    boxRGBA(renderer, 200, 250, 300, 280, 255, 255, 255, 255);  //custom game
    draw_text("Medium", 255, 0, font1, 0, renderer, where);

    boxRGBA(renderer, 200, 300, 300, 330, 255, 255, 255, 255);  //load game
    draw_text("Hard", 305, 0, font1, 0, renderer, where);

    boxRGBA(renderer, 200, 350, 300, 380, 255, 255, 255, 255);  //kilépés
    draw_text("Back", 355, 0, font1, 0, renderer, where);

    SDL_Event menu_event;
    SDL_WaitEvent(&menu_event);

    switch (menu_event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (menu_event.button.button == SDL_BUTTON_LEFT) {
                int x = menu_event.button.x;
                int y = menu_event.button.y;

                if(x>200 && x<300 && y>200 && y<230)
                {
                    *game_mode = easy;
                    return start_game;
                }
                if(x>200 && x<300 && y>250 && y<280)
                {
                    *game_mode = medium;
                    return start_game;
                }
                if(x>200 && x<300 && y>300 && y<330)
                {
                    *game_mode = hard;
                    return start_game;
                }
                if(x>200 && x<300 && y>350 && y<380)
                {
                    return main_menu;
                }
            }
            break;
        case SDL_QUIT:
            return exit_game;
            break;
    }

    return new_game;
}

/*Kezeli a menüpontokat, és ha a felhasználó újmenüpontra lép, akkor a függvény átmegy az adott menübe egy függvény segítségével. Ha a felhasználó játszani akar visszatér a játékmóddal*/
Game_Mode start_menu(TTF_Font *font, TTF_Font *font1, SDL_Renderer *renderer, SDL_Rect *where, SDL_Texture *image)
{
    bool start = false;
    Menu_Name which = main_menu;
    Game_Mode game_mode;

    while(!start)
    {
        switch(which)
        {
            case main_menu:
                which = show_main_menu(font, font1, renderer, where);
                SDL_RenderPresent(renderer);
                break;
            case new_game:
                which = show_new_game_menu(font1, renderer, where, &game_mode);
                SDL_RenderPresent(renderer);
                break;
            case custom_game:
                game_mode = custom;
                start = true;
                break;
            case load_game:
                game_mode = load;
                start = true;
                break;
            case start_game:
                start = true;
                break;
            case exit_game:
                game_mode = quit;
                start = true;
                break;
        }
    }

    return game_mode;
}
