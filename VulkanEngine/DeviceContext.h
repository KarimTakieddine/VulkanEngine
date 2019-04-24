#pragma once

#include "stdafx.h"
#include "VersionNumber.h"

struct DeviceContext
{
	CStringList m_extensionNames;
	VersionNumber m_driverVersion;
	VersionNumber m_apiVersion;
	QueueFamilyPropertiesList m_queueFamilyProperties;

	DeviceContext()
	:
	m_extensionNames(),
	m_driverVersion(),
	m_apiVersion(),
	m_queueFamilyProperties()
	{

	}

	DeviceContext
	(
		QueueFamilyPropertiesList queueFamilyProperties,
		VersionNumber const & driverVersion,
		VersionNumber const & apiVersion,
		CStringList const & extensionNames
	)
	:
	m_extensionNames(extensionNames),
	m_driverVersion(driverVersion),
	m_apiVersion(apiVersion),
	m_queueFamilyProperties(queueFamilyProperties)
	{

	}

	DeviceContext(DeviceContext const & other)
	:
	m_extensionNames(other.m_extensionNames),
	m_driverVersion(other.m_driverVersion),
	m_apiVersion(other.m_apiVersion),
	m_queueFamilyProperties(other.m_queueFamilyProperties)
	{

	}

	DeviceContext & operator=(DeviceContext const & other)
	{
		m_extensionNames = other.m_extensionNames;
		m_driverVersion = other.m_driverVersion;
		m_apiVersion = other.m_apiVersion;
		m_queueFamilyProperties = other.m_queueFamilyProperties;

		return *this;
	}
};