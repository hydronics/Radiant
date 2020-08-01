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
		RD_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_window_handle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RD_CORE_ASSERT(status, "Glad failed to initialize loader from GLFW");

		RD_CORE_INFO("OpenGL Renderer Info:");
		RD_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		RD_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		RD_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		RD_PROFILE_FUNCTION();

		glfwSwapBuffers(m_window_handle);
	}

}
