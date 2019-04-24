#include "stdafx.h"
#include "WindowFactory.h"

WindowPtr WindowFactory::createWindow(WindowContext const & context)
{
	WindowPtr window(new Window());

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow * handle = glfwCreateWindow(context.m_width, context.m_height, context.m_name.c_str(), nullptr, nullptr);

	if (handle == nullptr)
	{
		return window;
	}

	window->m_handle = handle;

	return window;
}
