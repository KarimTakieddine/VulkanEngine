#include "stdafx.h"
#include "EngineApplication.h"

const VersionNumber EngineApplication::APIVersion = VersionNumber(1, 1, 77);

const VersionNumber EngineApplication::ApplicationVersion = VersionNumber(1, 0, 0);

const VersionNumber EngineApplication::EngineVersion = VersionNumber(1, 0, 0);

EngineApplication & EngineApplication::getInstance()
{
	/*
		From the ISO/IEC 2012 standard, section 6.7.4:

		The zero-initialization of all block-scope variables with static storage
		duration or thread storage duration is performed before any other initialization
		takes place.

		If control enters the declaration concurrently while the variable is being
		initialized, the concurrent execution shall wait for completion of the
		initialization.
	*/

	static EngineApplication instance;

	return instance;
}

VkResult EngineApplication::initializeContext(char const * applicationName, char const * engineName, CStringList const & layerNames, CStringList const & extensionNames)
{
	if (glfwInit() != VK_TRUE)
	{
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	VkApplicationInfo applicationInfo;

	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = nullptr;
	applicationInfo.apiVersion = APIVersion.toInteger();
	applicationInfo.applicationVersion = ApplicationVersion.toInteger();
	applicationInfo.engineVersion = EngineVersion.toInteger();
	applicationInfo.pApplicationName = applicationName;
	applicationInfo.pEngineName = engineName;

	VkInstanceCreateInfo instanceCreateInfo;

	uint32_t requiredExtensionCount = 0;

	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledLayerCount = layerNames.size();
	instanceCreateInfo.ppEnabledLayerNames = layerNames.data();
	instanceCreateInfo.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
	instanceCreateInfo.enabledExtensionCount = requiredExtensionCount;

	VkResult instanceCreateResult = vkCreateInstance(&instanceCreateInfo, nullptr, &m_vulkanInstance);

	if (instanceCreateResult != VK_SUCCESS)
	{
		return instanceCreateResult;
	}

	return VK_SUCCESS;
}

VkResult EngineApplication::initializeWindow(char const * name, int width, int height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);

	if (!m_window)
	{
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	glfwSetWindowUserPointer(m_window, this);
	glfwSetWindowSizeCallback(m_window, &EngineApplication::onWindowResized);

	return VK_SUCCESS;
}

int EngineApplication::getSupportedDeviceIndex(VersionNumber const & driverVersion, PhysicalDeviceList const & physicalDeviceList)
{
	for (uint32_t i = 0; i < physicalDeviceList.size(); ++i)
	{
		VkPhysicalDeviceProperties deviceProperties;

		vkGetPhysicalDeviceProperties(physicalDeviceList[i], &deviceProperties);

		VersionNumber deviceAPIVersionNumber = VersionNumber::fromInteger(deviceProperties.apiVersion);
		VersionNumber deviceDriverVersionNumber = VersionNumber::fromInteger(deviceProperties.driverVersion);

		if (
			deviceAPIVersionNumber >= APIVersion &&
			deviceDriverVersionNumber >= driverVersion
			)
		{
			return i;
		}
	}

	return -1;
}

VkResult EngineApplication::selectPhysicalDevice(VersionNumber const & driverVersion)
{
	uint32_t physicalDeviceCount = 0;

	vkEnumeratePhysicalDevices(m_vulkanInstance, &physicalDeviceCount, nullptr);

	if (physicalDeviceCount == 0)
	{
		return VK_ERROR_DEVICE_LOST;
	}

	PhysicalDeviceList physicalDeviceList(physicalDeviceCount);

	vkEnumeratePhysicalDevices(m_vulkanInstance, &physicalDeviceCount, physicalDeviceList.data());

	int supportedDeviceIndex = getSupportedDeviceIndex(driverVersion, physicalDeviceList);

	if (supportedDeviceIndex == -1)
	{
		return VK_ERROR_INCOMPATIBLE_DRIVER;
	}

	m_physicalDevice = physicalDeviceList[supportedDeviceIndex];

	return VK_SUCCESS;
}

int EngineApplication::getQueueFamilyIndex(VkPhysicalDevice physicalDevice, uint32_t requiredCount, VkQueueFlags requiredFlags)
{
	uint32_t deviceQueueFamilyCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyCount, nullptr);

	if (deviceQueueFamilyCount == 0)
	{
		return -1;
	}

	std::vector<VkQueueFamilyProperties> queueFamilyPropertiesList(deviceQueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyCount, queueFamilyPropertiesList.data());

	for (uint32_t i = 0; i < queueFamilyPropertiesList.size(); ++i)
	{
		VkQueueFamilyProperties const & queueFamilyProperties = queueFamilyPropertiesList[i];

		if (
			( queueFamilyProperties.queueCount >= requiredCount ) &&
			( (queueFamilyProperties.queueFlags & requiredFlags) == requiredFlags )
		)
		{
			return i;
		}
	}

	return -1;
}

