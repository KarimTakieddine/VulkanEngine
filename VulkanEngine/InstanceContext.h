#pragma once

#include "stdafx.h"
#include "VersionNumber.h"

struct InstanceContext
{
	CStringList m_layerNames;
	std::string m_applicationName;
	std::string m_engineName;
	VersionNumber m_apiVersion;
	VersionNumber m_applicationVersion;
	VersionNumber m_engineVersion;

	InstanceContext()
	:
	m_layerNames(),
	m_applicationName(),
	m_engineName(),
	m_apiVersion(),
	m_applicationVersion(),
	m_engineVersion()
	{

	}

	InstanceContext
	(
		VersionNumber const & apiVersion,
		VersionNumber const & applicationVersion,
		VersionNumber const & engineVersion,
		std::string const & applicationName,
		std::string const & engineName,
		CStringList const & layerNames
	)
	:
	m_layerNames(layerNames),
	m_applicationName(applicationName),
	m_engineName(engineName),
	m_apiVersion(apiVersion),
	m_applicationVersion(applicationVersion),
	m_engineVersion(engineVersion)
	{

	}

	InstanceContext(InstanceContext const & other)
	:
	m_layerNames(other.m_layerNames),
	m_applicationName(other.m_applicationName),
	m_engineName(other.m_engineName),
	m_apiVersion(other.m_apiVersion),
	m_applicationVersion(other.m_applicationVersion),
	m_engineVersion(other.m_engineVersion)
	{

	}

	InstanceContext & operator=(InstanceContext const & other)
	{
		m_layerNames = other.m_layerNames;
		m_applicationName = other.m_applicationName;
		m_engineName = other.m_engineName;
		m_apiVersion = other.m_apiVersion;
		m_applicationVersion = other.m_applicationVersion;
		m_engineVersion = other.m_engineVersion;

		return *this;
	}
};