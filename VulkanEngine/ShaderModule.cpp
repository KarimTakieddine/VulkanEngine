#include "stdafx.h"
#include "ShaderModule.h"

ShaderModule::ShaderModule(ShaderModule && r_value)
:
m_device(r_value.m_device)
{
	std::swap(m_handle, r_value.m_handle);
}

ShaderModule & ShaderModule::operator=(ShaderModule && r_value)
{
	m_device = r_value.m_device;

	std::swap(m_handle, r_value.m_handle);

	return *this;
}

VkShaderModule ShaderModule::getHandle() const
{
	return m_handle;
}

ShaderModule::~ShaderModule()
{
	vkDestroyShaderModule(m_device->getLogicalDevice(), m_handle, nullptr);
}

ShaderModule::ShaderModule()
:
m_device(),
m_handle(VK_NULL_HANDLE)
{

}