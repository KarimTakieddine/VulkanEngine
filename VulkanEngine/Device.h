#pragma once

#include "stdafx.h"
#include "VersionNumber.h"

class Device
{

	friend class DeviceFactory;

public:

	static int getPhysicalDeviceIndex
	(
		VersionNumber const & minimumApiVersion,
		VersionNumber const & minimumDriverVersion,
		PhysicalDeviceList const & physicalDevices
	);

	static int getQueueFamilyIndex
	(
		VkPhysicalDevice physicalDevice,
		uint32_t requiredCount,
		VkQueueFlags requiredFlags
	);

	Device(Device const &) = delete;
	Device & operator=(Device const &) = delete;

	Device(Device && r_value);
	Device & operator=(Device && r_value);

	VkDevice getLogicalDevice();
	VkDevice const & getLogicalDevice() const;

	VkPhysicalDevice getPhysicalDevice();
	VkPhysicalDevice const & getPhysicalDevice() const;

	~Device();

private:

	Device();

	VkDevice m_logicalDevice;
	VkPhysicalDevice m_physicalDevice;
	uint32_t m_queueFamilyIndex;
};