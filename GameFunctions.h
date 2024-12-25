#pragma once
#include"GameMenu.h"

void initialization(const int rows, const int columns, square **tab);
void move(square *pointer, int rows, int columns, square **tab, int  bomb_chance,bool &quit, bool &isLoaded);

void choice(const int rows, const int columns, square *pointer, square **tab, int &click,
	bool &lost,	int bomb_chance, int x, int y, int &number_of_bomb, int &bombsViewed);

void generate(const int rows, const int columns, square **tab, int bomb_chance,
	int &number_of_bomb, square *pointer, int &bombsViewed);
void show_off(square **tab, int x, int y, int rows, int columns, int &click);
void set_safe(square *pointer, int &numberOfBombs);
void delete_tab(const int rows, square** tab);
bool game(bool &isLoaded);
void saveGame(square **tab,const int rows,const int columns,const int click,const int numberOfBombs);
void loadSize(int &rows, int &columns);
void loadContent(square **tab, const int rows, const int columns);
void loadClicksAndBombs(int &click, int &numberOfBombs);
//char getch(void);
//square **loadGame(square **tab, int rows, int columns);
