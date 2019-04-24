#pragma once

#include "SurfaceFactory.h"
#include "SwapChain.h"
#include "SwapChainContext.h"

typedef std::shared_ptr<SwapChain> SwapChainPtr;

class SwapChainFactory
{
public:

	static SwapChainPtr createSwapChain(DevicePtr const & device, SurfacePtr const & surface, SwapChainContext const & context);
};