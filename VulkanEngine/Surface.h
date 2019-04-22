#pragma once

#include "stdafx.h"
#include "VulkanInstance.h"

class Surface
{
public:

	VkSurfaceKHR getHandle() const;

	~Surface();

private:

	Surface();

	std::shared_ptr<VulkanInstance> m_vulkanInstance;
	VkSurfaceKHR m_handle;
};