#include "stdafx.h"
#include "DeviceBufferFactory.h"

bool DeviceBufferFactory::getMemoryTypeIndex
(
	VkPhysicalDevice physicalDevice,
	VkMemoryPropertyFlags propertyFlags,
	uint32_t memoryTypeFlags,
	uint32_t * outIndex
)
{
	VkPhysicalDeviceMemoryProperties memoryProperties;

	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	VkMemoryType const * memoryTypes = memoryProperties.memoryTypes;

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{
		if ( ( memoryTypeFlags & (1 << i) ) && ( ( memoryTypes[i].propertyFlags & propertyFlags ) == propertyFlags ) )
		{
			*outIndex = i;
			return true;
		}
	}

	return false;
}

std::shared_ptr<DeviceBuffer> DeviceBufferFactory::createDeviceBuffer
(
	VkDevice logicalDevice,
	VkPhysicalDevice physicalDevice,
	VkMemoryPropertyFlags propertyFlags,
	VkBufferCreateInfo const * bufferCreateInfo
)
{
	if (bufferCreateInfo == nullptr)
	{
		return nullptr;
	}

	VkBuffer bufferHandle;

	if (vkCreateBuffer(logicalDevice, bufferCreateInfo, nullptr, &bufferHandle) != VK_SUCCESS)
	{
		return nullptr;
	}

	VkMemoryRequirements bufferMemoryRequirements;

	vkGetBufferMemoryRequirements(logicalDevice, bufferHandle, &bufferMemoryRequirements);

	uint32_t memoryTypeIndex;

	if (!getMemoryTypeIndex(physicalDevice, propertyFlags, bufferMemoryRequirements.memoryTypeBits, &memoryTypeIndex))
	{
		return nullptr;
	}

	VkMemoryAllocateInfo memoryAllocationInfo;
	VkDeviceSize deviceMemorySize = bufferMemoryRequirements.size;

	memoryAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocationInfo.allocationSize = deviceMemorySize;
	memoryAllocationInfo.memoryTypeIndex = memoryTypeIndex;
	memoryAllocationInfo.pNext = nullptr;

	VkDeviceMemory deviceMemoryHandle;

	if (vkAllocateMemory(logicalDevice, &memoryAllocationInfo, nullptr, &deviceMemoryHandle) != VK_SUCCESS)
	{
		return nullptr;
	}

	std::shared_ptr<DeviceBuffer> deviceBufferPtr(new DeviceBuffer());

	deviceBufferPtr->m_deviceHandle = logicalDevice;
	deviceBufferPtr->m_bufferHandle = bufferHandle;
	deviceBufferPtr->m_deviceMemoryHandle = deviceMemoryHandle;
	deviceBufferPtr->m_deviceMemorySize = deviceMemorySize;

	return deviceBufferPtr;
}