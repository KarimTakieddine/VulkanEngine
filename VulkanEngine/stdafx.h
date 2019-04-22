// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <exception>
#include <iostream>
#include <fstream>
#include <vector>

#include "vulkan\vulkan.hpp"

#include "GLFW\glfw3.h"

typedef std::vector<char const *> CStringList;
typedef std::vector<VkPhysicalDevice> PhysicalDeviceList;
typedef std::vector<VkSurfaceFormatKHR> SurfaceFormatList;