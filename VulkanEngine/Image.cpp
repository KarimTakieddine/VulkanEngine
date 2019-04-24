#include "stdafx.h"
#include "Image.h"

Image::Image(Image && r_value)
:
m_device(r_value.m_device)
{
	std::swap(m_handle, r_value.m_handle);
}

Image & Image::operator=(Image && r_value)
{
	m_device = r_value.m_device;

	std::swap(m_handle, r_value.m_handle);

	return *this;
}

VkImage Image::getHandle() const
{
	return m_handle;
}

Image::~Image()
{
	
}

Image::Image()
:
m_device(),
m_handle(VK_NULL_HANDLE)
{

}