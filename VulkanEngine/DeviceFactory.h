#pragma once

#include "DeviceContext.h"
#include "Device.h"
#include "VulkanInstanceFactory.h"

typedef std::shared_ptr<Device> DevicePtr;

class DeviceFactory
{
public:

	static DevicePtr createDevice(VulkanInstancePtr const & vulkanInstance, DeviceContext const & deviceContext);
};