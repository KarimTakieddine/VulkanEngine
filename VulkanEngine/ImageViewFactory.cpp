#include "stdafx.h"
#include "ImageViewFactory.h"

ImageViewPtrList ImageViewFactory::createImageViews(DevicePtr const & device, VkSurfaceFormatKHR const & surfaceFormat, ImagePtrList const & images)
{
	ImageViewPtrList imageViews;

	imageViews.reserve(images.size());

	VkDevice const & logicalDevice = device->getLogicalDevice();

	for (ImagePtrList::const_iterator i = images.begin(); i != images.end(); ++i)
	{
		VkComponentMapping imageComponentMapping;
		imageComponentMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageComponentMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageComponentMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageComponentMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		VkImageSubresourceRange imageSubResourceRange;
		imageSubResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageSubResourceRange.baseMipLevel = 0;
		imageSubResourceRange.levelCount = 1;
		imageSubResourceRange.baseArrayLayer = 0;
		imageSubResourceRange.layerCount = 1;

		VkImageViewCreateInfo imageViewCreateInfo;
		imageViewCreateInfo.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.flags				= 0;
		imageViewCreateInfo.image				= (*i)->getHandle();
		imageViewCreateInfo.format				= surfaceFormat.format;
		imageViewCreateInfo.viewType			= VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.components			= imageComponentMapping;
		imageViewCreateInfo.subresourceRange	= imageSubResourceRange;
		imageViewCreateInfo.pNext				= nullptr;

		VkImageView handle;

		if (vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &handle) != VK_SUCCESS)
		{
			continue;
		}

		ImageViewPtr imageView(new ImageView());

		imageView->m_device = device;
		imageView->m_handle = handle;

		imageViews.push_back(imageView);
	}

	return imageViews;
}