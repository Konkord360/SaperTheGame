#include "GameFunctions.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <termios.h>
#include <ncurses.h>

void initialization(const int rows, const int columns, square **tab)
{
    for (int i = 0; i < rows; i++)
        tab[i] = new square[columns];

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; ++j)
            tab[i][j].number_of_bombs = 0;
    }
}

void move(square *pointer, int rows, int columns, square **tab, int bomb_chance, bool &quit, bool &isLoaded)
{
    bool lost = 0;
    int x = 0, y = 0, click = 0, number_of_bomb = 0, bombsViewed=0;
    char plate_holder, pointerPlate = 'o';
    int move;

    initscr();            // Start curses mode
    raw();                // Disable line buffering
    keypad(stdscr, TRUE); // Enable special keys like arrow keys
    noecho();             // Don't echo key presses to the screen


    if (isLoaded)
    {
        loadClicksAndBombs(click, number_of_bomb);
        isLoaded = false;
    }


    plate_holder = tab[x][y].plate;
    pointer->plate = pointerPlate;

    view(rows, columns, tab, click, bombsViewed);

    do              //game loop
    {
        move = getch(); 
        switch (move)
        {
            case 'k': 
                if (x != 0)
                { 
                    x--;
                    pointer = &tab[x][y]; 
                    tab[x + 1][y].plate = plate_holder;
                    plate_holder = tab[x][y].plate;
                    pointer->plate = pointerPlate;
                }break;

            case 'j': 
                if (x != rows - 1)
                { 
                    x++;
                    pointer = &tab[x][y];
                    tab[x - 1][y].plate = plate_holder;
                    plate_holder = tab[x][y].plate;
                    pointer->plate = pointerPlate;
                } break;

            case 'h':
                if (y != 0)
                { 
                    y--;
                    pointer = &tab[x][y];
                    tab[x][y + 1].plate = plate_holder;
                    plate_holder = tab[x][y].plate;
                    pointer->plate = pointerPlate;
                } break;

            case 'l':
                if (y != columns - 1) 
                { 
                    y++;
                    pointer = &tab[x][y];
                    tab[x][y - 1].plate = plate_holder;
                    plate_holder = tab[x][y].plate; 
                    pointer->plate = pointerPlate;
                }break;

            case 'o':
                if ((!pointer->is_clicked) && (!pointer->is_safe))
                { 
                    choice(rows, columns, pointer, tab, click, lost, bomb_chance, x, y, number_of_bomb, bombsViewed);
                    plate_holder = tab[x][y].plate; 
                } break;

            case 'a':
                if (!pointer->is_clicked)
                { 
                    set_safe(pointer, bombsViewed);
                    plate_holder = tab[x][y].plate;
                } break;

            case '#': 
                quit = true;
                tab[x][y].plate = plate_holder;
                saveGame(tab, rows, columns, click, number_of_bomb);
                break;
        }

        view(rows, columns, tab, click, bombsViewed);
        is_lost(lost, rows, columns, number_of_bomb, click);
    } while ( !quit && (!lost));
    endwin();
}

void choice(const int rows, const int columns, square *pointer, square **tab, int &click, bool &lost, int bomb_chance, int x, int y, int &number_of_bomb, int &bombsViewed)
{
    int i = 1, j = 1;

    click++;
    if (click == 1)
        generate(rows, columns, tab, bomb_chance, number_of_bomb, pointer, bombsViewed);

    if (pointer->is_bomb == true)
    {
        pointer->plate = 'x';
        lost = true;
    }

    else if (lost == false)
    {
        if (pointer->number_of_bombs != 0)
        {
            pointer->is_clicked = true;
            pointer->plate = '0' + pointer->number_of_bombs;
        }
        else
        {
            show_off(tab, x, y, rows, columns, click);
            click--;
        }
    }
}

