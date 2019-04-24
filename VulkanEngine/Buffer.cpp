#include "stdafx.h"
#include "Buffer.h"

Buffer::Buffer()
:
m_device(),
m_handle(VK_NULL_HANDLE),
m_deviceMemory(VK_NULL_HANDLE),
m_byteSize(),
m_deviceSize()
{}

Buffer::Buffer(Buffer && r_value)
:
m_device(r_value.m_device)
{
	std::swap(m_handle, r_value.m_handle);
	std::swap(m_deviceMemory, r_value.m_deviceMemory);
	std::swap(m_byteSize, r_value.m_byteSize);
	std::swap(m_deviceSize, r_value.m_deviceSize);
}

Buffer & Buffer::operator=(Buffer && r_value)
{
	m_device = r_value.m_device;

	std::swap(m_handle, r_value.m_handle);
	std::swap(m_deviceMemory, r_value.m_deviceMemory);
	std::swap(m_byteSize, r_value.m_byteSize);
	std::swap(m_deviceSize, r_value.m_deviceSize);

	return *this;
}

VkBuffer Buffer::getHandle() const
{
	return m_handle;
}

int Buffer::getMemoryTypeIndex
(
	VkMemoryPropertyFlags requiredFlags,
	VkMemoryRequirements const & memoryRequirements,
	VkPhysicalDevice const & physicalDevice

)
{
	VkPhysicalDeviceMemoryProperties memoryProperties;

	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	VkMemoryType const * memoryTypes = memoryProperties.memoryTypes;

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{
		if ((memoryRequirements.memoryTypeBits & (1 << i)) && ((memoryTypes[i].propertyFlags & requiredFlags) == requiredFlags))
		{
			return i;
		}
	}

	return -1;
}

VkResult Buffer::fill
(
	void * hostData,
	VkDeviceSize offset
)
{
	void * deviceData = nullptr;

	VkDevice const & logicalDevice = m_device->getLogicalDevice();

	VkResult memoryMapStatus = vkMapMemory(logicalDevice, m_deviceMemory, 0, m_byteSize, 0, &deviceData);

	if (memoryMapStatus != VK_SUCCESS)
	{
		return memoryMapStatus;
	}

	memcpy(deviceData, hostData, m_byteSize);

	vkUnmapMemory(logicalDevice, m_deviceMemory);

	return VK_SUCCESS;
}

Buffer::~Buffer()
{
	VkDevice const & logicalDevice = m_device->getLogicalDevice();

	vkDestroyBuffer(logicalDevice, m_handle, nullptr);
	vkFreeMemory(logicalDevice, m_deviceMemory, nullptr);
}