#include "stdafx.h"
#include "ShaderModuleFactory.h"

ShaderModulePtr ShaderModuleFactory::createShaderModule(DevicePtr const & device, char const * file)
{
	ShaderModulePtr shaderModule(new ShaderModule());

	shaderModule->m_device = device;

	CharacterBuffer shaderBuffer;

	if (!File::read(file, &shaderBuffer))
	{
		return shaderModule;
	}

	VkShaderModuleCreateInfo shaderModuleCreateInfo;

	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.flags = 0;
	shaderModuleCreateInfo.codeSize = shaderBuffer.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<uint32_t const *>(shaderBuffer.data());
	shaderModuleCreateInfo.pNext = nullptr;

	VkShaderModule handle;

	if (vkCreateShaderModule(device->getLogicalDevice(), &shaderModuleCreateInfo, nullptr, &handle) != VK_SUCCESS)
	{
		return shaderModule;
	}

	shaderModule->m_handle = handle;

	return shaderModule;
}