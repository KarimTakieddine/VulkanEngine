#include "stdafx.h"
#include "Buffer.h"

Buffer::Buffer()
:
m_memoryRequirements(),
m_logicalDevice		(VK_NULL_HANDLE),
m_handle			(VK_NULL_HANDLE),
m_createStatus		(VK_ERROR_INITIALIZATION_FAILED)
{}

Buffer::Buffer
(
	VkDeviceSize size,
	VkBufferUsageFlags usageFlags,
	VkSharingMode sharingMode,
	QueueFamilyIndexList queueFamilyIndexList,
	VkDevice logicalDevice
)
:
m_memoryRequirements(),
m_logicalDevice		(logicalDevice),
m_handle			(VK_NULL_HANDLE)
{
	VkBufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.flags					= 0;
	bufferCreateInfo.pNext					= nullptr;
	bufferCreateInfo.size					= size;
	bufferCreateInfo.usage					= usageFlags;
	bufferCreateInfo.sharingMode			= sharingMode;
	bufferCreateInfo.queueFamilyIndexCount	= queueFamilyIndexList.size();
	bufferCreateInfo.pQueueFamilyIndices	= &queueFamilyIndexList[0];

	m_createStatus = vkCreateBuffer(logicalDevice, &bufferCreateInfo, nullptr, &m_handle);

	if (m_createStatus == VK_SUCCESS)
	{
		vkGetBufferMemoryRequirements(logicalDevice, m_handle, &m_memoryRequirements);
		m_byteSize = size;
	}
}

Buffer::Buffer(Buffer const & other)
:
m_memoryRequirements(),
m_logicalDevice		(VK_NULL_HANDLE),
m_handle			(VK_NULL_HANDLE),
m_createStatus		(VK_ERROR_INITIALIZATION_FAILED)
{}

VkBuffer Buffer::getHandle() const
{
	return m_handle;
}

VkMemoryRequirements const & Buffer::getMemoryRequirements() const
{
	return m_memoryRequirements;
}

bool Buffer::getMemoryTypeIndex
(
	VkPhysicalDevice physicalDevice,
	VkMemoryPropertyFlags requiredFlags,
	uint32_t * outIndex

) const
{
	VkPhysicalDeviceMemoryProperties memoryProperties;

	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	VkMemoryType const * memoryTypes = memoryProperties.memoryTypes;

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{
		if ((m_memoryRequirements.memoryTypeBits & (1 << i)) && ((memoryTypes[i].propertyFlags & requiredFlags) == requiredFlags))
		{
			*outIndex = i;
			return true;
		}
	}

	return false;
}

bool Buffer::allocate(VkPhysicalDevice physicalDevice)
{
	VkMemoryAllocateInfo memoryAllocateInfo;
	uint32_t memoryTypeIndex;

	if (!getMemoryTypeIndex(physicalDevice, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &memoryTypeIndex))
	{
		return false;
	}

	memoryAllocateInfo.sType			= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize	= m_memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex	= memoryTypeIndex;
	memoryAllocateInfo.pNext			= nullptr;

	return vkAllocateMemory(m_logicalDevice, &memoryAllocateInfo, nullptr, &m_deviceMemory) == VK_SUCCESS;
}

bool Buffer::fill
(
	void * hostData,
	VkDeviceSize offset
)
{
	void * deviceData = nullptr;

	if (vkMapMemory(m_logicalDevice, m_deviceMemory, 0, m_byteSize, 0, &deviceData) != VK_SUCCESS)
	{
		return false;
	}

	memcpy(deviceData, hostData, m_byteSize);

	vkUnmapMemory(m_logicalDevice, m_deviceMemory);

	return true;
}

Buffer::~Buffer()
{
	vkDestroyBuffer(m_logicalDevice, m_handle, nullptr);
	vkFreeMemory(m_logicalDevice, m_deviceMemory, nullptr);
}