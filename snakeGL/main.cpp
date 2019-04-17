#include "game.h"

int main() {
	
		int widthfrommenu = 36;
		int heightfrommenu = 18;
		int fruitsfrommenu = 3;
	Game game("Lorem Ipsum Title", 1920, 1080, 4, 5, true, widthfrommenu, heightfrommenu, fruitsfrommenu);

	//MAIN LOOP
	while (!game.getWindowShouldClose()) {
		game.update();
		game.render();
	}
	
	return 0;
}