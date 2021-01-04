#include "Player.H"
Player::Player() {
	std::ifstream in;
	in.open("../GraphicsRush/data/player.txt");
	in >> highscore >> money_total;
	for (int item = 0; item < (int)items.size(); item++) {
		in >> items[item];
	}
	for (int cp = 0; cp < (int)cps.size(); cp++) {
		in >> cps[cp];
	}
	in.close();
}
void Player::saveFile() {
	std::ofstream out;
	out.open("../GraphicsRush/data/player.txt");
	out << highscore << std::endl << money_total << std:: endl;
	for (int item = 0; item < (int)items.size(); item++) {
		out << items[item] << " ";
	}
	out << std::endl;
	for (int cp = 0; cp < (int)cps.size(); cp++) {
		out << cps[cp] << " ";
	}
	out.close();
}