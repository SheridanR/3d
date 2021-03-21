#include "crap.h"
#include "sim.h"
#include "timer.h"
#include "draw.h"

int main(int argc, char* argv[]) {
	init();				// create window
	setup();			// create world sim
	while (running) {
		events();		// get input, respond to OS events
		timer();		// update game timer
		update();		// move objects (eg the camera)
		clear();		// wipe the backbuffer
		draw();			// draw on the backbuffer
		refresh();		// flip the backbuffer to the front
	}
	term();				// destroy all objects
	return 0;
}