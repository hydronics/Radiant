#pragma once

#include "Radiant/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Radiant {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* window);
		virtual ~OpenGLContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_window_handle;
	};
}
