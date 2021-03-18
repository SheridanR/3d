#include "draw.h"
#include "camera.h"
#include "timer.h"
#include "sim.h"
#include "crap.h"
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
	init();
	setup();
	while (running) {
		events();
		timer();
		update();
		clear();
		draw(&camera);
		refresh();
	}
	term();
	return 0;
}