#include "stdafx.h"
#include "SurfaceFactory.h"

SurfacePtr SurfaceFactory::createSurface(VulkanInstancePtr const & vulkanInstance, WindowPtr const & window)
{
	SurfacePtr surface(new Surface());

	surface->m_vulkanInstance = vulkanInstance;

	VkSurfaceKHR handle;

	if (glfwCreateWindowSurface(vulkanInstance->getState(), window->getHandle(), nullptr, &handle) != VK_SUCCESS)
	{
		return surface;
	}
	
	surface->m_handle = handle;
	
	return surface;
}