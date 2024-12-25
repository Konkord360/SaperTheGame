#include "GameMenu.h"
#include <curses.h>
#include <iostream>
#include <ncurses.h>

bool want_to_play(bool &isLoaded)
{
	system("clear");
	char ng;
	std::cout << "---------------" << std::endl;
	std::cout << "| 1. Nowa gra   |" << std::endl;
	std::cout << "| 2. Wczytaj gre|" << std::endl;
	std::cout << "| 3. Wyjscie    |" << std::endl;
	std::cout << "---------------" << std::endl;

	std::cin >> ng;

	system("clear");

	if (ng == '1')
		return true;
	else if (ng == '2')
	{
		isLoaded = true;
		return true;
	}
	else
		return false;
}

void make_game(int &rows, int &columns, int &bomb_chance)
{
	system("clear");
	char size, difficulty;
	int smallRows = 8, smallColumns = 8, mediumRows = 11,
		mediumColumns = 11, bigRows = 11, bigColumns = 17,
		easyBombsCh=10, mediumBombCh=20, hardBombCh=30;

	std::cout << "---------------------------" << std::endl;
	std::cout << "| Wybierz rozmiar planszy |" << std::endl;
	std::cout << "| 1.Maly                  |" << std::endl;
	std::cout << "| 2.Sredni                |" << std::endl;
	std::cout << "| 3.Duzy                  |" << std::endl;
	std::cout << "---------------------------" << std::endl;
	std::cin >> size;
	system("clear");

	switch (size)
	{
	case '1': rows = smallRows; columns = smallColumns; break;
	case '2': rows = mediumRows; columns = mediumColumns; break;
	case '3': rows = bigRows; columns = bigColumns; break;

	}
	std::cout << "----------------------------" << std::endl;
	std::cout << "| Wybierz poziom trudnosci |" << std::endl;
	std::cout << "| 1.Latwy                  |" << std::endl;
	std::cout << "| 2.Sredni                 |" << std::endl;
	std::cout << "| 3.Trudny                 |" << std::endl;
	std::cout << "----------------------------" << std::endl;
	std::cin >> difficulty;
	system("clear");

    switch (difficulty)
    {
        case '1': bomb_chance = easyBombsCh; break;
        case '2': bomb_chance = mediumBombCh; break;
        case '3': bomb_chance = hardBombCh; break;
    }
}

void view(const int rows, const int columns, square **tab,const int click,const int numberOfBombs)
{
    clear();
    refresh();
    printw("\n");
	char underline = '_', straightLine='|';

	for (int i = 0; i < rows; i++)
	{
		
		printw(" ");
		for (int j = 0; j < columns; ++j) {
			printw(" %c", underline);
        }
		
        printw("\n");

		printw(" ");
		for (int j = 0; j < columns; ++j)
			printw("%c%c", straightLine, tab[i][j].plate);

		printw("%c\n", straightLine);
	}
	printw(" ");

	for (int j = 0; j < columns; ++j)
		printw(" %c", underline);

    printw("\n");
    printw("Clicks: %d Bombs: %d SafeFields: %d\n", click, numberOfBombs, (rows*columns) - numberOfBombs);
    refresh();
}

void is_lost(bool &lost, int rows, int columns, int number_of_bombs, int click)
{
    if (lost)
    {	
        printw("--------------------------------\n");
        printw("| Porazka! Sprobuj jeszcze raz |\n");
        printw("--------------------------------\n");
        printw("Press any key to continue...\n");
        getch();
    } else if (click == (rows*columns) - number_of_bombs)
    {
        printw("--------------------------------\n");
        printw("| Brawo! Udalo ci sie osiagnac zwyciestwo!|\n");
        printw("--------------------------------\n");
        printw("Press any key to continue...\n");
        getch();
        lost = true;
    }
}

bool play_again()
{
    char choice;
    system("clear");

    std::cout << "---------------------------" << std::endl;
    std::cout << "| Chcesz zagrac ponownie? |" << std::endl;
    std::cout << "| 1. Tak                  |" << std::endl;
    std::cout << "| 2. Nie                  |" << std::endl;
    std::cout << "---------------------------" << std::endl;
    std::cin >> choice;

    switch (choice)
    {
        case '1': return true; break;
        case '2': return false; break;
        default: return false;
    }
}
