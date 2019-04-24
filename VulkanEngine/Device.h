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
		VkPhysicalDevice const & physicalDevice,
		VkQueueFamilyProperties const & requiredQueueFamilyProperties,
		QueueFamilyPropertiesList const & queueFamilyPropertiesList
	);

	Device(Device const &) = delete;
	Device & operator=(Device const &) = delete;

	Device(Device && r_value);
	Device & operator=(Device && r_value);

	VkDevice & getLogicalDevice();
	VkDevice const & getLogicalDevice() const;

	VkPhysicalDevice & getPhysicalDevice();
	VkPhysicalDevice const & getPhysicalDevice() const;

	std::vector<uint32_t> & getQueueFamilyIndices();
	std::vector<uint32_t> const & getQueueFamilyIndices() const;

	~Device();

private:

	Device();

	VkDevice m_logicalDevice;
	VkPhysicalDevice m_physicalDevice;
	std::vector<uint32_t> m_queueFamilyIndices;
};