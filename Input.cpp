#include "input.h"

void processKeyboardInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
	
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

}