void generate(const int rows, const int columns, square **tab, int bomb_chance, int &number_of_bomb, square *pointer, int &bombsViewed)
{
    srand(time(NULL));
    int if_bomb = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; ++j)
        {
            if_bomb = rand() % 100 + 1;

            if (tab[i][j].is_clicked == 0)
            {
                if (if_bomb <= bomb_chance && &tab[i][j] != pointer)
                {
                    tab[i][j].is_bomb = 1;
                    number_of_bomb++;
                }
                else
                    tab[i][j].is_bomb = 0;
            }
        }
    }
    bombsViewed = number_of_bomb;
    //liczenie bomb dooko³a pola
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; ++j)
        {

            if ((i != 0 && j != 0) && (i != 0 && j != columns - 1) && (i != rows - 1 && j != 0) && (i != rows - 1 && j != columns - 1))
            {
                if (tab[i][j + 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i + 1][j + 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i + 1][j].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i - 1][j].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i - 1][j - 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i][j - 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i - 1][j + 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i + 1][j - 1].is_bomb) tab[i][j].number_of_bombs++;
            }

            else if (i == 0 && j != 0 && j != columns - 1)
            {
                if (tab[i + 1][j].is_bomb)	tab[i][j].number_of_bombs++;
                if (tab[i + 1][j + 1].is_bomb)	tab[i][j].number_of_bombs++;
                if (tab[i + 1][j - 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i][j - 1].is_bomb)	tab[i][j].number_of_bombs++;
                if (tab[i][j + 1].is_bomb)	tab[i][j].number_of_bombs++;
            }

            else if (i == rows - 1 && j != 0 && j != columns - 1)
            {
                if (tab[i - 1][j].is_bomb)	tab[i][j].number_of_bombs++;
                if (tab[i - 1][j + 1].is_bomb)	tab[i][j].number_of_bombs++;
                if (tab[i - 1][j - 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i][j - 1].is_bomb)	tab[i][j].number_of_bombs++;
                if (tab[i][j + 1].is_bomb)	tab[i][j].number_of_bombs++;
            }

            else if (i != 0 && i != rows - 1 && j == 0)
            {
                if (tab[i][j + 1].is_bomb)	tab[i][j].number_of_bombs++;
                if (tab[i + 1][j + 1].is_bomb)	tab[i][j].number_of_bombs++;
                if (tab[i - 1][j + 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i - 1][j].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i + 1][j].is_bomb) tab[i][j].number_of_bombs++;
            }

            else if (i != 0 && i != rows - 1 && j == columns - 1)
            {
                if (tab[i][j - 1].is_bomb)	tab[i][j].number_of_bombs++;
                if (tab[i + 1][j - 1].is_bomb)	tab[i][j].number_of_bombs++;
                if (tab[i - 1][j - 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i - 1][j].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i + 1][j].is_bomb) tab[i][j].number_of_bombs++;
            }

            else if (i == 0 && j == 0)
            {
                if (tab[i][j + 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i + 1][j + 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i + 1][j].is_bomb) tab[i][j].number_of_bombs++;
            }

            else if (i == 0 && j == columns - 1)
            {
                if (tab[i][j - 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i + 1][j - 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i + 1][j].is_bomb) tab[i][j].number_of_bombs++;
            }

            else if (i == rows - 1 && j == 0)
            {
                if (tab[i - 1][j].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i - 1][j + 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i][j + 1].is_bomb) tab[i][j].number_of_bombs++;
            }

            else if (i == rows - 1 && j == columns - 1)
            {
                if (tab[i - 1][j].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i - 1][j - 1].is_bomb) tab[i][j].number_of_bombs++;
                if (tab[i][j - 1].is_bomb) tab[i][j].number_of_bombs++;
            }
        }
    }
}

