#pragma once

#include "stdafx.h"
#include "VulkanInstanceFactory.h"

class Surface
{
	friend class SurfaceFactory;

public:

	Surface(Surface const &) = delete;
	Surface & operator=(Surface const &) = delete;

	Surface(Surface && r_value);
	Surface & operator=(Surface && r_value);

	VkSurfaceKHR getHandle() const;

	~Surface();

private:

	Surface();

	VulkanInstancePtr m_vulkanInstance;
	VkSurfaceKHR m_handle;
};