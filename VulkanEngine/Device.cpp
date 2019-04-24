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
	VkPhysicalDevice const & physicalDevice,
	VkQueueFamilyProperties const & requiredQueueFamilyProperties,
	QueueFamilyPropertiesList const & queueFamilyPropertiesList
)
{
	for (uint32_t i = 0; i < queueFamilyPropertiesList.size(); ++i)
	{
		VkQueueFamilyProperties const & queueFamilyProperties = queueFamilyPropertiesList[i];

		if (
			( queueFamilyProperties.queueCount >= requiredQueueFamilyProperties.queueCount ) &&
			( (queueFamilyProperties.queueFlags & requiredQueueFamilyProperties.queueFlags) == requiredQueueFamilyProperties.queueFlags )
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
m_queueFamilyIndices()
{

}

Device::Device(Device && r_value)
:
m_queueFamilyIndices(r_value.m_queueFamilyIndices)
{
	std::swap(m_logicalDevice, r_value.m_logicalDevice);
	std::swap(m_physicalDevice, r_value.m_physicalDevice);
}

Device & Device::operator=(Device && r_value)
{
	m_queueFamilyIndices = r_value.m_queueFamilyIndices;

	std::swap(m_logicalDevice, r_value.m_logicalDevice);
	std::swap(m_physicalDevice, r_value.m_physicalDevice);

	return *this;
}

VkDevice & Device::getLogicalDevice()
{
	return m_logicalDevice;
}

VkDevice const & Device::getLogicalDevice() const
{
	return m_logicalDevice;
}

VkPhysicalDevice & Device::getPhysicalDevice()
{
	return m_physicalDevice;
}

VkPhysicalDevice const & Device::getPhysicalDevice() const
{
	return m_physicalDevice;
}

std::vector<uint32_t> & Device::getQueueFamilyIndices()
{
	return m_queueFamilyIndices;
}

std::vector<uint32_t> const & Device::getQueueFamilyIndices() const
{
	return m_queueFamilyIndices;
}

Device::~Device()
{
	vkDestroyDevice(m_logicalDevice, nullptr);
}