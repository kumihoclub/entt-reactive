
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
#include <algorithm>
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

	u32 test_draw_count = 4000;

	Transform* test_transform = new Transform[test_draw_count];
	Sprite* test_sprite = new Sprite[test_draw_count];

	for (int i = 0; i < test_draw_count; ++i) {
		float r = (float)rand() / (float)RAND_MAX;
		test_transform[i].pos.x = (-2.0 + r * (2.0 - -2.0));
	}

	for (int i = 0; i < test_draw_count; ++i) {
		float r = (float)rand() / (float)RAND_MAX;
		test_transform[i].pos.y = (-2.0 + r * (2.0 - -2.0));
	}

	for (int i = 0; i < test_draw_count; ++i) {
		test_transform[i].scale = { 3.0f, 3.0f };
		test_transform[i].layer = 0;
		test_sprite[i].size = { 32, 32 };
		test_sprite[i].texture = Texture::fetch("texture\\charset\\mako_world.png").value();
		test_sprite[i].uv = { 0.0f, 0.0f };
	}

	test_transform[0].pos.x = 0.0f;
	test_transform[0].pos.y = 0.0f;
	test_transform[0].layer = 0;

	window.show();

	f64 t = 0.0;
	f64 dt = 1 / 60.0;

	f64 current_time = glfwGetTime();
	f64 prev_time = 0.0;
	f64 frame_time = (1.0 / 60);
	f64 delta_time = 0.0;
	f64 accumulator = 0.0;
	//f64 delta_time = 0.0;

	while (!window.quitEvent()) {
		
		f64 current_time = glfwGetTime();
		delta_time += (current_time - prev_time);
		prev_time = current_time;
		if (delta_time >= frame_time) {
			timing.update(delta_time);
			window.update();
			if (Window::Control::keyHeld(GLFW_KEY_A)) {
				test_transform[0].pos.x -= 0.2 * (Timing::deltaTime() / (1.0 / 60));
			}
			else if (Window::Control::keyHeld(GLFW_KEY_D)) {
				test_transform[0].pos.x += 0.2 * (Timing::deltaTime() / (1.0 / 60));
			}
			if (Window::Control::keyHeld(GLFW_KEY_W)) {
				test_transform[0].pos.y += 0.2 * (Timing::deltaTime() / (1.0 / 60));
			}
			else if (Window::Control::keyHeld(GLFW_KEY_S)) {
				test_transform[0].pos.y -= 0.2 * (Timing::deltaTime() / (1.0 / 60));
			}

			if (!renderer.is_drawing()) {
				for (int i = 0; i < test_draw_count; ++i) {
					frame.createDrawCommand(test_transform[i], test_sprite[i]);
				}
				renderer.draw(frame);
			}
			delta_time = 0;
		}
	}

	delete[] test_transform;
	delete[] test_sprite;


	window.hide();

	renderer.wait();

	renderer.shutdown();
	shader_manager.shutdown();
	texture_manager.shutdown();
	database.shutdown();
	window.shutdown();
	return 0;
}