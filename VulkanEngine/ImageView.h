#pragma once

#include "stdafx.h"
#include "DeviceFactory.h"

class ImageView
{
	friend class ImageViewFactory;

public:

	ImageView(ImageView const &) = delete;
	ImageView & operator=(ImageView const &) = delete;

	ImageView(ImageView && r_value);
	ImageView & operator=(ImageView && r_value);

	VkImageView getHandle() const;

	~ImageView();

private:

	ImageView();

	DevicePtr m_device;
	VkImageView m_handle;
};