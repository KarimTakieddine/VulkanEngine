#include "stdafx.h"
#include "Window.h"

Window::Window(Window && r_value)
{
	std::swap(m_handle, r_value.m_handle);
}

Window & Window::operator=(Window && r_value)
{
	std::swap(m_handle, r_value.m_handle);

	return *this;
}

GLFWwindow const * Window::getHandle() const
{
	return m_handle;
}

GLFWwindow * Window::getHandle()
{
	return m_handle;
}

Window::~Window()
{
	glfwDestroyWindow(m_handle);
}

Window::Window()
:
m_handle(nullptr)
{
	
}