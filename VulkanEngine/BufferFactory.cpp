#include "stdafx.h"
#include "BufferFactory.h"

BufferPtr BufferFactory::createBuffer(DevicePtr const & device, BufferContext const & context, VkDeviceSize byteSize)
{
	BufferPtr buffer(new Buffer());

	buffer->m_device = device;

	VkBufferCreateInfo bufferCreateInfo;

	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.flags = 0;
	bufferCreateInfo.pQueueFamilyIndices = device->getQueueFamilyIndices().data();
	bufferCreateInfo.queueFamilyIndexCount = 1;
	bufferCreateInfo.usage = context.m_usage;
	bufferCreateInfo.sharingMode = context.m_sharingMode;
	bufferCreateInfo.size = byteSize;
	bufferCreateInfo.pNext = nullptr;

	VkBuffer handle;

	VkDevice const & logicalDevice = device->getLogicalDevice();

	if (vkCreateBuffer(logicalDevice, &bufferCreateInfo, nullptr, &handle) != VK_SUCCESS)
	{
		return buffer;
	}

	buffer->m_handle = handle;
	buffer->m_byteSize = byteSize;

	VkMemoryRequirements bufferMemoryRequirements;
	vkGetBufferMemoryRequirements(logicalDevice, handle, &bufferMemoryRequirements);

	int memoryTypeIndex = Buffer::getMemoryTypeIndex(context.m_memoryProperties, bufferMemoryRequirements, device->getPhysicalDevice());

	if (memoryTypeIndex == -1)
	{
		return buffer;
	}

	VkMemoryAllocateInfo memoryAllocateInfo;

	const VkDeviceSize deviceSize = bufferMemoryRequirements.size;

	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = deviceSize;
	memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;
	memoryAllocateInfo.pNext = nullptr;

	VkDeviceMemory deviceMemory;

	if (vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &deviceMemory) != VK_SUCCESS)
	{
		return buffer;
	}

	buffer->m_deviceMemory = deviceMemory;
	buffer->m_deviceSize = deviceSize;

	return buffer;
}