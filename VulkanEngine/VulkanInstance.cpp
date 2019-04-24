#include "stdafx.h"
#include "VulkanInstance.h"

VulkanInstance::VulkanInstance(VulkanInstance && r_value)
{
	std::swap(m_state, r_value.m_state);
}

VulkanInstance & VulkanInstance::operator=(VulkanInstance && r_value)
{
	std::swap(m_state, r_value.m_state);

	return *this;
}

VkInstance & VulkanInstance::getState()
{
	return m_state;
}

VkInstance const & VulkanInstance::getState() const
{
	return m_state;
}

VulkanInstance::~VulkanInstance()
{
	vkDestroyInstance(m_state, nullptr);
	glfwTerminate();
}

VulkanInstance::VulkanInstance()
:
m_state(VK_NULL_HANDLE)
{
	glfwInit();
}