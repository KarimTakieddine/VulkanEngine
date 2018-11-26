#pragma once

#include "stdafx.h"

class EngineApplication
{

public:

	static EngineApplication & getInstance()
	{
		/*
			From the ISO/IEC 2012 standard, section 6.7.4:

			The zero-initialization of all block-scope variables with static storage
			duration or thread storage duration is performed before any other initialization
			takes place.

			If control enters the declaration concurrently while the variable is being
			initialized, the concurrent execution shall wait for completion of the
			initialization.
		*/

		static EngineApplication instance;

		return instance;
	}

	~EngineApplication()
	{
		vkDestroyInstance(m_vulkanInstance, nullptr);
	}

private:

	EngineApplication()
	:
	m_vulkanInstance(VK_NULL_HANDLE)
	{

	}

	VkInstance m_vulkanInstance;
};