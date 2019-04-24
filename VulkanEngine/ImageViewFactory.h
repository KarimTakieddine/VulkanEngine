#pragma once

#include "stdafx.h"
#include "ImageView.h"
#include "ImageFactory.h"

typedef std::shared_ptr<ImageView> ImageViewPtr;
typedef std::vector<ImageViewPtr> ImageViewPtrList;

class ImageViewFactory
{
public:

	static ImageViewPtrList createImageViews(DevicePtr const & device, VkSurfaceFormatKHR const & surfaceFormat, ImagePtrList const & images);
};