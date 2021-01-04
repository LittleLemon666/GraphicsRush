#pragma once
#include<vector>
#include "fstream"
class Player {
	public:
		Player();
		void saveFile();
		int highscore = 0;
		int money_total = 0;
		std::vector<int> items = { 0, 0, 0, 0, 0, 0 };
		std::vector<int> cps = { 0, 0, 0, 0, 0 };
		bool invincible = false;
};