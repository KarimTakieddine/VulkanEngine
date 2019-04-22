#pragma once

#include "stdafx.h"

struct WindowContext
{
	std::string m_name;
	int m_width;
	int m_height;

	WindowContext()
	:
	m_name("Default Window"),
	m_width(640),
	m_height(480)
	{

	}

	WindowContext
	(
		int width,
		int height,
		std::string const & name
	)
	:
	m_name(name),
	m_width(width),
	m_height(height)
	{

	}

	WindowContext(WindowContext const & other)
	:
	m_name(other.m_name),
	m_width(other.m_width),
	m_height(other.m_height)
	{
		
	}

	WindowContext & operator=(WindowContext const & other)
	{
		m_name = other.m_name;
		m_width = other.m_width;
		m_height = other.m_height;

		return *this;
	}
};