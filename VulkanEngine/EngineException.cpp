#include "stdafx.h"
#include "EngineException.h"

EngineException::EngineException()
:
std::exception(),
m_message(),
m_errorCode()
{

}

EngineException::EngineException(EngineException const & other)
:
std::exception(other),
m_message(other.m_message),
m_errorCode(other.m_errorCode)
{

}

EngineException::EngineException(VkResult errorCode, std::string const & message)
:
std::exception(),
m_message(message),
m_errorCode(errorCode)
{
	
}

EngineException & EngineException::operator=(EngineException const & other)
{
	std::exception::operator=(other);

	m_message = other.m_message;
	m_errorCode = other.m_errorCode;

	return *this;
}

char const * EngineException::what() const
{
	return m_message.c_str();
}