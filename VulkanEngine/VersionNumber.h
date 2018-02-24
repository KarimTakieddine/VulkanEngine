#pragma once

#include "stdafx.h"

class VersionNumber
{
	/*
		The Vulkan version number is used in several places in the API. In each such use, the API major version number, minor version number, and patch version number are packed into a 32-bit integer as follows:

			The major version number is a 10-bit integer packed into bits 31-22.
			The minor version number is a 10-bit integer packed into bits 21-12.
			The patch version number is a 12-bit integer packed into bits 11-0.
	*/

public:

	static VersionNumber fromInteger(uint32_t integer);

	int m_major, m_minor, m_patch;

	VersionNumber();

	VersionNumber
	(
		int major,
		int minor,
		int patch
	);

	VersionNumber(VersionNumber const & other);

	uint32_t toInteger() const;

	bool operator==(VersionNumber const & other);
};