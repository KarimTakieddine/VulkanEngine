#pragma once

#include "stdafx.h"
#include "VersionNumber.h"

typedef std::vector<char const *> CStringList;

class EngineApplication
{

public:

	static const VersionNumber APIVersion;
	static const VersionNumber ApplicationVersion;
	static const VersionNumber EngineVersion;

	static EngineApplication & getInstance();

	VkResult initialize(char const * applicationName, char const * engineName, CStringList const & layerNames, CStringList const & extensionNames);

	VkResult initializeWindow(char const * name, int width, int height);

	~EngineApplication();

private:

	static void onWindowResized(GLFWwindow * window, int width, int height);

	EngineApplication();

	GLFWwindow * m_window;
	VkInstance m_vulkanInstance;
};