#include "game.h"

std::istream& operator >> (std::istream& in, Game& obj)
{
	in >> obj.highClassic >> obj.highQuick;
	return in;
}

std::ostream& operator << (std::ostream& out, const Game& obj)
{
	out << obj.highClassic << ' ' << obj.highQuick;
	return out;
}

int main() {
	if (!std::ifstream("resources.dat")) {
		std::ofstream saveHighScores("resources.dat");
		saveHighScores << 0 << ' ' << 0;
		saveHighScores.close();
	}
	std::ifstream loadHighScores("resources.dat", std::ios_base::in);
	Game snake("SnakeGL by Lriend", 1920, 1080, 4, 4, true);
	loadHighScores >> snake;
	loadHighScores.close();

	//MAIN LOOP
	while (!snake.getWindowShouldClose()) {
		snake.update();
		snake.render();
	}

	std::ofstream saveHighScores("resources.dat", std::ios_base::out);
	saveHighScores << snake;
	saveHighScores.close();
	return 0;
}