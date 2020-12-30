#include "Player.H"
Player::Player() {
	std::ifstream in;
	in.open("../GraphicsRush/data/player.txt");
	in >> highscore >> money_total;
	in.close();
}
void Player::saveFile() {
	std::ofstream out;
	out.open("../GraphicsRush/data/player.txt");
	out << highscore << std::endl << money_total;
	out.close();
}