#pragma once
#include "stdafx.h"
#include "PhysicalDeviceRequirements.h"

typedef std::vector<VkPhysicalDevice> PhysicalDeviceList;

class PhysicalDeviceLoader
{
public:

	PhysicalDeviceLoader();

	PhysicalDeviceLoader(VkInstance const * applicationInstance);

	PhysicalDeviceLoader(PhysicalDeviceLoader const & other);

	VkResult loadDevices();

	uint32_t getDeviceCount() const;

	PhysicalDeviceList & getData();

	VkPhysicalDevice & getDeviceAt(uint32_t index);

	int getIndexMatching(PhysicalDeviceRequirements const & requirements);

private:

	PhysicalDeviceList m_data;
	VkInstance const * m_applicationInstance;
};