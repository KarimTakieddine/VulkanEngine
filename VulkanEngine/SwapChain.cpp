#include "stdafx.h"
#include "SwapChain.h"

SwapChain::SwapChain()
:
m_device(),
m_handle(VK_NULL_HANDLE)
{ }

SwapChain::SwapChain(SwapChain && r_value)
:
m_device(r_value.m_device)
{
	std::swap(m_handle, r_value.m_handle);
}

SwapChain & SwapChain::operator=(SwapChain && r_value)
{
	m_device = r_value.m_device;

	std::swap(m_handle, r_value.m_handle);

	return *this;
}

int SwapChain::getFormatIndex(VkSurfaceFormatKHR const & requiredFormat, SurfacePtr const & surface, VkPhysicalDevice const & physicalDevice)
{
	const VkSurfaceKHR surfaceInstance = surface->getHandle();

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surfaceInstance, &formatCount, nullptr);
	
	if (formatCount == 0)
	{
		return -1;
	}

	SurfaceFormatList deviceSurfaceFormats(formatCount);

	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surfaceInstance, &formatCount, deviceSurfaceFormats.data());

	int selectedSurfaceFormatIndex = -1;

	for (uint32_t i = 0; i < formatCount; ++i)
	{
		VkSurfaceFormatKHR const & surfaceFormat = deviceSurfaceFormats[i];

		if (
			surfaceFormat.colorSpace == requiredFormat.colorSpace &&
			surfaceFormat.format == requiredFormat.format
		)
		{
			return i;
		}
	}

	return -1;
}

VkSwapchainKHR SwapChain::getHandle() const
{
	return m_handle;
}

SwapChain::~SwapChain()
{
	vkDestroySwapchainKHR(m_device->getLogicalDevice(), m_handle, nullptr);
}