#pragma once

#include "stdafx.h"
#include "Device.h"
#include "Surface.h"

class SwapChain
{
public:

	SwapChain(SwapChain const &) = delete;
	SwapChain & operator=(SwapChain const &) = delete;

	SwapChain(SwapChain && r_value);
	SwapChain & operator=(SwapChain && r_value);

	static int getFormatIndex(VkPhysicalDevice physicalDevice, VkSurfaceFormatKHR requiredFormat, std::shared_ptr<Surface> const & surface);

	VkSwapchainKHR getHandle() const;

	~SwapChain();

private:

	SwapChain();

	SwapChain(std::shared_ptr<Device> const & device);

	void release();

	void clear();

	std::shared_ptr<Device> m_device;
	VkSwapchainKHR m_handle;
};