void show_off(square **tab, int x, int y, int rows, int columns, int &click)
{
    if (x <= rows - 1 && y <= columns - 1 && x >= 0 && y >= 0 && tab[x][y].number_of_bombs == 0)
    {
        if (tab[x][y].number_of_bombs == 0 && tab[x][y].is_clicked == false)
        {
            tab[x][y].is_clicked = true;
            tab[x][y].plate = '0';
            click++;

            show_off(tab, x + 1, y, rows, columns, click);
            show_off(tab, x - 1, y, rows, columns, click);
            show_off(tab, x, y + 1, rows, columns, click);
            show_off(tab, x, y - 1, rows, columns, click);
            show_off(tab, x + 1, y + 1, rows, columns, click);
            show_off(tab, x - 1, y + 1, rows, columns, click);
            show_off(tab, x + 1, y - 1, rows, columns, click);
            show_off(tab, x - 1, y - 1, rows, columns, click);
        }
    }

    else if (x <= rows - 1 && y <= columns - 1 && x >= 0 && y >= 0 && tab[x][y].is_clicked == false)
    {
        tab[x][y].is_clicked = true;
        tab[x][y].plate = '0' + tab[x][y].number_of_bombs;
        click++;
    }
}

void set_safe(square *pointer, int &numberOfBombs)
{
    char falgPlate = '!', normalPlate = ' ';

    if (!pointer->is_safe )
    {
        pointer->is_safe = true;
        pointer->plate = falgPlate;
        numberOfBombs--;
    }

    else
    {
        pointer->is_safe = false;
        pointer->plate = normalPlate;
        numberOfBombs++;
    }
}

void delete_tab(const int rows, square** tab)
{
    for (int i = 0; i < rows; ++i)
        delete[] tab[i];

    delete[] tab;
}

bool game(bool &isLoaded)
{
    int rows, columns, bomb_chance=0;
    bool quit = false;

    if (isLoaded)
        loadSize(rows, columns);	

    else
        make_game(rows, columns, bomb_chance);

    square **tab = new square*[rows];
    initialization(rows, columns, tab);

    if (isLoaded)
        loadContent(tab, rows, columns);

    square *pointer = &tab[0][0];

    move(pointer, rows, columns, tab, bomb_chance, quit, isLoaded);
    delete_tab(rows, tab);

    if (quit)
    {
        quit = false;
        return want_to_play(isLoaded);

    }
    else
        return play_again();
}

void saveGame(square **tab,const int rows,const int columns, const int click, const int numberOfBombs)
{
    std::ofstream save;

    save.open("Save.txt");
    if (save.good())
    {
        save << rows << '\n';
        save << columns << '\n';
        save << click << '\n';
        save << numberOfBombs << '\n';
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                save << tab[i][j].is_bomb << ' ';
                save << tab[i][j].is_clicked << ' ';
                save << tab[i][j].is_safe << ' ';
                save << tab[i][j].number_of_bombs << ' ';
                save << (int)tab[i][j].plate << ' ';
            }
            save << '\n';
        }
        save.close();
    }
}

void loadSize(int &rows, int &columns)
{
    std::ifstream load;
    load.open("Save.txt");

    if (load.good())
    {
        load >> rows;
        load >> columns;
    }

    load.close();
    load.clear();
}

void loadClicksAndBombs(int &click, int &numberOfBombs)
{
    int rows, columns;
    std::ifstream load;
    load.open("Save.txt");

    if (load.good())
    {
        load >> rows;
        load >> columns;
        load >> click;
        load >> numberOfBombs;
    }

    load.close();
    load.clear();
}

void loadContent(square **tab, const int rows, const int columns)
{
    int holder2;
    std::ifstream load;
    load.open("Save.txt");
    int holder = 0;

    if (load.good())
    {
        load >> holder;
        load >> holder;
        load >> holder;
        load >> holder;

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {

                load >> tab[i][j].is_bomb;
                load >> tab[i][j].is_clicked;
                load >> tab[i][j].is_safe;
                load >> tab[i][j].number_of_bombs;
                load >> holder;
                tab[i][j].plate = holder;
            }
        }
    }
    load.close();
    load.clear();
}
