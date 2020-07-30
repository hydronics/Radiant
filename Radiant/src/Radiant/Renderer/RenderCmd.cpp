#include "rdpch.h"

#include "RenderCmd.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Radiant {

	RendererAPI* RenderCmd::s_renderer_api = new OpenGLRendererAPI;

}
