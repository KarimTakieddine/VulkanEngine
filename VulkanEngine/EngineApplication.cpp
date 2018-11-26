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

VkResult EngineApplication::initialize(char const * applicationName, char const * engineName, CStringList const & layerNames, CStringList const & extensionNames)
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

	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledLayerCount = layerNames.size();
	instanceCreateInfo.ppEnabledLayerNames = layerNames.data();
	instanceCreateInfo.enabledExtensionCount = extensionNames.size();
	instanceCreateInfo.ppEnabledExtensionNames = extensionNames.data();

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

EngineApplication::EngineApplication() : m_window(nullptr), m_vulkanInstance(VK_NULL_HANDLE) { }