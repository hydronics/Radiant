#include "rdpch.h"

#include "Win32RadiantWindow.h"
#include "Radiant/Events/RadiantEvents.h"
#include "Radiant/InputUtils.h"

#include "Platform/OpenGL/OpenGlContext.h"

namespace Radiant {

	static bool s_glfw_initialized = false;

	static void GlfwErrorCallback(int error, const char* description)
	{
		RD_CORE_ERROR("GLFW Error ({0}) : {1}", error, description);
	}

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

		m_context = new OpenGLContext(m_glfw_window);
		m_context->Init();

		glfwSetWindowUserPointer(m_glfw_window, &m_window_data);

		SetVSync(true);

		// glfw callbacks to window/application for event processing //

		// WINDOW EVENTS (RESIZE, CLOSE)
		glfwSetWindowSizeCallback(m_glfw_window, [](GLFWwindow* win, int w, int h)
		{
			auto win_data = *(WinDat*)glfwGetWindowUserPointer(win);
			win_data.props.width = w;
			win_data.props.height = h;

			WindowResizeEvent evt(w, h);
			win_data.event_callback(evt);
		});

		glfwSetWindowCloseCallback(m_glfw_window, [](GLFWwindow* win) {
			auto win_data = *(WinDat*)glfwGetWindowUserPointer(win);
			WindowCloseEvent evt;
			win_data.event_callback(evt);
		});

		// KEY EVENTS
		glfwSetKeyCallback(m_glfw_window, [](GLFWwindow* win, int key, int scancode, int action, int mod) {
			auto win_data = *(WinDat*)glfwGetWindowUserPointer(win);
			// switch on the action to determine what kind of key event (press, release, repeat)
			switch (action)
			{
				case GLFW_REPEAT:
				{
					KeyboardPressEvent evt(GLFW_To_Radiant_Keycode(key), 1);
					win_data.event_callback(evt);
					break;
				}
				case GLFW_PRESS:
				{
					KeyboardPressEvent evt(key, 0);
					win_data.event_callback(evt);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyboardReleaseEvent evt(key, 0);
					win_data.event_callback(evt);
					break;
				}
			}
		});

		glfwSetCharCallback(m_glfw_window, [](GLFWwindow* win, unsigned int key) {
			auto win_data = *(WinDat*)glfwGetWindowUserPointer(win);
			KeyboardTypedEvent evt(key);
			win_data.event_callback(evt);
		});

		// MOUSE EVENTS
		glfwSetMouseButtonCallback(m_glfw_window, [](GLFWwindow* win, int btn, int action, int mods) {
			auto win_data = *(WinDat*)glfwGetWindowUserPointer(win);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseClickEvent evt(btn);
					win_data.event_callback(evt);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseReleaseEvent evt(btn);
					win_data.event_callback(evt);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_glfw_window, [](GLFWwindow* win, double xoff, double yoff) {
			auto win_data = *(WinDat*)glfwGetWindowUserPointer(win);
			MouseScrollEvent evt((float)xoff, (float)yoff);
			win_data.event_callback(evt);
		});

		glfwSetCursorPosCallback(m_glfw_window, [](GLFWwindow* win, double x, double y) {
			auto win_data = *(WinDat*)glfwGetWindowUserPointer(win);
			MouseMoveEvent evt((float)x, (float)y);
			win_data.event_callback(evt);
		});

		// ERROR Callback
		glfwSetErrorCallback(GlfwErrorCallback);
	}

	void Win32RadiantWindow::ShutdownWindow()
	{
		glfwDestroyWindow(m_glfw_window);
	}

	void Win32RadiantWindow::OnUpdate()
	{
		glfwPollEvents();
		m_context->SwapBuffers();
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

}
