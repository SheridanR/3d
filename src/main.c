#include "crap.h"
#include "sim.h"
#include "timer.h"
#include "draw.h"

int main(int argc, char* argv[]) {
    const char* file;
    if (argc >= 2) {
        file = argv[1];
    } else {
        file = "mesh/cube.obj";
    }
    init();             // create window
    setup(file);        // create world sim
    while (running) {
        events();       // get input, respond to OS events
        timer();        // update game timer
        update();       // move objects (eg the camera)
        clear();        // wipe the backbuffer
        draw();         // draw on the backbuffer
        refresh();      // flip the backbuffer to the front
    }
    close();            // destroy world sim
    term();             // destroy window
    return 0;
}