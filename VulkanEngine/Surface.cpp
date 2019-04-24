#include "stdafx.h"
#include "Surface.h"

Surface::Surface()
:
m_vulkanInstance(),
m_handle(VK_NULL_HANDLE)
{

}

Surface::Surface(Surface && r_value)
:
m_vulkanInstance(r_value.m_vulkanInstance)
{
	std::swap(m_handle, r_value.m_handle);
}

Surface & Surface::operator=(Surface && r_value)
{
	m_vulkanInstance = r_value.m_vulkanInstance;
	
	std::swap(m_handle, r_value.m_handle);

	return *this;
}

VkSurfaceKHR Surface::getHandle() const
{
	return m_handle;
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(m_vulkanInstance->getState(), m_handle, nullptr);
}