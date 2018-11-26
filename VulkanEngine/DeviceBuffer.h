#pragma once

#include "stdafx.h"

class DeviceBuffer
{

public:

	DeviceBuffer()
	{
		clear();
	}

	DeviceBuffer(const DeviceBuffer &) = delete;
	DeviceBuffer &operator=(const DeviceBuffer &) = delete;

	DeviceBuffer
	(
		DeviceBuffer && r_value
	)
	{
		/* 
			TODO: Consider efficiency of this approach
			relative to simple member assignment.
		*/

		std::move(r_value);

		clear();
		
		std::swap(m_deviceHandle, r_value.m_deviceHandle);
		std::swap(m_bufferHandle, r_value.m_bufferHandle);
		std::swap(m_deviceMemoryHandle, r_value.m_deviceMemoryHandle);
		std::swap(m_deviceMemorySize, r_value.m_deviceMemorySize);
	}

	DeviceBuffer & operator=(DeviceBuffer && r_value)
	{
		// Check for self-assignment

		if (this == &r_value)
		{
			return *this;
		}

		release();

		std::swap(m_deviceHandle, r_value.m_deviceHandle);
		std::swap(m_bufferHandle, r_value.m_bufferHandle);
		std::swap(m_deviceMemoryHandle, r_value.m_deviceMemoryHandle);
		std::swap(m_deviceMemorySize, r_value.m_deviceMemorySize);

		return *this;
	}

	~DeviceBuffer()
	{
		release();
	}

private:

	void clear()
	{
		m_deviceHandle = VK_NULL_HANDLE;
		m_bufferHandle = VK_NULL_HANDLE;
		m_deviceMemoryHandle = VK_NULL_HANDLE;
		m_deviceMemorySize = 0;
	}

	void release()
	{
		if (m_deviceHandle != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(m_deviceHandle, m_bufferHandle, nullptr);
			vkFreeMemory(m_deviceHandle, m_deviceMemoryHandle, nullptr);
		}

		clear();
	}

	VkDevice m_deviceHandle;
	VkBuffer m_bufferHandle;
	VkDeviceMemory m_deviceMemoryHandle;
	VkDeviceSize m_deviceMemorySize;

};