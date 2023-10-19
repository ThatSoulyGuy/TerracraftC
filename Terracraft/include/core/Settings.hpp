#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include "core/Window.hpp"

namespace Settings
{
	extern std::string defaultDomain;
	extern Window mainWindow;
	extern std::map<std::string, void*> randomData;

	void InitGLFW()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 16);

		defaultDomain = "terracraft";
	}

	template<typename T>
	T& ConvertRandom(const std::string& name)
	{
		return *(T*)randomData[name];
	}
}

std::string Settings::defaultDomain;
Window Settings::mainWindow;
std::map<std::string, void*> Settings::randomData;

#endif // !SETTINGS_HPP