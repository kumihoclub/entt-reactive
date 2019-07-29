
#include "window.h"
#include "graphics.h"
#include "sim.h"
#include "frame.h"
#include "config.h"
#include "physics.h"

#include "glm/gtc/matrix_transform.hpp"

//TEST
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <algorithm>
///

Config config;
FrameContext frame;
Window window;
Graphics graphics;
Sim sim;

int main(int argc, char *argv[]) {

	config.title = "Black Telephone";
	config.window_size = { 960, 540 };
	config.sprite_renderable_capacity = 500;
	config.unit_pixels = 32;
	Config::set(config);

	frame.startup();
	window.startup(frame);
	graphics.startup(frame);
	sim.startup(frame);

	window.show();

	while (!window.quitEvent()) {
		window.recordInput();
		sim.step();
		graphics.draw();
		sim.wait();
		window.swap();
		frame.swapFrames();
	}

	window.hide();

	sim.shutdown();
	graphics.shutdown();
	window.shutdown();
	frame.shutdown();

	return 0;
}