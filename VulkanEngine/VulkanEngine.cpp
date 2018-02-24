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

	char const * enabledLayerNames[3] =
	{
		"VK_LAYER_LUNARG_api_dump",
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_parameter_validation"
	};

	uint32_t requiredExtensionCount = 0;

	assert(glfwInit() == VK_TRUE);

	instanceCreateInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.flags					= 0;
	instanceCreateInfo.pApplicationInfo			= &applicationInfo;
	instanceCreateInfo.enabledLayerCount		= 3;
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

	char const * deviceExtensionNames[1] =
	{
		"VK_KHR_swapchain"
	};

	VkDeviceCreateInfo logicalDeviceCreateInfo;
	logicalDeviceCreateInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	logicalDeviceCreateInfo.flags					= 0;
	logicalDeviceCreateInfo.queueCreateInfoCount	= 1;
	logicalDeviceCreateInfo.pEnabledFeatures		= &enabledDeviceFeatures;
	logicalDeviceCreateInfo.pQueueCreateInfos		= &deviceQueueCreateInfo;
	logicalDeviceCreateInfo.enabledLayerCount		= 0;
	logicalDeviceCreateInfo.ppEnabledLayerNames		= nullptr;
	logicalDeviceCreateInfo.enabledExtensionCount	= 1;
	logicalDeviceCreateInfo.ppEnabledExtensionNames = deviceExtensionNames;
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

	VkSurfaceCapabilitiesKHR deviceSurfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &deviceSurfaceCapabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
	assert(formatCount > 0);
	VkSurfaceFormatKHR * deviceSupportedSurfaceFormats = new VkSurfaceFormatKHR[formatCount];
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, deviceSupportedSurfaceFormats);
	int selectedSurfaceFormatIndex = -1;

	for (uint32_t i = 0; i < formatCount; ++i)
	{
		VkSurfaceFormatKHR const & surfaceFormat = deviceSupportedSurfaceFormats[i];

		if (surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR && surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
		{
			selectedSurfaceFormatIndex = i;
			break;
		}
	}

	assert(selectedSurfaceFormatIndex != -1);

	VkSurfaceFormatKHR const & selectedSurfaceFormat = deviceSupportedSurfaceFormats[selectedSurfaceFormatIndex];

	VkSwapchainKHR deviceSwapChain;
	VkSwapchainCreateInfoKHR deviceSwapChainCreateInfo;
	deviceSwapChainCreateInfo.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	deviceSwapChainCreateInfo.flags					= 0;
	deviceSwapChainCreateInfo.clipped				= VK_TRUE;
	deviceSwapChainCreateInfo.surface				= surface;
	deviceSwapChainCreateInfo.minImageCount			= deviceSurfaceCapabilities.minImageCount;
	deviceSwapChainCreateInfo.imageExtent			= deviceSurfaceCapabilities.currentExtent;
	deviceSwapChainCreateInfo.imageFormat			= selectedSurfaceFormat.format;
	deviceSwapChainCreateInfo.imageColorSpace		= selectedSurfaceFormat.colorSpace;
	deviceSwapChainCreateInfo.imageArrayLayers		= 1;
	deviceSwapChainCreateInfo.imageUsage			= deviceSurfaceCapabilities.supportedUsageFlags;
	deviceSwapChainCreateInfo.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
	deviceSwapChainCreateInfo.queueFamilyIndexCount = 1;
	deviceSwapChainCreateInfo.pQueueFamilyIndices	= reinterpret_cast<uint32_t const *>(&deviceQueueFamilyIndex);
	deviceSwapChainCreateInfo.compositeAlpha		= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	deviceSwapChainCreateInfo.presentMode			= VK_PRESENT_MODE_FIFO_KHR;
	deviceSwapChainCreateInfo.preTransform			= deviceSurfaceCapabilities.currentTransform;
	deviceSwapChainCreateInfo.oldSwapchain			= VK_NULL_HANDLE;
	deviceSwapChainCreateInfo.pNext					= nullptr;

	assert(vkCreateSwapchainKHR(logicalDevice, &deviceSwapChainCreateInfo, nullptr, &deviceSwapChain) == VK_SUCCESS);

	uint32_t swapChainImageCount = 0;
	vkGetSwapchainImagesKHR(logicalDevice, deviceSwapChain, &swapChainImageCount, nullptr);
	assert(swapChainImageCount >= deviceSurfaceCapabilities.minImageCount);
	VkImage * swapChainImages = new VkImage[swapChainImageCount];
	vkGetSwapchainImagesKHR(logicalDevice, deviceSwapChain, &swapChainImageCount, swapChainImages);

	VkImageView * swapChainImageViews = new VkImageView[swapChainImageCount]();

	for (uint32_t i = 0; i < swapChainImageCount; ++i)
	{
		VkComponentMapping imageComponentMapping;
		imageComponentMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageComponentMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageComponentMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageComponentMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		VkImageSubresourceRange imageSubResourceRange;
		imageSubResourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
		imageSubResourceRange.baseMipLevel		= 0;
		imageSubResourceRange.levelCount		= 1;
		imageSubResourceRange.baseArrayLayer	= 0;
		imageSubResourceRange.layerCount		= 1;

		VkImageViewCreateInfo imageViewCreateInfo;
		imageViewCreateInfo.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.flags				= 0;
		imageViewCreateInfo.image				= swapChainImages[i];
		imageViewCreateInfo.format				= selectedSurfaceFormat.format;
		imageViewCreateInfo.viewType			= VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.components			= imageComponentMapping;
		imageViewCreateInfo.subresourceRange	= imageSubResourceRange;
		imageViewCreateInfo.pNext				= nullptr;

		assert(vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &swapChainImageViews[i]) == VK_SUCCESS);
	}

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

	delete[](swapChainImageViews);
	delete[](swapChainImages);
	vkDestroySwapchainKHR(logicalDevice, deviceSwapChain, nullptr);
	delete[](deviceSupportedSurfaceFormats);
	vkDestroyDevice(logicalDevice, nullptr);
	delete[](physicalDevices);
	vkDestroyInstance(vulkanInstance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}