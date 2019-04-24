#pragma once

#include "stdafx.h"

struct SwapChainContext
{
	VkSurfaceFormatKHR m_surfaceFormat;
	VkPresentModeKHR m_presentMode;
	VkSharingMode m_sharingMode;
	VkBool32 m_clipped;

	SwapChainContext()
	:
	m_surfaceFormat(),
	m_presentMode(),
	m_sharingMode(),
	m_clipped()
	{
	
	}

	SwapChainContext
	(
		VkBool32 clipped,
		VkPresentModeKHR presentMode,
		VkSharingMode sharingMode,
		VkSurfaceFormatKHR surfaceFormat
	)
	:
	m_surfaceFormat(surfaceFormat),
	m_presentMode(presentMode),
	m_sharingMode(sharingMode),
	m_clipped(clipped)
	{

	}

	SwapChainContext(SwapChainContext const & other)
	:
	m_surfaceFormat(other.m_surfaceFormat),
	m_presentMode(other.m_presentMode),
	m_sharingMode(other.m_sharingMode),
	m_clipped(other.m_clipped)
	{

	}

	SwapChainContext & operator=(SwapChainContext const & other)
	{
		m_surfaceFormat = other.m_surfaceFormat;
		m_presentMode = other.m_presentMode;
		m_sharingMode = other.m_sharingMode;
		m_clipped = other.m_clipped;

		return *this;
	}
};