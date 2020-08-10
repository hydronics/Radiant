#include "ShadesmarEditorLayer.h"
#include "Radiant/Renderer/Renderer2d.h"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace Radiant {

	ShadesmarEditorLayer::ShadesmarEditorLayer() 
	#ifdef RD_DEBUG
		: Layer("ShadesmarEditorLayer")
	#else
		: Layer()
	#endif
		, m_camera_controller((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight())
	{
	}

	ShadesmarEditorLayer::~ShadesmarEditorLayer()
	{

	}

	void ShadesmarEditorLayer::OnAttach()
	{
		m_texture = Texture2d::Create("assets/textures/Checkerboard.png");
		m_rpg_sprite_sheet = Texture2d::Create("assets/textures/RPGpack_sheet_2X.png");
		m_barrel = SubTexture2d::CreateFromCoords(m_rpg_sprite_sheet, { 8, 1 }, { 128, 128 }, { 1, 1 });

		m_color_frame_buffer = FrameBuffer::Create({
			Application::Get().GetWindow().GetWidth(),
			Application::Get().GetWindow().GetHeight()
		});

		m_active_scene = CreateRef<Scene>();
		auto square = m_active_scene->CreateEntity("square");
		square.AddComponent<SpriteComponent>(glm::vec4{ 0.1f, 0.8f, 0.1f, 1.0f });
		m_square_entity = square;
	}

	void ShadesmarEditorLayer::OnDetach()
	{
	}

	void ShadesmarEditorLayer::OnUpdate(Timestep timestep)
	{
		RD_PROFILE_FUNCTION();

		float ts = timestep;

		// OnUpdate phase
		if (m_viewport_focused)
		{
			m_active_scene->OnUpdate(timestep);
			m_camera_controller.OnUpdate(timestep);
		}

		// OnRender preparation
		Renderer2d::ResetStats();
		m_color_frame_buffer->Bind();
		RenderCmd::SetClearColor({ 0.22f, 0.12f, 0.55f, 1.0f });
		RenderCmd::Clear();

		static float rotation = 0.0f;
		rotation += ts * 45.0f;

		Renderer2d::BeginScene(m_camera_controller.GetCamera());

		m_active_scene->OnUpdate(timestep);

		Renderer2d::EndScene();

		m_color_frame_buffer->Unbind();
	}

	void ShadesmarEditorLayer::OnEvent(Event& e)
	{
		m_camera_controller.OnEvent(e);
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
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
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
					Application::Get().Close();
					ImGui::EndMenu();
				}
			}
			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		auto stats = Renderer2d::GetStats();
		ImGui::Text("Radiant Renderer2d Statistics:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Total Vertex Count: %d", stats.GetTotalVertexCount());
		ImGui::Text("Total Index Count: %d", stats.GetTotalIndexCount());
		
		if (m_square_entity)
		{
			ImGui::Separator();
			ImGui::Text("%s", m_square_entity.GetComponent<TagComponent>().tag.c_str());
			auto& color = m_square_entity.GetComponent<SpriteComponent>().color;
			ImGui::ColorEdit4("Color", glm::value_ptr(color));
			ImGui::Separator();
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_viewport_focused = ImGui::IsWindowFocused();
		m_viewport_hovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_viewport_focused || !m_viewport_hovered);

		ImVec2 avail_viewport = ImGui::GetContentRegionAvail();
		if (m_viewport_size != *(glm::vec2*) & avail_viewport && avail_viewport.x > 0 && avail_viewport.y > 0)
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
}
