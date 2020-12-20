#include "Player.H"
Player::Player() {
	std::ifstream in;
	in.open("../GraphicsRush/data/player.txt");
	in >> highscore >> money_total;
	in.close();
}