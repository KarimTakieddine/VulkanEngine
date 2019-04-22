#pragma once

#include "stdafx.h"
#include "InstanceContext.h"
#include "VulkanInstance.h"

typedef std::shared_ptr<VulkanInstance> VulkanInstancePtr;

class VulkanInstanceFactory
{
public:

	static VulkanInstancePtr createVulkanInstance(InstanceContext const & context);
};