#include "game.h"

int main() {
		Game snake("Lorem Ipsum Title", 640, 480, 4, 5, true);
	//MAIN LOOP
	while (!snake.getWindowShouldClose()) {
		snake.update();
		snake.render();
	}
	return 0;
}