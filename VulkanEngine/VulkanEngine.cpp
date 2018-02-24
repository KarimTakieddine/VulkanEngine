// VulkanEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VersionNumber.h"

int main()
{
	VersionNumber apiVersionNumber(1, 0, 68);
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

	vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanInstance);

    return 0;
}