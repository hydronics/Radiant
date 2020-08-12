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

		m_square_entity = m_active_scene->CreateEntity("square");
		m_square_entity.AddComponent<SpriteComponent>(glm::vec4{ 0.1f, 0.8f, 0.1f, 1.0f });

		m_camera_entity = m_active_scene->CreateEntity("main_camera");
		auto& cam_comp_ref = m_camera_entity.AddComponent<CameraComponent>();
		cam_comp_ref.fixed_aspect_ratio = false;
		cam_comp_ref.primary = true;

		m_clip_camera_entity = m_active_scene->CreateEntity("other_camera");
		m_clip_camera_entity.AddComponent<CameraComponent>().primary = false;
	}

	void ShadesmarEditorLayer::OnDetach()
	{
	}

	void ShadesmarEditorLayer::OnUpdate(Timestep timestep)
	{
		RD_PROFILE_FUNCTION();

		float ts = timestep;

		if (FrameBufferProperties spec = m_color_frame_buffer->GetProps();
			m_viewport_size.x > 0.0f && m_viewport_size.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != m_viewport_size.x || spec.height != m_viewport_size.y))
		{
			m_color_frame_buffer->Resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
			m_camera_controller.ResizeCameraBounds((float)m_viewport_size.x, (float)m_viewport_size.y);

			m_active_scene->OnViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
		}

		// OnRender preparation
		Renderer2d::ResetStats();
		m_color_frame_buffer->Bind();
		RenderCmd::SetClearColor({ 0.22f, 0.12f, 0.55f, 1.0f });
		RenderCmd::Clear();

		// OnUpdate phase
		if (m_viewport_focused)
		{
			m_camera_controller.OnUpdate(timestep);
		}

		m_active_scene->OnUpdate(timestep);

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

		ImGui::DragFloat3("Camera transform", glm::value_ptr(m_camera_entity.GetComponent<TransformComponent>().transform[3]));
		if (ImGui::Checkbox("Primary Camera", &m_primary_camera))
		{
			m_camera_entity.GetComponent<CameraComponent>().primary = m_primary_camera;
			m_clip_camera_entity.GetComponent<CameraComponent>().primary = !m_primary_camera;
		}

		auto& cam = m_clip_camera_entity.GetComponent<CameraComponent>().camera;
		float size = cam.GetOrthoSize();
		if ( ImGui::DragFloat("Camera transform", &size) )
		{
			m_clip_camera_entity.GetComponent<CameraComponent>().camera.SetOrthoSize(size);
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_viewport_focused = ImGui::IsWindowFocused();
		m_viewport_hovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_viewport_focused || !m_viewport_hovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_viewport_size = { viewportPanelSize.x, viewportPanelSize.y };

		auto frame_buffer_id = m_color_frame_buffer->GetColorAttachmentId();
		ImGui::Image((void*)(uint64_t)frame_buffer_id, ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

}
