
#include "window.h"
#include "database.h"
#include "texture.h"
#include "shader.h"
#include "timing.h"
#include "renderer.h"
#include "frame.h"

//TEST
#include <cstdlib>
#include <iostream>
#include <ctime>
///

Window::Manager window;
Database::Manager database;
Texture::Manager texture_manager;
Shader::Manager shader_manager;
Timing::Manager timing;
Renderer renderer;
Frame frame;

int main(int argc, char *argv[]) {
	window.init("Test", 960, 540);
	database.init();
	texture_manager.init();
	shader_manager.init();
	renderer.init();
	timing.init();

	window.show();

	//TEST
	
	Transform* test_transform = new Transform[50000];
	Sprite* test_sprite = new Sprite[50000];

	const int maximum_number = 31;
	const int minimum_number = -32;
	unsigned int i;

	for (int i = 0; i < 10000; ++i) {
		float r = (float)rand() / (float)RAND_MAX;
		test_transform[i].pos.x = ( -8.0 + r * (8.0 - -8.0));
	}

	for (int i = 0; i < 50000; ++i) {
		float r = (float)rand() / (float)RAND_MAX;
		test_transform[i].pos.y = (-8.0 + r * (8.0 - -8.0));
	}

	for (int i = 0; i < 50000; ++i) {
		test_transform[i].scale = { 3.0f, 3.0f };
		test_transform[i].layer = 0;
		test_sprite[i].size = { 32, 32 };
		test_sprite[i].texture = Texture::fetch("texture\\charset\\mako_world.png").value();
		test_sprite[i].uv = { 0.0f, 0.0f };
	}

	Timing::Timer frame_timer;
	frame_timer.setTime(1.0 / 60.0);
	frame_timer.run();
	while (!window.quitEvent()) {
		timing.update();
		window.update();
		if (!window.minimized()) {
			if (frame_timer.triggered()) {
				if (Window::Control::keyHeld(GLFW_KEY_A)) {
					test_transform[0].pos.x -= 0.2;
				}
				else if (Window::Control::keyHeld(GLFW_KEY_D)) {
					test_transform[0].pos.x += 0.2;
				}
				if (Window::Control::keyHeld(GLFW_KEY_W)) {
					test_transform[0].pos.y += 0.2;
				}
				else if (Window::Control::keyHeld(GLFW_KEY_S)) {
					test_transform[0].pos.y -= 0.2;
				}

				for (int i = 0; i < 50000; ++i) {
					frame.createWorldDrawCommand(test_transform[i], test_sprite[i]);
				}

				renderer.draw(frame);
				window.swap();
				frame_timer.reset();
			}
		}
	}

	delete[] test_transform;
	delete[] test_sprite;


	window.hide();

	shader_manager.shutdown();
	texture_manager.shutdown();
	database.shutdown();
	window.shutdown();
	return 0;
}