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

	VkDeviceMemory getMemoryHandle() const;

	VkMemoryRequirements const & getMemoryRequirements() const;

	bool getMemoryTypeIndex
	(
		VkPhysicalDevice physicalDevice,
		VkMemoryPropertyFlags requiredFlags,
		uint32_t * outIndex
	) const;

	bool allocate
	(
		VkPhysicalDevice physicalDevice,
		VkMemoryPropertyFlags memoryPropertyFlags
	);

	bool fill
	(
		void * hostData,
		VkDeviceSize offset
	);

	~Buffer();

private:

	Buffer(Buffer const & other);

	VkMemoryRequirements	m_memoryRequirements;
	VkDeviceMemory			m_deviceMemory;
	VkDeviceSize			m_byteSize;
	VkBuffer				m_handle;
	VkDevice				m_logicalDevice;
	VkResult				m_createStatus;
};