#include "stdafx.h"
#include "Surface.h"

Surface::Surface()
:
m_handle(VK_NULL_HANDLE)
{

}

VkSurfaceKHR Surface::getHandle() const
{
	return m_handle;
}

Surface::~Surface()
{
	vkDestroySurfaceKHR(m_vulkanInstance->getState(), m_handle, nullptr);
}