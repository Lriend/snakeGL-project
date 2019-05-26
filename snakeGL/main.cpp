#include "game.h"

std::istream& operator >> (std::istream& in, Game& obj)
{
	in >> obj.highClassic;
	in >> obj.highQuick;
	return in;
}

std::ostream& operator << (std::ostream& out, const Game& obj)
{
	out << obj.highClassic << ' ';
	out << obj.highQuick << std::endl;
	return out;
}

int main() {
	
	Game snake("SnakeGL by Lriend", 1920, 1080, 4, 4, true, 0,0);
	//MAIN LOOP
	while (!snake.getWindowShouldClose()) {
		snake.update();
		snake.render();
	}

	std::cout << snake;
	system("pause");
	return 0;
}