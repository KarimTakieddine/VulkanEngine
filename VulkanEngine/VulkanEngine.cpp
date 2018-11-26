// VulkanEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Buffer.h"
#include "DeviceBufferFactory.h"
#include "DeviceQueue.h"
#include "File.h"
#include "PhysicalDeviceLoader.h"
#include "Vertex.h"
#include "MatrixTransform.h"
#include "SceneUniform.h"
#include "Time.h"

int main()
{
	VersionNumber apiVersionNumber(1, 0, 65);
	VersionNumber applicationVersionNumber(1, 0, 0);
	VersionNumber engineVersionNumber(1, 0, 0);

	VkApplicationInfo applicationInfo;
	applicationInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.apiVersion			= apiVersionNumber.toInteger();
	applicationInfo.applicationVersion	= applicationVersionNumber.toInteger();
	applicationInfo.engineVersion		= engineVersionNumber.toInteger();
	applicationInfo.pApplicationName	= "VulkanEngine";
	applicationInfo.pEngineName			= "VulkanEngine";
	applicationInfo.pNext				= nullptr;

	VkInstance vulkanInstance;
	VkInstanceCreateInfo instanceCreateInfo;

	char const * enabledLayerNames[3] =
	{
		"VK_LAYER_LUNARG_api_dump",
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_parameter_validation"
	};

	uint32_t requiredExtensionCount = 0;

	assert(glfwInit() == VK_TRUE);

	instanceCreateInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.flags					= 0;
	instanceCreateInfo.pApplicationInfo			= &applicationInfo;
	instanceCreateInfo.enabledLayerCount		= 3;
	instanceCreateInfo.ppEnabledLayerNames		= enabledLayerNames;
	instanceCreateInfo.ppEnabledExtensionNames	= glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
	instanceCreateInfo.enabledExtensionCount	= requiredExtensionCount;
	instanceCreateInfo.pNext					= nullptr;

	assert(vkCreateInstance(&instanceCreateInfo, nullptr, &vulkanInstance) == VK_SUCCESS);

	PhysicalDeviceRequirements physicalDeviceRequirements;
	physicalDeviceRequirements.apiVersionNumber		= apiVersionNumber;
	physicalDeviceRequirements.driverVersionNumber	= VersionNumber(391, 140, 0);
	PhysicalDeviceLoader physicalDeviceLoader(vulkanInstance);
	assert(physicalDeviceLoader.loadDevices() == VK_SUCCESS);

	int supportedDeviceIndex = physicalDeviceLoader.getIndexMatching(physicalDeviceRequirements);

	assert(supportedDeviceIndex != -1);

	VkPhysicalDevice & physicalDevice = physicalDeviceLoader.getDeviceAt(supportedDeviceIndex);

	int deviceQueueFamilyIndex = DeviceQueue::getFamilyIndexMatching(1, VK_QUEUE_GRAPHICS_BIT|VK_QUEUE_TRANSFER_BIT, physicalDevice);

	assert(deviceQueueFamilyIndex != -1);

	VkDeviceQueueCreateInfo deviceQueueCreateInfo;

	float queuePriorities[1]				= { 1.0f };
	deviceQueueCreateInfo.sType				= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.flags				= 0;
	deviceQueueCreateInfo.queueCount		= 1;
	deviceQueueCreateInfo.pQueuePriorities	= queuePriorities;
	deviceQueueCreateInfo.queueFamilyIndex	= deviceQueueFamilyIndex;
	deviceQueueCreateInfo.pNext				= nullptr;

	VkPhysicalDeviceFeatures enabledDeviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &enabledDeviceFeatures);

	char const * deviceExtensionNames[1] =
	{
		"VK_KHR_swapchain"
	};

	VkDeviceCreateInfo logicalDeviceCreateInfo;
	logicalDeviceCreateInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	logicalDeviceCreateInfo.flags					= 0;
	logicalDeviceCreateInfo.queueCreateInfoCount	= 1;
	logicalDeviceCreateInfo.pEnabledFeatures		= &enabledDeviceFeatures;
	logicalDeviceCreateInfo.pQueueCreateInfos		= &deviceQueueCreateInfo;
	logicalDeviceCreateInfo.enabledLayerCount		= 0;
	logicalDeviceCreateInfo.ppEnabledLayerNames		= nullptr;
	logicalDeviceCreateInfo.enabledExtensionCount	= 1;
	logicalDeviceCreateInfo.ppEnabledExtensionNames = deviceExtensionNames;
	logicalDeviceCreateInfo.pNext					= nullptr;

	VkDevice logicalDevice;
	assert(vkCreateDevice(physicalDevice, &logicalDeviceCreateInfo, nullptr, &logicalDevice) == VK_SUCCESS);

	DeviceQueue presentationQueue(deviceQueueFamilyIndex, 0, logicalDevice);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow * window = glfwCreateWindow(640, 480, "Vulkan Window", nullptr, nullptr);

	VkSurfaceKHR surface;
	assert(glfwCreateWindowSurface(vulkanInstance, window, nullptr, &surface) == VK_SUCCESS);

	VkBool32 deviceSupportsPresentation = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, deviceQueueFamilyIndex, surface, &deviceSupportsPresentation);
	assert(deviceSupportsPresentation);

	VkSurfaceCapabilitiesKHR deviceSurfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &deviceSurfaceCapabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
	assert(formatCount > 0);
	VkSurfaceFormatKHR * deviceSupportedSurfaceFormats = new VkSurfaceFormatKHR[formatCount];
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, deviceSupportedSurfaceFormats);
	int selectedSurfaceFormatIndex = -1;

	for (uint32_t i = 0; i < formatCount; ++i)
	{
		VkSurfaceFormatKHR const & surfaceFormat = deviceSupportedSurfaceFormats[i];

		if (surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR && surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
		{
			selectedSurfaceFormatIndex = i;
			break;
		}
	}

	assert(selectedSurfaceFormatIndex != -1);

	VkSurfaceFormatKHR const & selectedSurfaceFormat = deviceSupportedSurfaceFormats[selectedSurfaceFormatIndex];

	VkSwapchainKHR deviceSwapChain;
	VkSwapchainCreateInfoKHR deviceSwapChainCreateInfo;
	deviceSwapChainCreateInfo.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	deviceSwapChainCreateInfo.flags					= 0;
	deviceSwapChainCreateInfo.clipped				= VK_TRUE;
	deviceSwapChainCreateInfo.surface				= surface;
	deviceSwapChainCreateInfo.minImageCount			= deviceSurfaceCapabilities.minImageCount;
	deviceSwapChainCreateInfo.imageExtent			= deviceSurfaceCapabilities.currentExtent;
	deviceSwapChainCreateInfo.imageFormat			= selectedSurfaceFormat.format;
	deviceSwapChainCreateInfo.imageColorSpace		= selectedSurfaceFormat.colorSpace;
	deviceSwapChainCreateInfo.imageArrayLayers		= 1;
	deviceSwapChainCreateInfo.imageUsage			= deviceSurfaceCapabilities.supportedUsageFlags;
	deviceSwapChainCreateInfo.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
	deviceSwapChainCreateInfo.queueFamilyIndexCount = 1;
	deviceSwapChainCreateInfo.pQueueFamilyIndices	= reinterpret_cast<uint32_t const *>(&deviceQueueFamilyIndex);
	deviceSwapChainCreateInfo.compositeAlpha		= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	deviceSwapChainCreateInfo.presentMode			= VK_PRESENT_MODE_FIFO_KHR;
	deviceSwapChainCreateInfo.preTransform			= deviceSurfaceCapabilities.currentTransform;
	deviceSwapChainCreateInfo.oldSwapchain			= VK_NULL_HANDLE;
	deviceSwapChainCreateInfo.pNext					= nullptr;

	assert(vkCreateSwapchainKHR(logicalDevice, &deviceSwapChainCreateInfo, nullptr, &deviceSwapChain) == VK_SUCCESS);

	uint32_t swapChainImageCount = 0;
	vkGetSwapchainImagesKHR(logicalDevice, deviceSwapChain, &swapChainImageCount, nullptr);
	assert(swapChainImageCount >= deviceSurfaceCapabilities.minImageCount);
	VkImage * swapChainImages = new VkImage[swapChainImageCount];
	vkGetSwapchainImagesKHR(logicalDevice, deviceSwapChain, &swapChainImageCount, swapChainImages);

	VkImageView * swapChainImageViews = new VkImageView[swapChainImageCount]();

	VkBufferCreateInfo deviceBufferCreateInfo;

	QueueFamilyIndexList queueFamilyIndices(1, deviceQueueFamilyIndex);

	deviceBufferCreateInfo.flags					= 0;
	deviceBufferCreateInfo.pNext					= nullptr;
	deviceBufferCreateInfo.pQueueFamilyIndices		= queueFamilyIndices.data();
	deviceBufferCreateInfo.queueFamilyIndexCount	= 1;
	deviceBufferCreateInfo.sharingMode				= VK_SHARING_MODE_EXCLUSIVE;
	deviceBufferCreateInfo.size						= 64;
	deviceBufferCreateInfo.usage					= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	deviceBufferCreateInfo.sType					= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

	std::shared_ptr<DeviceBuffer> deviceBuffer = DeviceBufferFactory::createDeviceBuffer(logicalDevice, physicalDevice, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &deviceBufferCreateInfo);

	for (uint32_t i = 0; i < swapChainImageCount; ++i)
	{
		VkComponentMapping imageComponentMapping;
		imageComponentMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageComponentMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageComponentMapping.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageComponentMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		VkImageSubresourceRange imageSubResourceRange;
		imageSubResourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
		imageSubResourceRange.baseMipLevel		= 0;
		imageSubResourceRange.levelCount		= 1;
		imageSubResourceRange.baseArrayLayer	= 0;
		imageSubResourceRange.layerCount		= 1;

		VkImageViewCreateInfo imageViewCreateInfo;
		imageViewCreateInfo.sType				= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.flags				= 0;
		imageViewCreateInfo.image				= swapChainImages[i];
		imageViewCreateInfo.format				= selectedSurfaceFormat.format;
		imageViewCreateInfo.viewType			= VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.components			= imageComponentMapping;
		imageViewCreateInfo.subresourceRange	= imageSubResourceRange;
		imageViewCreateInfo.pNext				= nullptr;

		assert(vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &swapChainImageViews[i]) == VK_SUCCESS);
	}

	CharacterBuffer vertexShaderBuffer;
	assert(File::read("Shaders\\SPIR-V\\vert.spv", &vertexShaderBuffer));

	VkShaderModule vertexShaderModule;
	VkShaderModuleCreateInfo vertexShaderCreateInfo;
	vertexShaderCreateInfo.sType	= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertexShaderCreateInfo.flags	= 0;
	vertexShaderCreateInfo.codeSize = vertexShaderBuffer.size();
	vertexShaderCreateInfo.pCode	= reinterpret_cast<uint32_t const *>(vertexShaderBuffer.data());
	vertexShaderCreateInfo.pNext	= nullptr;

	assert(vkCreateShaderModule(logicalDevice, &vertexShaderCreateInfo, nullptr, &vertexShaderModule) == VK_SUCCESS);

	CharacterBuffer fragmentShaderBuffer;
	assert(File::read("Shaders\\SPIR-V\\frag.spv", &fragmentShaderBuffer));

	VkShaderModule fragmentShaderModule;
	VkShaderModuleCreateInfo fragmentShaderCreateInfo;
	fragmentShaderCreateInfo.sType		= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	fragmentShaderCreateInfo.flags		= 0;
	fragmentShaderCreateInfo.codeSize	= fragmentShaderBuffer.size();
	fragmentShaderCreateInfo.pCode		= reinterpret_cast<uint32_t const *>(fragmentShaderBuffer.data());
	fragmentShaderCreateInfo.pNext		= nullptr;

	assert(vkCreateShaderModule(logicalDevice, &fragmentShaderCreateInfo, nullptr, &fragmentShaderModule) == VK_SUCCESS);

	Vertex vertexData[4] =
	{
		Vertex(Vector3(-0.5f, -0.5f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)),
		Vertex(Vector3(0.5f, -0.5f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
		Vertex(Vector3(0.5f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)),
		Vertex(Vector3(-0.5f, 0.5f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)),
	};

	Buffer * vertexBuffer = new Buffer(4 * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, QueueFamilyIndexList(1, deviceQueueFamilyIndex), logicalDevice);
	assert(vertexBuffer->allocate(physicalDevice, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	assert(vertexBuffer->fill(reinterpret_cast<void *>(vertexData), 0));
	vkBindBufferMemory(logicalDevice, vertexBuffer->getHandle(), vertexBuffer->getMemoryHandle(), 0);

	uint16_t indexData[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	Buffer * indexBuffer = new Buffer(6 * sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, QueueFamilyIndexList(1, deviceQueueFamilyIndex), logicalDevice);
	assert(indexBuffer->allocate(physicalDevice, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	assert(indexBuffer->fill(reinterpret_cast<void *>(indexData), 0));
	vkBindBufferMemory(logicalDevice, indexBuffer->getHandle(), indexBuffer->getMemoryHandle(), 0);

	SceneUniform sceneUniform
	(
		MatrixTransform::identityMatrix4(),
		MatrixTransform::view(Vector3(2.0f, 2.0f, 2.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)),
		MatrixTransform::projection(45.0f, 640.0f / 480.0f, 0.1, 10.0f)
	);

	/*
		GLM was originally designed for OpenGL, where the Y coordinate
		of the clip coordinates is inverted. The easiest way to compensate
		for that is to flip the sign on the scaling factor of the Y axis in
		the projection matrix.
	*/

	sceneUniform.projection()[1][1] *= -1;

	sceneUniform.view() = sceneUniform.view() * MatrixTransform::rotation(10.0f);

	Buffer * sceneUniformBuffer = new Buffer(sizeof(SceneUniform), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, QueueFamilyIndexList(1, deviceQueueFamilyIndex), logicalDevice);
	assert(sceneUniformBuffer->allocate(physicalDevice, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
	assert(sceneUniformBuffer->fill(reinterpret_cast<void *>(&sceneUniform), 0));
	vkBindBufferMemory(logicalDevice, sceneUniformBuffer->getHandle(), sceneUniformBuffer->getMemoryHandle(), 0);

	VkPipelineShaderStageCreateInfo vertexShaderStageInfo;
	vertexShaderStageInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageInfo.flags					= 0;
	vertexShaderStageInfo.pSpecializationInfo	= nullptr;
	vertexShaderStageInfo.pNext					= nullptr;
	vertexShaderStageInfo.stage					= VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageInfo.module				= vertexShaderModule;
	vertexShaderStageInfo.pName					= "main";

	VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;
	fragmentShaderStageInfo.sType				= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderStageInfo.flags				= 0;
	fragmentShaderStageInfo.stage				= VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderStageInfo.module				= fragmentShaderModule;
	fragmentShaderStageInfo.pName				= "main";
	fragmentShaderStageInfo.pSpecializationInfo	= nullptr;
	fragmentShaderStageInfo.pNext				= nullptr;

	VkPipelineShaderStageCreateInfo shaderStages[2] = { vertexShaderStageInfo, fragmentShaderStageInfo };

	VkVertexInputBindingDescription vertexInputBindingDescription;

	vertexInputBindingDescription.binding	= 0;
	vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertexInputBindingDescription.stride	= sizeof(Vertex);

	VkVertexInputAttributeDescription positionInputAttributeDescription;

	positionInputAttributeDescription.binding	= 0;
	positionInputAttributeDescription.format	= VK_FORMAT_R32G32B32_SFLOAT;
	positionInputAttributeDescription.location	= 0;
	positionInputAttributeDescription.offset	= 0;

	VkVertexInputAttributeDescription colorInputAttributeDescription;

	colorInputAttributeDescription.binding	= 0;
	colorInputAttributeDescription.format	= VK_FORMAT_R32G32B32_SFLOAT;
	colorInputAttributeDescription.location	= 1;
	colorInputAttributeDescription.offset	= sizeof(Vector3);

	VkVertexInputAttributeDescription vertexInputAttributeDescriptions[2] =
	{
		positionInputAttributeDescription,
		colorInputAttributeDescription
	};

	VkPipelineVertexInputStateCreateInfo vertexInputStateInfo;
	vertexInputStateInfo.sType								= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateInfo.flags								= 0;
	vertexInputStateInfo.vertexBindingDescriptionCount		= 1;
	vertexInputStateInfo.pVertexBindingDescriptions			= &vertexInputBindingDescription;
	vertexInputStateInfo.vertexAttributeDescriptionCount	= 2;
	vertexInputStateInfo.pVertexAttributeDescriptions		= vertexInputAttributeDescriptions;
	vertexInputStateInfo.pNext								= nullptr;

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo;
	inputAssemblyStateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateInfo.flags					= 0;
	inputAssemblyStateInfo.topology					= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyStateInfo.primitiveRestartEnable	= VK_FALSE;
	inputAssemblyStateInfo.pNext					= nullptr;

	VkExtent2D const & currentExtent = deviceSurfaceCapabilities.currentExtent;

	VkViewport viewport;
	viewport.x			= 0.0f;
	viewport.y			= 0.0f;
	viewport.width		= (float)currentExtent.width;
	viewport.height		= (float)currentExtent.height;
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;

	VkRect2D scissorRect;
	scissorRect.offset = { 0, 0 };
	scissorRect.extent = currentExtent;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
	viewportStateCreateInfo.sType			= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.flags			= 0;
	viewportStateCreateInfo.viewportCount	= 1;
	viewportStateCreateInfo.pViewports		= &viewport;
	viewportStateCreateInfo.scissorCount	= 1;
	viewportStateCreateInfo.pScissors		= &scissorRect;
	viewportStateCreateInfo.pNext			= nullptr;

	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
	rasterizationStateCreateInfo.sType						= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationStateCreateInfo.flags						= 0;
	rasterizationStateCreateInfo.rasterizerDiscardEnable	= VK_FALSE;
	rasterizationStateCreateInfo.depthClampEnable			= VK_FALSE;
	rasterizationStateCreateInfo.polygonMode				= VK_POLYGON_MODE_FILL;
	rasterizationStateCreateInfo.cullMode					= VK_CULL_MODE_BACK_BIT;
	rasterizationStateCreateInfo.frontFace					= VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationStateCreateInfo.lineWidth					= 1.0f;
	rasterizationStateCreateInfo.depthBiasEnable			= VK_FALSE;
	rasterizationStateCreateInfo.depthBiasConstantFactor	= 0.0f;
	rasterizationStateCreateInfo.depthBiasClamp				= 0.0f;
	rasterizationStateCreateInfo.depthBiasSlopeFactor		= 0.0f;
	rasterizationStateCreateInfo.pNext						= nullptr;

	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
	multisampleStateCreateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampleStateCreateInfo.flags					= 0;
	multisampleStateCreateInfo.sampleShadingEnable		= VK_FALSE;
	multisampleStateCreateInfo.rasterizationSamples		= VK_SAMPLE_COUNT_1_BIT;
	multisampleStateCreateInfo.minSampleShading			= 1.0f;
	multisampleStateCreateInfo.pSampleMask				= nullptr;
	multisampleStateCreateInfo.alphaToCoverageEnable	= VK_FALSE;
	multisampleStateCreateInfo.alphaToOneEnable			= VK_FALSE;
	multisampleStateCreateInfo.pNext					= nullptr;

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
	colorBlendAttachmentState.colorWriteMask		= VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachmentState.blendEnable			= VK_TRUE;
	colorBlendAttachmentState.srcColorBlendFactor	= VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachmentState.dstColorBlendFactor	= VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachmentState.colorBlendOp			= VK_BLEND_OP_ADD;
	colorBlendAttachmentState.srcAlphaBlendFactor	= VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstAlphaBlendFactor	= VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.alphaBlendOp			= VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo	= {};
	colorBlendStateCreateInfo.flags									= 0;
	colorBlendStateCreateInfo.sType									= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendStateCreateInfo.logicOpEnable							= VK_FALSE;
	colorBlendStateCreateInfo.logicOp								= VK_LOGIC_OP_COPY;
	colorBlendStateCreateInfo.attachmentCount						= 1;
	colorBlendStateCreateInfo.pAttachments							= &colorBlendAttachmentState;
	colorBlendStateCreateInfo.blendConstants[0]						= 0.0f;
	colorBlendStateCreateInfo.blendConstants[1]						= 0.0f;
	colorBlendStateCreateInfo.blendConstants[2]						= 0.0f;
	colorBlendStateCreateInfo.blendConstants[3]						= 0.0f;
	colorBlendStateCreateInfo.pNext									= nullptr;

	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
	dynamicStateCreateInfo.sType				= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.flags				= 0;
	dynamicStateCreateInfo.dynamicStateCount	= 2;
	dynamicStateCreateInfo.pDynamicStates		= dynamicStates;
	dynamicStateCreateInfo.pNext				= nullptr;

	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding;

	descriptorSetLayoutBinding.binding				= 0;
	descriptorSetLayoutBinding.descriptorCount		= 1;
	descriptorSetLayoutBinding.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorSetLayoutBinding.stageFlags			= VK_SHADER_STAGE_VERTEX_BIT;
	descriptorSetLayoutBinding.pImmutableSamplers	= nullptr;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;

	descriptorSetLayoutCreateInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.flags			= 0;
	descriptorSetLayoutCreateInfo.bindingCount	= 1;
	descriptorSetLayoutCreateInfo.pBindings		= &descriptorSetLayoutBinding;
	descriptorSetLayoutCreateInfo.pNext			= nullptr;

	assert(vkCreateDescriptorSetLayout(logicalDevice, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout) == VK_SUCCESS);

	VkDescriptorPoolSize descriptorPoolSize;
	descriptorPoolSize.type				= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorPoolSize.descriptorCount	= 1;

	VkDescriptorPool descriptorPool;
	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
	descriptorPoolCreateInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags			= 0;
	descriptorPoolCreateInfo.maxSets		= 1;
	descriptorPoolCreateInfo.poolSizeCount	= 1;
	descriptorPoolCreateInfo.pPoolSizes		= &descriptorPoolSize;
	descriptorPoolCreateInfo.pNext			= nullptr;

	assert(vkCreateDescriptorPool(logicalDevice, &descriptorPoolCreateInfo, nullptr, &descriptorPool) == VK_SUCCESS);

	VkDescriptorSet descriptorSet;
	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
	VkDescriptorSetLayout layouts[] = { descriptorSetLayout };
	descriptorSetAllocateInfo.sType					= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.descriptorPool		= descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount	= 1;
	descriptorSetAllocateInfo.pSetLayouts			= &descriptorSetLayout;
	descriptorSetAllocateInfo.pNext					= nullptr;

	assert(vkAllocateDescriptorSets(logicalDevice, &descriptorSetAllocateInfo, &descriptorSet) == VK_SUCCESS);

	VkDescriptorBufferInfo descriptorBufferInfo;
	descriptorBufferInfo.buffer	= sceneUniformBuffer->getHandle();
	descriptorBufferInfo.offset	= 0;
	descriptorBufferInfo.range	= sizeof(SceneUniform);

	VkWriteDescriptorSet writeDescriptorSet;
	writeDescriptorSet.sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescriptorSet.dstSet			= descriptorSet;
	writeDescriptorSet.dstBinding		= 0;
	writeDescriptorSet.dstArrayElement	= 0;
	writeDescriptorSet.descriptorCount	= 1;
	writeDescriptorSet.descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSet.pBufferInfo		= &descriptorBufferInfo;
	writeDescriptorSet.pImageInfo		= nullptr;
	writeDescriptorSet.pTexelBufferView = nullptr;
	writeDescriptorSet.pNext			= nullptr;

	vkUpdateDescriptorSets(logicalDevice, 1, &writeDescriptorSet, 0, nullptr);

	VkPipelineLayout pipelineLayout;

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.flags					= 0;
	pipelineLayoutCreateInfo.setLayoutCount			= 1;
	pipelineLayoutCreateInfo.pSetLayouts			= &descriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges	= 0;
	pipelineLayoutCreateInfo.pNext					= nullptr;
	
	assert(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout) == VK_SUCCESS);

	VkAttachmentDescription colorAttachmentDescription;
	colorAttachmentDescription.flags			= 0;
	colorAttachmentDescription.format			= selectedSurfaceFormat.format;
    colorAttachmentDescription.samples			= VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentDescription.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachmentDescription.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentDescription.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentDescription.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentDescription.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentDescription.finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentReference;
	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription;
	subpassDescription.colorAttachmentCount		= 1;
	subpassDescription.pColorAttachments		= &colorAttachmentReference;
	subpassDescription.flags					= 0;
	subpassDescription.inputAttachmentCount		= 0;
	subpassDescription.pInputAttachments		= nullptr;
	subpassDescription.pResolveAttachments		= nullptr;
	subpassDescription.preserveAttachmentCount	= 0;
	subpassDescription.pPreserveAttachments		= nullptr;
	subpassDescription.pDepthStencilAttachment	= nullptr;
	subpassDescription.pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;

	VkRenderPass renderPass;

	VkRenderPassCreateInfo renderPassCreateInfo;
	renderPassCreateInfo.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount	= 1;
	renderPassCreateInfo.pAttachments		= &colorAttachmentDescription;
	renderPassCreateInfo.subpassCount		= 1;
	renderPassCreateInfo.pSubpasses			= &subpassDescription;
	renderPassCreateInfo.dependencyCount	= 0;
	renderPassCreateInfo.flags				= 0;
	renderPassCreateInfo.pDependencies		= nullptr;
	renderPassCreateInfo.pNext				= nullptr;
	
	assert(vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &renderPass) == VK_SUCCESS);

	VkPipeline graphicsPipeline;
	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType				= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.flags				= 0;
	graphicsPipelineCreateInfo.stageCount			= 2;
	graphicsPipelineCreateInfo.pStages				= shaderStages;
	graphicsPipelineCreateInfo.layout				= pipelineLayout;
	graphicsPipelineCreateInfo.renderPass			= renderPass;
	graphicsPipelineCreateInfo.subpass				= 0;
	graphicsPipelineCreateInfo.pVertexInputState	= &vertexInputStateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState	= &inputAssemblyStateInfo;
	graphicsPipelineCreateInfo.pTessellationState	= nullptr;
	graphicsPipelineCreateInfo.pRasterizationState	= &rasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState	= &multisampleStateCreateInfo;
	graphicsPipelineCreateInfo.pViewportState		= &viewportStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState	= nullptr;
	graphicsPipelineCreateInfo.pColorBlendState		= &colorBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState		= nullptr;
	graphicsPipelineCreateInfo.basePipelineHandle	= VK_NULL_HANDLE;
	graphicsPipelineCreateInfo.basePipelineIndex	= -1;
	graphicsPipelineCreateInfo.pNext				= nullptr;

	assert(vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &graphicsPipeline) == VK_SUCCESS);

	VkFramebuffer * swapChainFrameBuffers = new VkFramebuffer[swapChainImageCount]();

	for (uint32_t i = 0; i < swapChainImageCount; ++i)
	{
		VkFramebufferCreateInfo frameBufferCreateInfo;
		frameBufferCreateInfo.sType				= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.flags				= 0;
		frameBufferCreateInfo.attachmentCount	= 1;
		frameBufferCreateInfo.pAttachments		= &swapChainImageViews[i];
		frameBufferCreateInfo.width				= currentExtent.width;
		frameBufferCreateInfo.height			= currentExtent.height;
		frameBufferCreateInfo.layers			= 1;
		frameBufferCreateInfo.renderPass		= renderPass;
		frameBufferCreateInfo.pNext				= nullptr;

		assert(vkCreateFramebuffer(logicalDevice, &frameBufferCreateInfo, nullptr, &swapChainFrameBuffers[i]) == VK_SUCCESS);
	}

	VkCommandPool commandPool;
	VkCommandPoolCreateInfo commandPoolCreateInfo;
	commandPoolCreateInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags				= 0;
	commandPoolCreateInfo.queueFamilyIndex	= deviceQueueFamilyIndex;
	commandPoolCreateInfo.pNext				= nullptr;

	assert(vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool) == VK_SUCCESS);

	VkCommandBuffer * commandBuffers = new VkCommandBuffer[swapChainImageCount]();

	VkCommandBufferAllocateInfo commandBufferAllocateInfo;
	commandBufferAllocateInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool			= commandPool;
	commandBufferAllocateInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount	= swapChainImageCount;
	commandBufferAllocateInfo.pNext					= nullptr;

	assert(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, commandBuffers) == VK_SUCCESS);

	for (uint32_t i = 0; i < swapChainImageCount; ++i)
	{
		VkCommandBufferBeginInfo commandBufferBeginInfo;
		commandBufferBeginInfo.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags			= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		commandBufferBeginInfo.pInheritanceInfo = nullptr;
		commandBufferBeginInfo.pNext			= nullptr;

		assert(vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo) == VK_SUCCESS);

		VkClearValue clearColor = { 1.0f, 0.411765f, 0.705882f, 1.0f };

		VkRenderPassBeginInfo renderPassBeginInfo	= {};
		renderPassBeginInfo.sType					= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass				= renderPass;
		renderPassBeginInfo.framebuffer				= swapChainFrameBuffers[i];
		renderPassBeginInfo.pNext					= nullptr;
		renderPassBeginInfo.renderArea.offset		= {0, 0};
		renderPassBeginInfo.renderArea.extent		= currentExtent;
		renderPassBeginInfo.clearValueCount			= 1;
		renderPassBeginInfo.pClearValues			= &clearColor;

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

		VkBuffer vertexBuffers[] = { vertexBuffer->getHandle() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT16);

		vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

		vkCmdDrawIndexed(commandBuffers[i], 6, 1, 0, 0, 0);

		vkCmdEndRenderPass(commandBuffers[i]);

		assert(vkEndCommandBuffer(commandBuffers[i]) == VK_SUCCESS);
	}

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderCompleteSemaphore;

	VkSemaphoreCreateInfo imageAvailableCreateInfo;
	imageAvailableCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	imageAvailableCreateInfo.flags = 0;
	imageAvailableCreateInfo.pNext = nullptr;

	VkSemaphoreCreateInfo renderCompleteCreateInfo;
	renderCompleteCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	renderCompleteCreateInfo.flags = 0;
	renderCompleteCreateInfo.pNext = nullptr;

	assert(vkCreateSemaphore(logicalDevice, &imageAvailableCreateInfo, nullptr, &imageAvailableSemaphore) == VK_SUCCESS && vkCreateSemaphore(logicalDevice, &renderCompleteCreateInfo, nullptr, &renderCompleteSemaphore) == VK_SUCCESS);

	Matrix4 initialViewMatrix = sceneUniform.view();

	float viewRotation = 0.0f;

	Time::initialize();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		uint32_t imageIndex = 0;
		vkAcquireNextImageKHR(logicalDevice, deviceSwapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType		= VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[]		= { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[]	= { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount		= 1;
		submitInfo.pWaitSemaphores			= waitSemaphores;
		submitInfo.pWaitDstStageMask		= waitStages;

		submitInfo.commandBufferCount	= 1;
		submitInfo.pCommandBuffers		= &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[]	= { renderCompleteSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores	= signalSemaphores;

		if (vkQueueSubmit(presentationQueue.getHandle(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo	= {};
		presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount	= 1;
		presentInfo.pWaitSemaphores		= signalSemaphores;

		VkSwapchainKHR swapChains[]		= { deviceSwapChain };
		presentInfo.swapchainCount		= 1;
		presentInfo.pSwapchains			= swapChains;

		presentInfo.pImageIndices		= &imageIndex;

		vkQueuePresentKHR(presentationQueue.getHandle(), &presentInfo);

		vkQueueWaitIdle(presentationQueue.getHandle());

		viewRotation += 30.0f * Time::deltaTime;

		sceneUniform.view() = initialViewMatrix * MatrixTransform::rotation(viewRotation);
		sceneUniformBuffer->fill(&sceneUniform, 0);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		Time::update();
	}

	vkDeviceWaitIdle(logicalDevice);

	for (int i = 0; i < 2; ++i)
	{
		vkDestroyFramebuffer(logicalDevice, swapChainFrameBuffers[i], nullptr);
	}

	vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);
	vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
	delete(sceneUniformBuffer);
	delete(vertexBuffer);
	vkDestroySemaphore(logicalDevice, renderCompleteSemaphore, nullptr);
	vkDestroySemaphore(logicalDevice, imageAvailableSemaphore, nullptr);
	vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
	delete[](swapChainFrameBuffers);
	vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);
	vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
	delete[](swapChainImageViews);
	delete[](swapChainImages);
	vkDestroySwapchainKHR(logicalDevice, deviceSwapChain, nullptr);
	delete[](deviceSupportedSurfaceFormats);
	vkDestroyDevice(logicalDevice, nullptr);
	vkDestroyInstance(vulkanInstance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}