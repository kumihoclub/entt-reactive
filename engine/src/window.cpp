
#include "glad/glad.h"
#include "glad/src/glad.c"

#include "window.h"

namespace {

	u8 prev_keyboard_state[GLFW_KEY_LAST];
	u8 cur_keyboard_state[GLFW_KEY_LAST];
	b32 quit_event = false;
	i32 initial_width = 0;
	i32 initial_height = 0;
	b32 is_minimized = false;
	GLFWwindow* window_handle = nullptr;

}

b32 Window::Manager::init(const char* title, u32 w, u32 h) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	window_handle = glfwCreateWindow(w, h, title, NULL, NULL);
	if (!window_handle) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window_handle);

	glfwGetFramebufferSize(window_handle, &initial_width, &initial_height);

	memset(prev_keyboard_state, 0, GLFW_KEY_LAST);
	memset(cur_keyboard_state, 0, GLFW_KEY_LAST);

	glfwSetKeyCallback(window_handle, [](GLFWwindow * window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			cur_keyboard_state[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			cur_keyboard_state[key] = false;
		}
	});

	glfwSetWindowIconifyCallback(window_handle, [](GLFWwindow * window, int state) {
		if (window == window_handle) {
			is_minimized = state;
		}
	});

	int res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, initial_width, initial_height);
	glClearColor(0.175f, 0.225f, 0.250f, 1.0f);
}

void Window::Manager::update() {
	memcpy(prev_keyboard_state, cur_keyboard_state, GLFW_KEY_LAST);
	glfwPollEvents();
}

void Window::Manager::shutdown() {
	glfwDestroyWindow(window_handle);
	glfwTerminate();
}

void Window::Manager::show() {
	glfwShowWindow(window_handle);
}

void Window::Manager::hide() {
	glfwHideWindow(window_handle);
}

void Window::Manager::clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Manager::swap() {
	glfwSwapBuffers(window_handle);
}

b32 Window::Manager::quitEvent() {
	return glfwWindowShouldClose(window_handle);
}

b32 Window::Manager::minimized() {
	return is_minimized;
}

u32 Window::View::getWidth() {
	return initial_width;
}

u32 Window::View::getHeight() {
	return initial_height;
}

b32 Window::Control::keyPressed(u8 key) {
	return (cur_keyboard_state[key] == true && prev_keyboard_state[key] == false);
}

b32 Window::Control::keyHeld(u8 key) {
	return (cur_keyboard_state[key] == true && prev_keyboard_state[key] == true);
}

b32 Window::Control::keyReleased(u8 key) {
	return (cur_keyboard_state[key] == false && prev_keyboard_state[key] == true);
}