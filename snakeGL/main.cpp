#include "game.h"

int main() {
		Game snake("SnakeGL by Lriend", 1920, 1080, 4, 4, true);
	//MAIN LOOP
	while (!snake.getWindowShouldClose()) {
		snake.update();
		snake.render();
	}
	return 0;
}