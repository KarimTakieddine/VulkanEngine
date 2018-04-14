#pragma once
#include "stdafx.h"

/*
	Vulkan-enabled devices execute work submitted to queues. Each device
	will have one or more queues, and each of those queues will belong
	to one or more of the device's queue families.

	Queue families are segregated by functionality, expressed in the form
	of a combination of flag bits. Multiple queues with identical functionality
	may exist within a given family.
*/

typedef std::vector<VkQueueFamilyProperties> QueueFamilyPropertiesList;

class DeviceQueue
{
public:

	static int getFamilyIndexMatching
	(
		uint32_t requiredCount,
		VkQueueFlags requiredFlags,
		VkPhysicalDevice physicalDevice
	);

	DeviceQueue();

	DeviceQueue
	(
		uint32_t familyIndex,
		uint32_t queueIndex,
		VkDevice logicalDevice
	);

	void initialize
	(
		uint32_t familyIndex,
		uint32_t queueIndex,
		VkDevice logicalDevice
	);

	bool isValid() const;

	VkQueue & handle();

private:

	DeviceQueue::DeviceQueue(DeviceQueue const & other);

	VkQueue m_handle;
};