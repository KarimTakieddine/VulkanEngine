#pragma once

#include "stdafx.h"
#include "Window.h"
#include "WindowContext.h"

typedef std::shared_ptr<Window> WindowPtr;

class WindowFactory
{
public:

	static WindowPtr createWindow(WindowContext const & context);
};