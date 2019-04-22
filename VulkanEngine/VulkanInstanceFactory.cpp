#include "stdafx.h"
#include "VulkanInstanceFactory.h"

VulkanInstancePtr VulkanInstanceFactory::createVulkanInstance(InstanceContext const & context)
{
	VulkanInstancePtr vulkanInstance(new VulkanInstance());

	VkApplicationInfo applicationInfo;
	
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = context.m_applicationName.c_str();
	applicationInfo.pEngineName = context.m_engineName.c_str();
	applicationInfo.apiVersion = context.m_apiVersion.toInteger();
	applicationInfo.applicationVersion = context.m_applicationVersion.toInteger();
	applicationInfo.engineVersion = context.m_engineVersion.toInteger();
	applicationInfo.pNext = nullptr;

	VkInstanceCreateInfo instanceCreateInfo;

	CStringList const & requiredLayerNames = context.m_layerNames;
	uint32_t requiredExtensionCount = 0;

	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledLayerCount = requiredLayerNames.size();
	instanceCreateInfo.ppEnabledLayerNames = requiredLayerNames.data();
	instanceCreateInfo.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
	instanceCreateInfo.enabledExtensionCount = requiredExtensionCount;
	instanceCreateInfo.pNext = nullptr;

	VkInstance state;

	if (vkCreateInstance(&instanceCreateInfo, nullptr, &state) != VK_SUCCESS)
	{
		return vulkanInstance;
	}

	vulkanInstance->m_state = state;

	return vulkanInstance;
}