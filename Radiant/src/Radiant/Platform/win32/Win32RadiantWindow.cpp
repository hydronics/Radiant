#include "rdpch.h"
#include "Win32RadiantWindow.h"

namespace Radiant {

	static bool s_glfw_initialized = false;

	RadiantWindow* RadiantWindow::Create(const WindowProps& props)
	{
		return new Win32RadiantWindow(props);
	}

	Win32RadiantWindow::Win32RadiantWindow(const WindowProps& props)
	{
		InitWindow(props);
	}

	Win32RadiantWindow::~Win32RadiantWindow()
	{
		ShutdownWindow();
	}

	void Win32RadiantWindow::InitWindow(const WindowProps& props)
	{
		m_window_data.props = props;

		RD_CORE_INFO("Initializing Win32 Radiant Window - ({0}x{1}) - {2}", props.width, props.height, props.name);

		if (!s_glfw_initialized)
		{
			// Initialize GLFW for the first time.  Acts like singleton
			int success = glfwInit();

			RD_CORE_ASSERT(success, "ERROR Initializing GLFW!!");
			s_glfw_initialized = true;
		}

		m_glfw_window = glfwCreateWindow((int)props.width, (int)props.height, props.name.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_glfw_window);
		glfwSetWindowUserPointer(m_glfw_window, &m_window_data);
		SetVSync(true);
	}

	void Win32RadiantWindow::ShutdownWindow()
	{
		glfwDestroyWindow(m_glfw_window);
	}

	void Win32RadiantWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_glfw_window);
	}

	unsigned int Win32RadiantWindow::GetWidth() const
	{
		return m_window_data.props.width;
	}

	unsigned int Win32RadiantWindow::GetHeight() const
	{
		return m_window_data.props.height;
	}

	void Win32RadiantWindow::SetVSync(bool vsync)
	{
		if (vsync) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}

		m_window_data.vsync_enabled = vsync;
	}

	bool Win32RadiantWindow::IsVSyncEnabled() const
	{
		return m_window_data.vsync_enabled;
	}

	void Win32RadiantWindow::SetEventCallback(EventCallbackFn EvtCallback)
	{
		m_window_data.event_callback = EvtCallback;
	}

}
