#pragma once

#include "stdafx.h"
#include "Image.h"
#include "SurfaceFactory.h"
#include "SwapChainFactory.h"

typedef std::shared_ptr<Image> ImagePtr;
typedef std::vector<ImagePtr> ImagePtrList;

class ImageFactory
{
public:

	static ImagePtrList createImages(DevicePtr const & device, SurfacePtr const & surface, SwapChainPtr const & swapChain);
};