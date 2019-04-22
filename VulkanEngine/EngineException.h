#pragma once

#include "stdafx.h"

class EngineException : public std::exception
{
	std::string m_message;

	VkResult m_errorCode;

public:

	EngineException();

	EngineException
	(
		VkResult errorCode,
		std::string const & message
	);

	EngineException(EngineException const & other);

	EngineException & operator=(EngineException const & other);

	virtual char const * what() const;
};