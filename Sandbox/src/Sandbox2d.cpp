#include "Sandbox2d.h"
#include "Radiant/Renderer/Renderer2d.h"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>


Sandbox2d::Sandbox2d() 
#ifdef RD_DEBUG
	: Layer("Sandbox2d")
#else
	: Layer()
#endif
	, m_camera_controller(1920.0f / 1080.0f)
{

}

Sandbox2d::~Sandbox2d()
{

}

void Sandbox2d::OnAttach()
{
	m_texture = Radiant::Texture2d::Create("assets/textures/Checkerboard.png");
}

void Sandbox2d::OnDetach()
{
}

void Sandbox2d::OnUpdate(Radiant::Timestep timestep)
{
	RD_PROFILE_FUNCTION();

	float ts = timestep;

	// OnUpdate phase
	m_camera_controller.OnUpdate(timestep);

	// OnRender preparation
	{
		RD_PROFILE_SCOPE("RenderCmd setup");
		Radiant::RenderCmd::SetClearColor({ 0.12f, 0.12f, 0.12f, 1.0f });
		Radiant::RenderCmd::Clear();
	}

	// OnRender Begin scene / draw quads
	{
		RD_PROFILE_SCOPE("Renderer2d BEGIN / scene draw");
		Radiant::Renderer2d::BeginScene(m_camera_controller.GetCamera());
		Radiant::Renderer2d::DrawQuad({ -0.5f, -0.5f }, { 1.0f, 1.0f }, { 0.75f, 0.33f, 0.12f, 1.0f });
		Radiant::Renderer2d::DrawQuad({ 0.5f, 0.5f }, { 0.5f, 0.8f }, { 0.35f, 0.33f, 0.72f, 1.0f });
		Radiant::Renderer2d::DrawQuad({ 0.85f, -0.75f, 0.1f }, { 10.0f, 10.0f }, m_texture);
	}

	// OnRender End scene
	{
		RD_PROFILE_SCOPE("Renderer2d END Scene");
		Radiant::Renderer2d::EndScene();
	}
}

void Sandbox2d::OnEvent(Radiant::Event& e)
{
	m_camera_controller.OnEvent(e);
}