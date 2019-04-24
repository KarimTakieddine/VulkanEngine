#pragma once

#include "stdafx.h"
#include "File.h"
#include "ShaderModule.h"

typedef std::shared_ptr<ShaderModule> ShaderModulePtr;

class ShaderModuleFactory
{
public:

	static ShaderModulePtr createShaderModule(DevicePtr const & device, char const * file);
};