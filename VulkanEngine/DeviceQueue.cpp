#include "stdafx.h"
#include "DeviceQueue.h"

int DeviceQueue::getFamilyIndexMatching
(
	uint32_t requiredCount,
	VkQueueFlags requiredFlags,
	VkPhysicalDevice physicalDevice
)
{
	uint32_t deviceQueueFamilyCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyCount, nullptr);

	if (deviceQueueFamilyCount == 0)
	{
		return -1;
	}

	QueueFamilyPropertiesList queueFamilyPropertiesList(deviceQueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyCount, &queueFamilyPropertiesList[0]);

	for (uint32_t i = 0; i < queueFamilyPropertiesList.size(); ++i)
	{
		VkQueueFamilyProperties const & queueFamilyProperties = queueFamilyPropertiesList[i];

		if ((queueFamilyProperties.queueCount >= requiredCount) && ((queueFamilyProperties.queueFlags & requiredFlags) == requiredFlags))
		{
			return i;
		}
	}

	return -1;
}

DeviceQueue::DeviceQueue()
:
m_handle(VK_NULL_HANDLE)
{}

DeviceQueue::DeviceQueue
(
	uint32_t familyIndex,
	uint32_t queueIndex,
	VkDevice logicalDevice
)
:
m_handle(VK_NULL_HANDLE)
{
	initialize(familyIndex, queueIndex, logicalDevice);
}

void DeviceQueue::initialize
(
	uint32_t familyIndex,
	uint32_t queueIndex,
	VkDevice logicalDevice
)
{
	vkGetDeviceQueue(logicalDevice, familyIndex, queueIndex, &m_handle);
}

bool DeviceQueue::isValid() const
{
	return m_handle != VK_NULL_HANDLE;
}

VkQueue DeviceQueue::getHandle() const
{
	return m_handle;
}

DeviceQueue::DeviceQueue
(
	DeviceQueue const & other
)
:
m_handle(VK_NULL_HANDLE)
{}