VkResult EngineApplication::initializeLogicalDevice()
{
	int deviceQueueFamilyIndex = getQueueFamilyIndex(m_physicalDevice, 1, VK_QUEUE_TRANSFER_BIT | VK_QUEUE_GRAPHICS_BIT);

	if (deviceQueueFamilyIndex == -1)
	{
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	static CStringList deviceExtensionNames(1, "VK_KHR_swapchain");

	VkDeviceQueueCreateInfo deviceQueueCreateInfo;

	float queuePriorities[1]				= { 1.0f };
	deviceQueueCreateInfo.sType				= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.flags				= 0;
	deviceQueueCreateInfo.queueCount		= 1;
	deviceQueueCreateInfo.pQueuePriorities	= queuePriorities;
	deviceQueueCreateInfo.queueFamilyIndex	= deviceQueueFamilyIndex;
	deviceQueueCreateInfo.pNext				= nullptr;

	VkPhysicalDeviceFeatures enabledDeviceFeatures;
	vkGetPhysicalDeviceFeatures(m_physicalDevice, &enabledDeviceFeatures);

	VkDeviceCreateInfo logicalDeviceCreateInfo;
	logicalDeviceCreateInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	logicalDeviceCreateInfo.flags					= 0;
	logicalDeviceCreateInfo.queueCreateInfoCount	= 1;
	logicalDeviceCreateInfo.pEnabledFeatures		= &enabledDeviceFeatures;
	logicalDeviceCreateInfo.pQueueCreateInfos		= &deviceQueueCreateInfo;
	logicalDeviceCreateInfo.enabledLayerCount		= 0;
	logicalDeviceCreateInfo.ppEnabledLayerNames		= nullptr;
	logicalDeviceCreateInfo.enabledExtensionCount	= 1;
	logicalDeviceCreateInfo.ppEnabledExtensionNames = deviceExtensionNames.data();
	logicalDeviceCreateInfo.pNext					= nullptr;
	
	VkResult a = vkCreateDevice(m_physicalDevice, &logicalDeviceCreateInfo, nullptr, &m_logicalDevice);

	return a;
}

VkResult EngineApplication::initializeDevice(VersionNumber const & driverVersion)
{
	VkResult physicalDeviceStatus = selectPhysicalDevice(driverVersion);

	if (physicalDeviceStatus != VK_SUCCESS)
	{
		return physicalDeviceStatus;
	}

	VkResult logicalDeviceStatus = initializeLogicalDevice();

	if (logicalDeviceStatus != VK_SUCCESS)
	{
		return logicalDeviceStatus;
	}

	return VK_SUCCESS;
}

EngineApplication::~EngineApplication()
{
	vkDestroyInstance(m_vulkanInstance, nullptr);
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void EngineApplication::onWindowResized(GLFWwindow * window, int width, int height)
{
	/*
		Emulating invocation of non-static member function by storing
		transient EngineApplication state.
	*/

	EngineApplication * instance = reinterpret_cast<EngineApplication *>(glfwGetWindowUserPointer(window));
}

EngineApplication::EngineApplication()
:
m_window(nullptr),
m_vulkanInstance(VK_NULL_HANDLE),
m_physicalDevice(VK_NULL_HANDLE),
m_logicalDevice(VK_NULL_HANDLE),
m_deviceQueue(VK_NULL_HANDLE)
{

}