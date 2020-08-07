#include "ShadesmarEditorLayer.h"
#include "Radiant/Renderer/Renderer2d.h"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>


ShadesmarEditorLayer::ShadesmarEditorLayer() 
#ifdef RD_DEBUG
	: Layer("ShadesmarEditorLayer")
#else
	: Layer()
#endif
	, m_camera_controller((float)Radiant::Application::Get().GetWindow().GetWidth() / (float)Radiant::Application::Get().GetWindow().GetHeight())
{
}

ShadesmarEditorLayer::~ShadesmarEditorLayer()
{

}

void ShadesmarEditorLayer::OnAttach()
{
	m_texture = Radiant::Texture2d::Create("assets/textures/Checkerboard.png");
	m_rpg_sprite_sheet = Radiant::Texture2d::Create("assets/textures/RPGpack_sheet_2X.png");
	m_barrel = Radiant::SubTexture2d::CreateFromCoords(m_rpg_sprite_sheet, { 8, 1 }, { 128, 128 }, { 1, 1 });

	m_color_frame_buffer = Radiant::FrameBuffer::Create({
		Radiant::Application::Get().GetWindow().GetWidth(),
		Radiant::Application::Get().GetWindow().GetHeight()
	});
}

void ShadesmarEditorLayer::OnDetach()
{
}

void ShadesmarEditorLayer::OnImGuiRender()
{
	static bool dockspace_open = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);


	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Docking"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				Radiant::Application::Get().Close();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");
	auto stats = Radiant::Renderer2d::GetStats();
	ImGui::Text("Radiant Renderer2d Statistics:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Total Vertex Count: %d", stats.GetTotalVertexCount());
	ImGui::Text("Total Index Count: %d", stats.GetTotalIndexCount());
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport");
	ImVec2 avail_viewport = ImGui::GetContentRegionAvail();
	if (m_viewport_size != *(glm::vec2*)&avail_viewport)
	{
		m_color_frame_buffer->Resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
		m_viewport_size = { avail_viewport.x, avail_viewport.y };
		m_camera_controller.ResizeCameraBounds((float)m_viewport_size.x, (float)m_viewport_size.y);
	}
	ImGui::PopStyleVar();

	auto frame_buffer_id = m_color_frame_buffer->GetColorAttachmentId();
	ImGui::Image((void*)(uint64_t)frame_buffer_id, ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();

	ImGui::End();
}

void ShadesmarEditorLayer::OnUpdate(Radiant::Timestep timestep)
{
	RD_PROFILE_FUNCTION();

	float ts = timestep;

	m_color_frame_buffer->Bind();

	// OnUpdate phase
	m_camera_controller.OnUpdate(timestep);

	// OnRender preparation
	Radiant::Renderer2d::ResetStats();
	{
		RD_PROFILE_SCOPE("RenderCmd setup");
		Radiant::RenderCmd::SetClearColor({ 0.22f, 0.12f, 0.55f, 1.0f });
		Radiant::RenderCmd::Clear();
	}

	// OnRender Begin scene / draw quads
	{
		RD_PROFILE_SCOPE("Renderer2d BEGIN / scene draw");

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

		Radiant::Renderer2d::BeginScene(m_camera_controller.GetCamera());
		Radiant::Renderer2d::DrawQuad({ 0.0f, 0.0f, 0.2f }, { 1.0f, 1.0f }, m_barrel);
		Radiant::Renderer2d::EndScene();
	}

	m_color_frame_buffer->Unbind();
}

void ShadesmarEditorLayer::OnEvent(Radiant::Event& e)
{
	m_camera_controller.OnEvent(e);
}
