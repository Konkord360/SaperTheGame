﻿#include "GameMenu.h"
#include "GameFunctions.h"

int main()
{
	bool isLoaded = false;
	bool newgame = want_to_play(isLoaded);
	
	
	while (newgame)
		newgame = game(isLoaded);
	
    return 0;
}
