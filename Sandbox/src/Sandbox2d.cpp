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
	m_rpg_sprite_sheet = Radiant::Texture2d::Create("assets/textures/RPGpack_sheet_2X.png");
	m_grass = Radiant::SubTexture2d::CreateFromCoords(m_rpg_sprite_sheet, { 8, 1 }, { 128, 128 }, { 1, 1 });
}

void Sandbox2d::OnDetach()
{
}

void Sandbox2d::OnImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = Radiant::Renderer2d::GetStats();

	ImGui::Text("Radiant Renderer2d Statistics:");

	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Total Vertex Count: %d", stats.GetTotalVertexCount());
	ImGui::Text("Total Index Count: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

void Sandbox2d::OnUpdate(Radiant::Timestep timestep)
{
	RD_PROFILE_FUNCTION();

	float ts = timestep;

	// OnUpdate phase
	m_camera_controller.OnUpdate(timestep);

	// OnRender preparation
	Radiant::Renderer2d::ResetStats();
	{
		RD_PROFILE_SCOPE("RenderCmd setup");
		Radiant::RenderCmd::SetClearColor({ 0.12f, 0.12f, 0.12f, 1.0f });
		Radiant::RenderCmd::Clear();
	}

	// OnRender Begin scene / draw quads
	{
		RD_PROFILE_SCOPE("Renderer2d BEGIN / scene draw");
#if 0

		static float rotation = 0.0f;
		rotation += ts * 45.0f;

		Radiant::Renderer2d::BeginScene(m_camera_controller.GetCamera());
		// Testing vec3 API
		Radiant::Renderer2d::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 10.0f, 10.0f }, m_texture, 5.0f);
		Radiant::Renderer2d::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, { 0.75f, 0.33f, 0.12f, 1.0f });
		Radiant::Renderer2d::DrawRotatedQuad({ 0.5f, 0.5f, 0.0f }, { 0.5f, 0.8f }, glm::radians(37.0f), { 0.35f, 0.33f, 0.72f, 1.0f });
		Radiant::Renderer2d::DrawRotatedQuad({ -1.0f, -2.0f, 0.0f }, { 2.0f, 2.0f }, glm::radians(45.0f), m_texture, 10.0f);
		//Testing vec2 API
		Radiant::Renderer2d::DrawQuad({ 9.5f, 9.5f }, { 1.0f, 1.0f }, { 0.75f, 0.33f, 0.12f, 1.0f });
		Radiant::Renderer2d::DrawRotatedQuad({ 12.0f, 8.0f }, { 2.0f, 2.0f }, glm::radians(rotation), { 0.35f, 0.73f, 0.32f, 1.0f });
		Radiant::Renderer2d::DrawRotatedQuad({ 9.0f, 8.0f }, { 2.0f, 2.0f }, glm::radians(45.0f), m_texture, 10.0f);
		Radiant::Renderer2d::DrawQuad({ 10.0f, 10.0f }, { 10.0f, 10.0f }, m_texture, 5.0f);

		// Stress test for draw calls and max quads
		for (float y = -10.0f; y < 10.0f; y += 0.5f)
		{
			for (float x = -10.0f; x < 10.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 0.5f) / 10.0f, 0.5f};
				Radiant::Renderer2d::DrawQuad({ 5.0f + x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Radiant::Renderer2d::EndScene();
#endif
		Radiant::Renderer2d::BeginScene(m_camera_controller.GetCamera());
		Radiant::Renderer2d::DrawQuad({ 0.0f, 0.0f, 0.2f }, { 1.0f, 1.0f }, m_grass);
		Radiant::Renderer2d::EndScene();
	}
}

void Sandbox2d::OnEvent(Radiant::Event& e)
{
	m_camera_controller.OnEvent(e);
}
