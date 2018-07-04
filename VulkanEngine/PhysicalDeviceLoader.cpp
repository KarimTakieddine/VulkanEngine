#include "stdafx.h"
#include "PhysicalDeviceLoader.h"

PhysicalDeviceLoader::PhysicalDeviceLoader()
:
m_data(),
m_applicationInstance(VK_NULL_HANDLE)
{}

PhysicalDeviceLoader::PhysicalDeviceLoader
(
	VkInstance applicationInstance
)
:
m_data(),
m_applicationInstance(applicationInstance)
{}

PhysicalDeviceLoader::PhysicalDeviceLoader
(
	PhysicalDeviceLoader const & other
)
:
m_data(other.m_data),
m_applicationInstance(other.m_applicationInstance)
{}

VkResult PhysicalDeviceLoader::loadDevices()
{
	if (m_applicationInstance == VK_NULL_HANDLE)
	{
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	uint32_t deviceCount		= 0;
	VkResult enumerationStatus	= vkEnumeratePhysicalDevices(m_applicationInstance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		return enumerationStatus;
	}

	m_data.resize(deviceCount);

	return vkEnumeratePhysicalDevices(m_applicationInstance, &deviceCount, &m_data[0]);
}

uint32_t PhysicalDeviceLoader::getDeviceCount() const
{
	return m_data.size();
}

PhysicalDeviceList & PhysicalDeviceLoader::getData()
{
	return m_data;
}

VkPhysicalDevice & PhysicalDeviceLoader::getDeviceAt(uint32_t index)
{
	return getData()[index];
}

int PhysicalDeviceLoader::getIndexMatching
(
	PhysicalDeviceRequirements const & requirements
)
{
	for (uint32_t i = 0; i < getDeviceCount(); ++i)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(getDeviceAt(i), &deviceProperties);

		VersionNumber deviceAPIVersionNumber	= VersionNumber::fromInteger(deviceProperties.apiVersion);
		VersionNumber deviceDriverVersionNumber	= VersionNumber::fromInteger(deviceProperties.driverVersion);

		if (
			deviceAPIVersionNumber		>= requirements.apiVersionNumber &&
			deviceDriverVersionNumber	>= requirements.driverVersionNumber
		)
		{
			return i;
		}
	}

	return -1;
}