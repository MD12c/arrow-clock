#include "Window.h"

Window::Window(const char* WINDOW_NAME, unsigned int width, unsigned int height, GLfloat RED, GLfloat GREEN, GLfloat BLUE)
	: m_WINDOW_NAME(WINDOW_NAME),
	m_width(width),
	m_height(height),
	m_RED(RED),
	m_GREEN(GREEN),
	m_BLUE(BLUE)
{}


void Window::glfwSetup() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // no title bar/border
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE); // transparent bg
	//glfwWindowHint(GLFW_FLOATING, GLFW_TRUE); // always on top
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(m_width, m_height, m_WINDOW_NAME, NULL, NULL);
	if (m_window == NULL)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(m_window);

	GLFWimage images[1];
	images[0].width  = 32;
	images[0].height = 32;

	images[0].pixels = stbi_load("Assets/Textures/clock2.png", &images[0].width, &images[0].height, 0, 4);
	if (images[0].pixels) {
		glfwSetWindowIcon(m_window, 1, images);
		stbi_image_free(images[0].pixels);
	}

	gladLoadGL();
	glViewport(0, 0, m_width, m_height);
	glClearColor(m_RED, m_GREEN, m_BLUE, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(m_window);
};

GLFWwindow* Window::getWindow() {
	return m_window;
}

Window::~Window() {
	if (m_window) {
		glfwDestroyWindow(m_window);
	}
	glfwTerminate();
}

void Window::glClearCurrentColor() {
	glClearColor(m_RED, m_GREEN, m_BLUE, 0.0f);
}