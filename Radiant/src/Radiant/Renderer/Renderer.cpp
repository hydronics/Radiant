#include "rdpch.h"
#include "Renderer.h"
#include "Renderer2d.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Radiant {
	Renderer::SceneData* Renderer::s_scene_data = new Renderer::SceneData;


	void Renderer::Init()
	{
		RD_PROFILE_FUNCTION();

		Renderer2d::Init();
		RenderCmd::Init();
	}

	void Renderer::Shutdown()
	{
		RD_PROFILE_FUNCTION();
	}

	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RD_PROFILE_FUNCTION();

		RenderCmd::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const OrthoCamera& camera)
	{
		RD_PROFILE_FUNCTION();

		s_scene_data->view_projection_matrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		RD_PROFILE_FUNCTION();
	}

}
