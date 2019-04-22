#pragma once

#include "stdafx.h"
#include "InstanceContext.h"
#include "WindowContext.h"
#include "DeviceContext.h"

struct EngineContext
{
	InstanceContext m_instanceContext;

	WindowContext m_windowContext;

	DeviceContext m_deviceContext;

	EngineContext()
	:
	m_instanceContext(),
	m_windowContext(),
	m_deviceContext()
	{

	}

	EngineContext
	(
		DeviceContext const & deviceContext,
		WindowContext const & windowContext,
		InstanceContext const & instanceContext
	)
	:
	m_instanceContext(instanceContext),
	m_windowContext(windowContext),
	m_deviceContext(deviceContext)
	{

	}

	EngineContext(EngineContext const & other)
	:
	m_instanceContext(other.m_instanceContext),
	m_windowContext(other.m_windowContext),
	m_deviceContext(other.m_deviceContext)
	{

	}
	
	EngineContext & operator=(EngineContext const & other)
	{
		m_instanceContext = other.m_instanceContext;
		m_windowContext = other.m_windowContext;
		m_deviceContext = other.m_deviceContext;

		return *this;
	}
};