#include "stdafx.h"
#include "ImageFactory.h"

ImagePtrList ImageFactory::createImages(DevicePtr const & device, SurfacePtr const & surface, SwapChainPtr const & swapChain)
{
	ImagePtrList imageList;

	uint32_t imageCount = 0;

	const VkSwapchainKHR swapChainHandle = swapChain->getHandle();

	vkGetSwapchainImagesKHR(device->getLogicalDevice(), swapChainHandle, &imageCount, nullptr);

	if (imageCount == 0)
	{
		return imageList;
	}

	imageList.reserve(imageCount);

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	
	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device->getPhysicalDevice(), surface->getHandle(), &surfaceCapabilities) != VK_SUCCESS)
	{
		return imageList;
	}

	if (imageCount < surfaceCapabilities.minImageCount)
	{
		return imageList;
	}

	ImageList swapChainImages(imageCount);

	vkGetSwapchainImagesKHR(device->getLogicalDevice(), swapChainHandle, &imageCount, swapChainImages.data());

	for (size_t i = 0; i < swapChainImages.size(); ++i)
	{
		ImagePtr image(new Image());

		image->m_device = device;
		image->m_handle = swapChainImages[i];

		imageList.push_back(image);
	}

	return imageList;
}