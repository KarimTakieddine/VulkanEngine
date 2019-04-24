#pragma once

#include "stdafx.h"
#include "DeviceFactory.h"

class Image
{
	friend class ImageFactory;

public:

	Image(Image const &) = delete;
	Image & operator=(Image const &) = delete;

	Image(Image && r_value);
	Image & operator=(Image && r_value);

	VkImage getHandle() const;

	~Image();

private:

	Image();

	DevicePtr m_device;
	VkImage m_handle;
};