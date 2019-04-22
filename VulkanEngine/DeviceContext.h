#pragma once

#include "stdafx.h"
#include "VersionNumber.h"

struct DeviceContext
{
	CStringList m_extensionNames;
	VersionNumber m_driverVersion;
	VersionNumber m_apiVersion;
	VkQueueFlags m_requiredQueueFeatures;
	uint32_t m_requiredQueueCount;

	DeviceContext()
	:
	m_extensionNames(),
	m_driverVersion(),
	m_apiVersion(),
	m_requiredQueueFeatures(),
	m_requiredQueueCount()
	{

	}

	DeviceContext
	(
		uint32_t requiredQueueCount,
		VkQueueFlags requiredQueueFeatures,
		VersionNumber const & driverVersion,
		VersionNumber const & apiVersion,
		CStringList const & extensionNames
	)
	:
	m_extensionNames(extensionNames),
	m_driverVersion(driverVersion),
	m_apiVersion(apiVersion),
	m_requiredQueueFeatures(requiredQueueFeatures),
	m_requiredQueueCount(requiredQueueCount)
	{

	}

	DeviceContext(DeviceContext const & other)
	:
	m_extensionNames(other.m_extensionNames),
	m_driverVersion(other.m_driverVersion),
	m_apiVersion(other.m_apiVersion),
	m_requiredQueueFeatures(other.m_requiredQueueFeatures),
	m_requiredQueueCount(other.m_requiredQueueCount)
	{

	}

	DeviceContext & operator=(DeviceContext const & other)
	{
		m_extensionNames = other.m_extensionNames;
		m_driverVersion = other.m_driverVersion;
		m_apiVersion = other.m_apiVersion;
		m_requiredQueueFeatures = other.m_requiredQueueFeatures;
		m_requiredQueueCount = other.m_requiredQueueCount;

		return *this;
	}
};