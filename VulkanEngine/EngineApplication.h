#pragma once

#include "stdafx.h"
#include "EngineContext.h"

class EngineApplication
{

public:

	static const VersionNumber APIVersion;
	static const VersionNumber ApplicationVersion;
	static const VersionNumber EngineVersion;

	static EngineApplication & getInstance();

	VkResult initializeContext(char const * applicationName, char const * engineName, CStringList const & layerNames, CStringList const & extensionNames);

	VkResult initializeWindow(char const * name, int width, int height);

	VkResult initializeDevice(VersionNumber const & driverVersion);

	static int getSupportedDeviceIndex(VersionNumber const & driverVersion, PhysicalDeviceList const & physicalDeviceList);

	static int getQueueFamilyIndex(VkPhysicalDevice physicalDevice, uint32_t requiredCount, VkQueueFlags requiredFlags);

	~EngineApplication();

private:

	static void onWindowResized(GLFWwindow * window, int width, int height);

	VkResult selectPhysicalDevice(VersionNumber const & driverVersion);

	VkResult initializeLogicalDevice();

	EngineApplication();

	GLFWwindow * m_window;
	VkInstance m_vulkanInstance;
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_logicalDevice;
	VkQueue m_deviceQueue;
};