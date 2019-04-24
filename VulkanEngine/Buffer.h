#pragma once

#include "stdafx.h"
#include "DeviceFactory.h"

/*
	Buffers are the simplest type of resource and are used
	to store linear structured or raw data with optional
	formatting.
*/

typedef std::vector<uint32_t> QueueFamilyIndexList;

class Buffer
{
	friend class BufferFactory;

public:

	Buffer(Buffer const &) = delete;
	Buffer & operator=(Buffer const &) = delete;

	Buffer(Buffer && r_value);
	Buffer & operator=(Buffer && r_value);

	VkBuffer getHandle() const;

	static int getMemoryTypeIndex
	(
		VkMemoryPropertyFlags requiredFlags,
		VkMemoryRequirements const & memoryRequirements,
		VkPhysicalDevice const & physicalDevice
	);

	VkResult fill
	(
		void * hostData,
		VkDeviceSize offset = 0
	);

	~Buffer();

private:

	Buffer();

	DevicePtr m_device;
	VkDeviceMemory m_deviceMemory;
	VkDeviceSize m_byteSize;
	VkDeviceSize m_deviceSize;
	VkBuffer m_handle;
};