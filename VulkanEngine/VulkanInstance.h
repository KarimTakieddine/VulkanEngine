#pragma once

#include "stdafx.h"

class VulkanInstance
{
	friend class VulkanInstanceFactory;

public:

	VulkanInstance(VulkanInstance const &) = delete;
	VulkanInstance & operator=(VulkanInstance const &) = delete;

	VulkanInstance(VulkanInstance && r_value);
	VulkanInstance & operator=(VulkanInstance && r_value);

	VkInstance getState();
	VkInstance const & getState() const;

	~VulkanInstance();

private:

	VulkanInstance();

	VkInstance m_state;
};