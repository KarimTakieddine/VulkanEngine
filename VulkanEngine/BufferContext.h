#pragma once

#include "stdafx.h"

struct BufferContext
{
	VkMemoryPropertyFlags m_memoryProperties;
	VkBufferUsageFlagBits m_usage;
	VkSharingMode m_sharingMode;

	BufferContext()
	:
	m_memoryProperties(),
	m_usage(),
	m_sharingMode()
	{

	}

	BufferContext
	(
		VkMemoryPropertyFlags memoryProperties,
		VkBufferUsageFlagBits usage,
		VkSharingMode sharingMode
	)
	:
	m_memoryProperties(memoryProperties),
	m_usage(usage),
	m_sharingMode(sharingMode)
	{

	}

	BufferContext(BufferContext const & other)
	:
	m_memoryProperties(other.m_memoryProperties),
	m_usage(other.m_usage),
	m_sharingMode(other.m_sharingMode)
	{

	}

	BufferContext & operator=(BufferContext const & other)
	{
		m_memoryProperties = other.m_memoryProperties;
		m_usage = other.m_usage;
		m_sharingMode = other.m_sharingMode;

		return *this;
	}
};