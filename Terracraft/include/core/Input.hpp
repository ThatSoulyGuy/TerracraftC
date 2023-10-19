#ifndef INPUT_HPP
#define INPUT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace Input
{
	extern GLFWwindow* window;
	extern glm::vec2 mousePosition;

	void InitInput(GLFWwindow* _window)
	{
		window = _window;
	}

	bool GetKeyDown(int key)
	{
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	bool GetKeyUp(int key)
	{
		return glfwGetKey(window, key) == GLFW_RELEASE;
	}

	bool GetMouseButtonDown(int button)
	{
		return glfwGetMouseButton(window, button) == GLFW_PRESS;
	}

	bool GetMouseButtonUp(int button)
	{
		return glfwGetMouseButton(window, button) == GLFW_RELEASE;
	}

	void SetCursorMode(const bool& value)
	{
		if (!value)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

GLFWwindow* Input::window;
glm::vec2 Input::mousePosition;

#endif // !INPUT_HPP