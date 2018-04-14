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

Buffer::~Buffer()
{
	vkDestroyBuffer(m_logicalDevice, m_handle, nullptr);
}