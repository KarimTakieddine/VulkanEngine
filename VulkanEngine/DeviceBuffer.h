#pragma once

#include "stdafx.h"

class DeviceBuffer
{

public:

	friend class DeviceBufferFactory;

	DeviceBuffer(const DeviceBuffer &) = delete;
	DeviceBuffer &operator=(const DeviceBuffer &) = delete;

	DeviceBuffer(DeviceBuffer && r_value);

	DeviceBuffer & operator=(DeviceBuffer && r_value);

	VkDevice getDeviceHandle() const;

	VkBuffer getBufferHandle() const;

	VkBuffer getDeviceMemoryHandle() const;

	VkDeviceSize getDeviceMemorySize() const;

	~DeviceBuffer();

private:

	DeviceBuffer();

	void clear();

	void release();

	VkDevice m_deviceHandle;
	VkBuffer m_bufferHandle;
	VkDeviceMemory m_deviceMemoryHandle;
	VkDeviceSize m_deviceMemorySize;

};