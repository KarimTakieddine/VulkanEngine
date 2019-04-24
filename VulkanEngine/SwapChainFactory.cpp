#include "stdafx.h"
#include "SwapChainFactory.h"

SwapChainPtr SwapChainFactory::createSwapChain(DevicePtr const & device, SurfacePtr const & surface, SwapChainContext const & context)
{
	SwapChainPtr swapChain(new SwapChain());

	swapChain->m_device = device;

	VkPhysicalDevice const & physicalDevice = device->getPhysicalDevice();
	const VkSurfaceKHR surfaceHandle = surface->getHandle();

	std::vector<uint32_t> const & queueFamilyIndices = device->getQueueFamilyIndices();

	for (std::vector<uint32_t>::const_iterator i = queueFamilyIndices.begin(); i != queueFamilyIndices.end(); ++i)
	{
		VkBool32 deviceSupportsPresentation = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, *i, surfaceHandle, &deviceSupportsPresentation);

		if (!deviceSupportsPresentation)
		{
			return swapChain;
		}
	}

	VkSurfaceFormatKHR const & surfaceFormat = context.m_surfaceFormat;

	if (SwapChain::getFormatIndex(surfaceFormat, surface, physicalDevice) == -1)
	{
		return swapChain;
	}

	VkSurfaceCapabilitiesKHR deviceSurfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surfaceHandle, &deviceSurfaceCapabilities);

	VkSwapchainKHR handle;

	VkSwapchainCreateInfoKHR swapChainCreateInfo;
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.flags = 0;
	swapChainCreateInfo.clipped = context.m_clipped;
	swapChainCreateInfo.surface = surfaceHandle;
	swapChainCreateInfo.minImageCount = deviceSurfaceCapabilities.minImageCount;
	swapChainCreateInfo.imageExtent = deviceSurfaceCapabilities.currentExtent;
	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = deviceSurfaceCapabilities.supportedUsageFlags;
	swapChainCreateInfo.imageSharingMode = context.m_sharingMode;
	swapChainCreateInfo.queueFamilyIndexCount = 1;
	swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.presentMode = context.m_presentMode;
	swapChainCreateInfo.preTransform = deviceSurfaceCapabilities.currentTransform;
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	swapChainCreateInfo.pNext = nullptr;

	if (vkCreateSwapchainKHR(device->getLogicalDevice(), &swapChainCreateInfo, nullptr, &handle) != VK_SUCCESS)
	{
		return swapChain;
	}

	swapChain->m_handle = handle;

	return swapChain;
}