#include "stdafx.h"
#include "DeviceFactory.h"

DevicePtr DeviceFactory::createDevice(VulkanInstancePtr const & vulkanInstance, DeviceContext const & deviceContext)
{
	DevicePtr device(new Device());

	VkInstance const & instanceState = vulkanInstance->getState();
	uint32_t physicalDeviceCount = 0;

	vkEnumeratePhysicalDevices(instanceState, &physicalDeviceCount, nullptr);

	if (physicalDeviceCount == 0)
	{
		return device;
	}

	PhysicalDeviceList physicalDeviceList(physicalDeviceCount);

	if (vkEnumeratePhysicalDevices(instanceState, &physicalDeviceCount, physicalDeviceList.data()) != VK_SUCCESS)
	{
		return device;
	}

	int physicalDeviceIndex = Device::getPhysicalDeviceIndex(deviceContext.m_apiVersion, deviceContext.m_driverVersion, physicalDeviceList);

	if (physicalDeviceIndex == -1)
	{
		return device;
	}

	VkPhysicalDevice const & physicalDevice = physicalDeviceList[physicalDeviceIndex];

	const uint32_t requiredQueueCount = deviceContext.m_requiredQueueCount;

	int queueFamilyIndex = Device::getQueueFamilyIndex(physicalDevice, requiredQueueCount, deviceContext.m_requiredQueueFeatures);

	if (queueFamilyIndex == -1)
	{
		return device;
	}

	std::vector<float> deviceQueuePriorities(requiredQueueCount, 1.0f);

	VkDeviceQueueCreateInfo deviceQueueCreateInfo;

	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.flags = 0;
	deviceQueueCreateInfo.queueCount = requiredQueueCount;
	deviceQueueCreateInfo.pQueuePriorities = deviceQueuePriorities.data();
	deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
	deviceQueueCreateInfo.pNext = nullptr;

	VkPhysicalDeviceFeatures enabledDeviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &enabledDeviceFeatures);

	CStringList const & deviceExtensionNames = deviceContext.m_extensionNames;

	VkDeviceCreateInfo deviceCreateInfo;

	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.enabledExtensionCount = deviceExtensionNames.size();
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensionNames.data();
	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = nullptr;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	deviceCreateInfo.pEnabledFeatures = &enabledDeviceFeatures;
	deviceCreateInfo.pNext = nullptr;

	VkDevice logicalDevice;

	if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS)
	{
		return device;
	}

	device->m_logicalDevice = logicalDevice;
	device->m_physicalDevice = physicalDevice;
	device->m_queueFamilyIndex = queueFamilyIndex;

	return device;
}
