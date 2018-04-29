#include "stdafx.h"
#include "VersionNumber.h"

VersionNumber VersionNumber::fromInteger(uint32_t integer)
{
	return VersionNumber(
		(integer & 0xFFC00000)	>> 22,
		(integer & 0x3FF000)	>> 12,
		(integer & 0xFFF)
	);
}

VersionNumber::VersionNumber()
:
m_major(),
m_minor(),
m_patch()
{ }

VersionNumber::VersionNumber
(
	int major,
	int minor,
	int patch
)
:
m_major(major),
m_minor(minor),
m_patch(patch)
{ }

VersionNumber::VersionNumber
(
	VersionNumber const & other
)
:
m_major(other.m_major),
m_minor(other.m_minor),
m_patch(other.m_patch)
{ }

uint32_t VersionNumber::toInteger() const
{
	return (m_major << 22) | (m_minor << 12) | m_patch;
}

bool VersionNumber::operator==(VersionNumber const & other)
{
	return (m_major == other.m_major) && (m_minor == other.m_minor) && (m_patch == other.m_patch);
}

bool VersionNumber::operator!=(VersionNumber const & other)
{
	return !(*this == other);
}

bool VersionNumber::operator>(VersionNumber const & other)
{
	return toInteger() > other.toInteger();
}

bool VersionNumber::operator>=(VersionNumber const & other)
{
	return toInteger() >= other.toInteger();
}

bool VersionNumber::operator<(VersionNumber const & other)
{
	return toInteger() < other.toInteger();
}

bool VersionNumber::operator<=(VersionNumber const & other)
{
	return toInteger() <= other.toInteger();
}
