#pragma once

#include "stdafx.h"
#include "Surface.h"
#include "VulkanInstanceFactory.h"
#include "WindowFactory.h"

typedef std::shared_ptr<Surface> SurfacePtr;

class SurfaceFactory
{
public:

	static SurfacePtr createSurface(VulkanInstancePtr const & instance, WindowPtr const & window);
};