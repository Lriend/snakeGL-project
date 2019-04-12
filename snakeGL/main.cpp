#include "game.h"

int main() {
	Game game("Lorem Ipsum Title", 640, 480, 4, 5, true);

	//MAIN LOOP
	while (!game.getWindowShouldClose()) {
		game.update();
		game.render();
	}
	
	return 0;
}