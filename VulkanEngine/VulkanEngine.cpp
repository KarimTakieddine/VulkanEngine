// VulkanEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VersionNumber.h"

int main()
{
	VersionNumber apiVersionNumber(1, 0, 65);
	VersionNumber applicationVersionNumber(1, 0, 0);
	VersionNumber engineVersionNumber(1, 0, 0);

	VkApplicationInfo applicationInfo;
	applicationInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.apiVersion			= apiVersionNumber.toInteger();
	applicationInfo.applicationVersion	= applicationVersionNumber.toInteger();
	applicationInfo.engineVersion		= engineVersionNumber.toInteger();
	applicationInfo.pApplicationName	= "VulkanEngine";
	applicationInfo.pEngineName			= "VulkanEngine";
	applicationInfo.pNext				= nullptr;

	VkInstance vulkanInstance;
	VkInstanceCreateInfo instanceCreateInfo;

	char const * enabledLayerNames[1] =
	{
		"VK_LAYER_LUNARG_api_dump"
	};

	instanceCreateInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.flags					= 0;
	instanceCreateInfo.pNext					= nullptr;
	instanceCreateInfo.pApplicationInfo			= &applicationInfo;
	instanceCreateInfo.enabledLayerCount		= 1;
	instanceCreateInfo.ppEnabledLayerNames		= enabledLayerNames;
	instanceCreateInfo.enabledExtensionCount	= 0;
	instanceCreateInfo.ppEnabledExtensionNames	= nullptr;

	assert(vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanInstance) == VK_SUCCESS);

	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(vulkanInstance, &physicalDeviceCount, nullptr);

	assert(physicalDeviceCount > 0);

	VkPhysicalDevice * physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
	assert(vkEnumeratePhysicalDevices(vulkanInstance, &physicalDeviceCount, physicalDevices) == VK_SUCCESS);
	int supportedDeviceIndex = -1;

	for (uint32_t i = 0; i < physicalDeviceCount; ++i)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
		VersionNumber deviceAPIVersionNumber = VersionNumber::fromInteger(deviceProperties.apiVersion);

		if (deviceAPIVersionNumber == apiVersionNumber)
		{
			supportedDeviceIndex = i;
			break;
		}
	}

	assert(supportedDeviceIndex != -1);

	delete[](physicalDevices);
	vkDestroyInstance(vulkanInstance, nullptr);

    return 0;
}