#pragma once

#include "stdafx.h"
#include "DeviceFactory.h"

class ShaderModule
{
	friend class ShaderModuleFactory;

public:

	ShaderModule(ShaderModule const &) = delete;
	ShaderModule & operator=(ShaderModule const &) = delete;

	ShaderModule(ShaderModule && r_value);
	ShaderModule & operator=(ShaderModule && r_value);

	VkShaderModule getHandle() const;

	~ShaderModule();

private:

	ShaderModule();

	DevicePtr m_device;
	VkShaderModule m_handle;
};