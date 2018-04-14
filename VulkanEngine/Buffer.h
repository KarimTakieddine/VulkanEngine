#pragma once
#include "stdafx.h"

/*
	Buffers are the simplest type of resource and are used
	to store linear structured or raw data with optional
	formatting.
*/

typedef std::vector<uint32_t> QueueFamilyIndexList;

class Buffer
{
public:

	Buffer();

	Buffer
	(
		VkDeviceSize size,
		VkBufferUsageFlags usageFlags,
		VkSharingMode sharingMode,
		QueueFamilyIndexList queueFamilyIndexList,
		VkDevice logicalDevice
	);

	VkBuffer getHandle() const;

	VkMemoryRequirements const & getMemoryRequirements() const;

	~Buffer();

private:

	Buffer(Buffer const & other);

	VkMemoryRequirements	m_memoryRequirements;
	VkDevice				m_logicalDevice;
	VkBuffer				m_handle;
	VkResult				m_createStatus;
};