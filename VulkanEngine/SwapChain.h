#pragma once

#include "stdafx.h"
#include "DeviceFactory.h"
#include "SurfaceFactory.h"

class SwapChain
{
	friend class SwapChainFactory;

public:

	SwapChain(SwapChain const &) = delete;
	SwapChain & operator=(SwapChain const &) = delete;

	SwapChain(SwapChain && r_value);
	SwapChain & operator=(SwapChain && r_value);

	static int getFormatIndex(VkSurfaceFormatKHR const & requiredFormat, SurfacePtr const & surface, VkPhysicalDevice const & physicalDevice);

	VkSwapchainKHR getHandle() const;

	~SwapChain();

private:

	SwapChain();

	DevicePtr m_device;
	VkSwapchainKHR m_handle;
};