#pragma once

#include "stdafx.h"
#include "DeviceBuffer.h"

typedef std::shared_ptr<DeviceBuffer> DeviceBufferPtr;

class DeviceBufferFactory
{

public:

	static bool getMemoryTypeIndex(VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags propertyFlags, uint32_t memoryTypeFlags, uint32_t * outIndex);

	static DeviceBufferPtr createDeviceBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags propertyFlags, VkBufferCreateInfo const * bufferCreateInfo);

};