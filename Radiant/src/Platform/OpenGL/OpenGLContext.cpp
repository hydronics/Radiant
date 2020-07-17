#include "rdpch.h"

#include "OpenGlContext.h"
#include <glfw/glfw3.h>
#include <glad/glad.h>

namespace Radiant {



	OpenGLContext::OpenGLContext(GLFWwindow* window) : GraphicsContext()
		, m_window_handle(window)
	{
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_window_handle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RD_CORE_ASSERT(status, "Glad failed to initialize loader from GLFW");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_window_handle);
	}

}
