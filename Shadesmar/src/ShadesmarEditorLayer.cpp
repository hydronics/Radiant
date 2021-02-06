#include "ShadesmarEditorLayer.h"
#include "Radiant/Renderer/Renderer2d.h"

#include <imgui/imgui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

namespace Radiant {
	
	ShadesmarEditorLayer::EditorState ShadesmarEditorLayer::s_editor_state;
	ShadesmarEditorLayer::UserSettings ShadesmarEditorLayer::s_user_settings;

	ShadesmarEditorLayer::ShadesmarEditorLayer() 
	#ifdef RD_DEBUG
		: Layer("ShadesmarEditorLayer")
	#else
		: Layer()
	#endif
		, m_camera_controller((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight())
	{
		UseDefaultEditorLightTheme();
		UseDefaultEditorDarkTheme();
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

		if (!s_editor_state.active_scene)
		{
			// By default always loads into an empty scene.  Scene will include a xz-plane grid
			s_editor_state.active_scene = CreateRef<RDScene>(); 
		}

		m_square_entity = ActiveScene()->CreateEntity("square");
		m_square_entity.AddComponent<SpriteComponent>(glm::vec4{ 0.1f, 0.8f, 0.1f, 1.0f });

		m_primary_camera_entity = ActiveScene()->CreateEntity("main_camera");
		auto& cam_comp_ref = m_primary_camera_entity.AddComponent<CameraComponent>();
		cam_comp_ref.FixedAspectRatio = false;
		cam_comp_ref.Primary = true;

		m_second_camera_entity = ActiveScene()->CreateEntity("other_camera");
		m_second_camera_entity.AddComponent<CameraComponent>().Primary = false;

		class CameraController : public RDScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
			}

			virtual void OnDestroy() override
			{
			}

			virtual void OnUpdate(Timestep ts) override
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				float spd = 5.0f;

				if (Input::IsKeyPressed(RD_KEY_A))
					transform[3][0] -= spd * ts;
				if (Input::IsKeyPressed(RD_KEY_D))
					transform[3][0] += spd * ts;
				if (Input::IsKeyPressed(RD_KEY_W))
					transform[3][1] += spd * ts;
				if (Input::IsKeyPressed(RD_KEY_S))
					transform[3][1] -= spd * ts;
			}
		};

		m_primary_camera_entity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		SceneHierarchyPanel.SetContext(ActiveScene());
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

			ActiveScene()->OnViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
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

		ActiveScene()->OnUpdate(timestep);

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
		
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace Initialization and turning it on.
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}


			///////////////////////////////////
			///		Basic menu bar			///
			///////////////////////////////////
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Docking"))
				{
					if (ImGui::MenuItem("Exit"))
					{
						Application::Get().Close();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			///////////////////////////////////
			///		Viewport				///
			///////////////////////////////////
			{
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

			///////////////////////////////////
			///		Scene Hierarchy         ///
			///////////////////////////////////
			SceneHierarchyPanel.OnImGuiRender();

			///////////////////////////////////
			///		Editor State			///
			///////////////////////////////////
			ImGui::Begin("Editor Settings");
			{
				ImGui::Text(ActiveScene()->GetName().c_str());
				ImGui::BeginChild("");
				{
					//// Camera Orbit vs Fly
					if (ImGui::Checkbox("Camera Orbit", &s_editor_state.camera_orbit))
					{
						// TODO: Handle changing camera rotation style
					}

					//// Camera Perspective (Ortho vs. Perspective)
					if (ImGui::Checkbox("Camera Orthographic", &s_editor_state.camera_use_ortho))
					{
						// TODO: Handle changing camera projection style
					}
					//// Camera Free-orientation vs. FPS (Camera Roll is disabled)
					if (ImGui::Checkbox("Camera FPS-style", &s_editor_state.camera_fps))
					{
						// TODO: Handle changing camera movement style
					}

					auto translate = s_editor_state.transform_type == XType::Translate;
					if (ImGui::RadioButton("Translation", translate))
					{
						s_editor_state.transform_type = XType::Translate;
					}
					auto rotate = s_editor_state.transform_type == XType::Rotate;
					if (ImGui::RadioButton("Rotate", rotate))
					{
						s_editor_state.transform_type = XType::Rotate;
					}
					auto scale = s_editor_state.transform_type == XType::Scale;
					if (ImGui::RadioButton("Scale", scale))
					{
						s_editor_state.transform_type = XType::Scale;
					}

					// Display grid enable
					if (ImGui::Checkbox("Display Grid", &s_editor_state.enable_grid_display))
					{
						// TODO: Handle displaying or hiding grid
					}
					// Grid-snapping enable
					if (ImGui::Checkbox("Enable Snap-To Grid", &s_editor_state.enable_grid_snapping))
					{
						// TODO: Handle enabling the snap-to grid state.  Allow transform changes happen at discrete intervals, set by spacing dimentions below.
					}
					// Grid-dimensions and spacing
					if (ImGui::DragFloat2("Grid Dimensions", glm::value_ptr(s_editor_state.grid_dimensions)))
					{
						// TODO: Handle setting new dimension sizes to grid.
					}
					if (ImGui::DragFloat2("Grid Cell Spacing", glm::value_ptr(s_editor_state.grid_spacing)))
					{
						// TODO: Handle setting new Grid spacing sizes.
					}
				}
				ImGui::EndChild();
			}
			ImGui::End();

			///////////////////////////////////
			///		Renderer Debug Info		///
			///////////////////////////////////
			{
				ImGui::Begin("Renderer Debug Info");
				auto stats = Renderer2d::GetStats();
				ImGui::Text("Radiant Renderer2d Statistics:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quad Count: %d", stats.QuadCount);
				ImGui::Text("Total Vertex Count: %d", stats.GetTotalVertexCount());
				ImGui::Text("Total Index Count: %d", stats.GetTotalIndexCount());
				ImGui::End();
			}
		}
		// Active Scene Properties : When an scene is loaded and active in Shadesmar, a list of Scene global properties

		// ContentNavigator : A general file/path utility display which shows all files and allows interaction with only Shadesmar assets (i.e. src code, VS proj fiels would be greyed out or ignored entirely)
		// SceneList : Editor needs to reference an ASSET_SCENE directory to find scenes to display.
	}

	void ShadesmarEditorLayer::UseDefaultEditorLightTheme()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
		colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.90f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_DockingPreview] = colors[ImGuiCol_Header] * ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

	void ShadesmarEditorLayer::UseDefaultEditorDarkTheme()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);

		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);

		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);

		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

		colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

		colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);

		colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);

		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);

		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);

		colors[ImGuiCol_DockingPreview] = colors[ImGuiCol_HeaderActive] * ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);

		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

}
