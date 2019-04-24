#pragma once

#include "stdafx.h"
#include "Buffer.h"
#include "BufferContext.h"
#include "DeviceFactory.h"

typedef std::shared_ptr<Buffer> BufferPtr;

class BufferFactory
{
public:

	static BufferPtr createBuffer(DevicePtr const & device, BufferContext const & context, VkDeviceSize byteSize);
};