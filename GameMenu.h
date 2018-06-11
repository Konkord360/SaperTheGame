#pragma once
#include "stdafx.h"

struct square
{
	bool is_bomb = 0;
	bool is_clicked = 0;
	bool is_safe = 0;
	int number_of_bombs = 0;
	unsigned char plate = (char)176;
};

bool want_to_play(bool &isLoaded);
void make_game(int &rows, int &columns, int &bomb_chance);
void view(const int rows, const int columns, square **tab,const int click,const int numberOfBombs);
void is_lost(bool &lost, int rows, int columns, int number_of_bombs, int click);
bool play_again();