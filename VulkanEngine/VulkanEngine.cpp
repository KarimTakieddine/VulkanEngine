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

	uint32_t requiredExtensionCount = 0;

	assert(glfwInit() == GL_TRUE);

	instanceCreateInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.flags					= 0;
	instanceCreateInfo.pApplicationInfo			= &applicationInfo;
	instanceCreateInfo.enabledLayerCount		= 1;
	instanceCreateInfo.ppEnabledLayerNames		= enabledLayerNames;
	instanceCreateInfo.ppEnabledExtensionNames	= glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
	instanceCreateInfo.enabledExtensionCount	= requiredExtensionCount;
	instanceCreateInfo.pNext					= nullptr;

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

	VkPhysicalDevice & physicalDevice = physicalDevices[supportedDeviceIndex];

	uint32_t deviceQueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyCount, nullptr);
	assert(deviceQueueFamilyCount > 0);
	VkQueueFamilyProperties * deviceQueueFamilyPropertiesList = new VkQueueFamilyProperties[deviceQueueFamilyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyCount, deviceQueueFamilyPropertiesList);

	int deviceQueueFamilyIndex = -1;

	for (uint32_t i = 0; i < deviceQueueFamilyCount; ++i)
	{
		VkQueueFamilyProperties const & deviceQueueFamilyProperties = deviceQueueFamilyPropertiesList[i];
		int requiredFlags											= VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT;

		if ((deviceQueueFamilyProperties.queueFlags & requiredFlags) == requiredFlags)
		{
			deviceQueueFamilyIndex = i;
			break;
		}
	}

	assert(deviceQueueFamilyIndex != -1);

	VkDeviceQueueCreateInfo deviceQueueCreateInfo;

	float queuePriorities[1]				= { 1.0f };
	deviceQueueCreateInfo.sType				= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.flags				= 0;
	deviceQueueCreateInfo.queueCount		= 1;
	deviceQueueCreateInfo.pQueuePriorities	= queuePriorities;
	deviceQueueCreateInfo.queueFamilyIndex	= deviceQueueFamilyIndex;
	deviceQueueCreateInfo.pNext				= nullptr;

	VkPhysicalDeviceFeatures enabledDeviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &enabledDeviceFeatures);

	VkDeviceCreateInfo logicalDeviceCreateInfo;
	
	logicalDeviceCreateInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	logicalDeviceCreateInfo.flags					= 0;
	logicalDeviceCreateInfo.queueCreateInfoCount	= 1;
	logicalDeviceCreateInfo.pEnabledFeatures		= &enabledDeviceFeatures;
	logicalDeviceCreateInfo.pQueueCreateInfos		= &deviceQueueCreateInfo;
	logicalDeviceCreateInfo.enabledLayerCount		= 0;
	logicalDeviceCreateInfo.ppEnabledLayerNames		= nullptr;
	logicalDeviceCreateInfo.enabledExtensionCount	= 0;
	logicalDeviceCreateInfo.ppEnabledExtensionNames = nullptr;
	logicalDeviceCreateInfo.pNext					= nullptr;

	VkDevice logicalDevice;
	assert(vkCreateDevice(physicalDevice, &logicalDeviceCreateInfo, nullptr, &logicalDevice) == VK_SUCCESS);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow * window = glfwCreateWindow(640, 480, "Vulkan Window", nullptr, nullptr);

	VkSurfaceKHR surface;
	assert(glfwCreateWindowSurface(vulkanInstance, window, nullptr, &surface) == VK_SUCCESS);

	VkBool32 deviceSupportsPresentation = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, deviceQueueFamilyIndex, surface, &deviceSupportsPresentation);
	assert(deviceSupportsPresentation);

	VkQueue presentationQueue;
	vkGetDeviceQueue(logicalDevice, deviceQueueFamilyIndex, 0, &presentationQueue);

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	vkDeviceWaitIdle(logicalDevice);

	vkDestroyDevice(logicalDevice, nullptr);
	delete[](physicalDevices);
	vkDestroyInstance(vulkanInstance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}