#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "draw.h"
#include "crap.h"

bool running = true;

int main(int argc, char* argv[]) {
	init();
	while (running) {
		update();
		clear();
		refresh();
	}
	term();
	return 0;
}