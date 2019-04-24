#pragma once

#include "stdafx.h"

class Window
{
	friend class WindowFactory;

public:

	Window(Window const &) = delete;
	Window & operator=(Window const &) = delete;

	Window(Window && r_value);
	Window & operator=(Window && r_value);

	GLFWwindow const * getHandle() const;
	GLFWwindow * getHandle();

	~Window();

private:

	Window();

	GLFWwindow * m_handle;
};