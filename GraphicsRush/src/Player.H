#pragma once
#include "fstream"
class Player {
	public:
		Player();
		void saveFile();
		int highscore = 0;
		int money_total = 0;
};