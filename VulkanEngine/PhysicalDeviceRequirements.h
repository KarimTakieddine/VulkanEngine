#pragma once
#include "VersionNumber.h"

/*
	Will be extended to allow for validation
	of further application-specific requirements.
*/

struct PhysicalDeviceRequirements
{
	VersionNumber apiVersionNumber, driverVersionNumber;
};