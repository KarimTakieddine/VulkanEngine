#include "stdafx.h"
#include "ImageView.h"

ImageView::ImageView(ImageView && r_value)
:
m_device(r_value.m_device)
{
	std::swap(m_handle, r_value.m_handle);
}

ImageView & ImageView::operator=(ImageView && r_value)
{
	m_device = r_value.m_device;

	std::swap(m_handle, r_value.m_handle);

	return *this;
}

VkImageView ImageView::getHandle() const
{
	return m_handle;
}

ImageView::~ImageView()
{
	vkDestroyImageView(m_device->getLogicalDevice(), m_handle, nullptr);
}

ImageView::ImageView()
:
m_device(),
m_handle(VK_NULL_HANDLE)
{

}