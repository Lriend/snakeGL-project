#include "game.h"

int main() {
	
		int widthfrommenu = 30;
		int heightfrommenu = 30;
		int fruitsfrommenu = 3;
	Game game("Lorem Ipsum Title", 640, 480, 4, 5, true, widthfrommenu, heightfrommenu, fruitsfrommenu);

	//MAIN LOOP
	while (!game.getWindowShouldClose()) {
		game.update();
		game.render();
	}
	
	return 0;
}