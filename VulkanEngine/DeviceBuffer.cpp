#include "stdafx.h"
#include "DeviceBuffer.h"

DeviceBuffer::DeviceBuffer()
{
	clear();
}

DeviceBuffer::DeviceBuffer(DeviceBuffer && r_value)
{
	clear();

	std::swap(m_deviceHandle, r_value.m_deviceHandle);
	std::swap(m_bufferHandle, r_value.m_bufferHandle);
	std::swap(m_bufferSize, r_value.m_bufferSize);
	std::swap(m_deviceMemoryHandle, r_value.m_deviceMemoryHandle);
	std::swap(m_deviceMemorySize, r_value.m_deviceMemorySize);
}

DeviceBuffer & DeviceBuffer::operator=(DeviceBuffer && r_value)
{
	if (this == &r_value)
	{
		return *this;
	}

	release();

	std::swap(m_deviceHandle, r_value.m_deviceHandle);
	std::swap(m_bufferHandle, r_value.m_bufferHandle);
	std::swap(m_bufferSize, r_value.m_bufferSize);
	std::swap(m_deviceMemoryHandle, r_value.m_deviceMemoryHandle);
	std::swap(m_deviceMemorySize, r_value.m_deviceMemorySize);

	return *this;
}

VkDevice DeviceBuffer::getDeviceHandle() const
{
	return m_deviceHandle;
}

VkBuffer DeviceBuffer::getBufferHandle() const
{
	return m_bufferHandle;
}

VkBuffer DeviceBuffer::getDeviceMemoryHandle() const
{
	return m_deviceMemoryHandle;
}

VkDeviceSize DeviceBuffer::getDeviceMemorySize() const
{
	return m_deviceMemorySize;
}

VkResult DeviceBuffer::bind(VkDeviceSize offset) const
{
	return vkBindBufferMemory(m_deviceHandle, m_bufferHandle, m_deviceMemoryHandle, offset);
}

VkResult DeviceBuffer::write(void const * data) const
{
	void * deviceData = nullptr;

	VkResult memoryMapResult = vkMapMemory(m_deviceHandle, m_deviceMemoryHandle, 0, m_bufferSize, 0, &deviceData);

	if (memoryMapResult != VK_SUCCESS)
	{
		return memoryMapResult;
	}

	memcpy(deviceData, data, m_bufferSize);

	vkUnmapMemory(m_deviceHandle, m_deviceMemoryHandle);

	return VK_SUCCESS;
}

DeviceBuffer::~DeviceBuffer()
{
	release();
}

void DeviceBuffer::clear()
{
	m_deviceHandle = VK_NULL_HANDLE;
	m_bufferHandle = VK_NULL_HANDLE;
	m_bufferSize = 0;
	m_deviceMemoryHandle = VK_NULL_HANDLE;
	m_deviceMemorySize = 0;
}

void DeviceBuffer::release()
{
	if (m_deviceHandle != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_deviceHandle, m_bufferHandle, nullptr);
		vkFreeMemory(m_deviceHandle, m_deviceMemoryHandle, nullptr);
	}

	clear();
}