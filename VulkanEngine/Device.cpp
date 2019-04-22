#include "stdafx.h"
#include "Device.h"

int Device::getPhysicalDeviceIndex
(
	VersionNumber const & minimumApiVersion,
	VersionNumber const & minimumDriverVersion,
	PhysicalDeviceList const & physicalDeviceList
)
{
	for (uint32_t i = 0; i < physicalDeviceList.size(); ++i)
	{
		VkPhysicalDeviceProperties deviceProperties;

		vkGetPhysicalDeviceProperties(physicalDeviceList[i], &deviceProperties);

		VersionNumber deviceAPIVersion = VersionNumber::fromInteger(deviceProperties.apiVersion);
		VersionNumber deviceDriverVersion = VersionNumber::fromInteger(deviceProperties.driverVersion);

		if (
			deviceAPIVersion >= minimumApiVersion &&
			deviceDriverVersion >= minimumDriverVersion
		)
		{
			return i;
		}
	}

	return -1;
}

int Device::getQueueFamilyIndex
(
	VkPhysicalDevice physicalDevice,
	uint32_t requiredCount,
	VkQueueFlags requiredFlags
)
{
	uint32_t deviceQueueFamilyCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyCount, nullptr);

	if (deviceQueueFamilyCount == 0)
	{
		return -1;
	}

	std::vector<VkQueueFamilyProperties> queueFamilyPropertiesList(deviceQueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyCount, queueFamilyPropertiesList.data());

	for (uint32_t i = 0; i < queueFamilyPropertiesList.size(); ++i)
	{
		VkQueueFamilyProperties const & queueFamilyProperties = queueFamilyPropertiesList[i];

		if (
			(queueFamilyProperties.queueCount >= requiredCount) &&
			((queueFamilyProperties.queueFlags & requiredFlags) == requiredFlags)
		)
		{
			return i;
		}
	}

	return -1;
}

Device::Device()
:
m_logicalDevice(VK_NULL_HANDLE),
m_physicalDevice(VK_NULL_HANDLE),
m_queueFamilyIndex()
{

}

Device::Device(Device && r_value)
{
	std::swap(m_logicalDevice, r_value.m_logicalDevice);
	std::swap(m_physicalDevice, r_value.m_physicalDevice);
	std::swap(m_queueFamilyIndex, r_value.m_queueFamilyIndex);
}

Device & Device::operator=(Device && r_value)
{
	std::swap(m_logicalDevice, r_value.m_logicalDevice);
	std::swap(m_physicalDevice, r_value.m_physicalDevice);
	std::swap(m_queueFamilyIndex, r_value.m_queueFamilyIndex);

	return *this;
}

VkDevice Device::getLogicalDevice()
{
	return m_logicalDevice;
}

VkDevice const & Device::getLogicalDevice() const
{
	return m_logicalDevice;
}

VkPhysicalDevice Device::getPhysicalDevice()
{
	return m_physicalDevice;
}

VkPhysicalDevice const & Device::getPhysicalDevice() const
{
	return m_physicalDevice;
}

Device::~Device()
{
	vkDestroyDevice(m_logicalDevice, nullptr);
}