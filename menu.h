#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

/*Menüpontok neveit tartalmazó adatszerkezet*/
typedef enum Menu_Name {main_menu, new_game, custom_game, load_game, start_game, exit_game} Menu_Name;
/*Játéktípusok neveit tartalmazó adaszerkezet*/
typedef enum Game_Mode {quit, easy, medium, hard, custom, load} Game_Mode;

Game_Mode start_menu(TTF_Font *font, TTF_Font *font1, SDL_Renderer *renderer, SDL_Rect *where, SDL_Texture *image);

#endif // MENU_H_INCLUDED
