#pragma once

#include "Radiant.h"
#include "Radiant/Renderer/EditorCamera.h"

#include "Panels/SceneHierarchyPanel.h"

namespace Radiant {

	class ShadesmarEditorLayer : public Layer
	{
		struct EditorTheme
		{
			// TODO: Needs to contain a vec4 for every color we can modify in the ImGui color theme.

		};

	public:
		ShadesmarEditorLayer();
		virtual ~ShadesmarEditorLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnImGuiRender() override;

		void OnUpdate(Timestep timestep) override;
		void OnEvent(Event& e) override;

		// Event handler for detecting shortcut inputs for Save/Load
	private:
		bool OnKeyPressed(KeyboardPressedEvent& e);
		void OpenFile();
		void SaveAs();
		void NewScene();

	private:
		//Shadesmar UI Theme: 
		//	Theme-priority order: UseTheme(filepath), $client_root/RadiantEditorThemeConfig.json, Radiant internal defaults
		//	Default functions for setting Shadesmar defaults.  Only used when client theme isn't found.  UseTheme will override theme if called after theme has been set in Radiant.
		//	(these are hard-coded, not read from a config so no user modification can happen easily to these defaults)
		void UseDefaultEditorDarkTheme();
		void UseDefaultEditorLightTheme();

		//	ParseConfig file
		void LoadTheme(Ref<EditorTheme>);
		Ref<EditorTheme> LoadThemeFromFile(const std::string& ConfigFile);

	private:
		SceneHierarchyPanel SceneHierarchyPanel;

	private:
		// Active scene : includes access to all entities and components
		Ref<Scene> ActiveScene{ nullptr };

		EditorCamera EditorCam;
		int CurrentGizmoMode{ -1 };

		Ref<FrameBuffer> ColorFrameBuffer;

		glm::vec2 ViewportSize{ 1280.0f, 720.0f };
		bool ViewportFocused = false;
		bool ViewportHovered = false;

		struct EditorState
		{
			// Camera Orbit vs Fly
			bool camera_orbit{ true };
			// Camera Perspective (Ortho vs. Perspective)
			bool camera_use_ortho{ true };
			// Camera Free-orientation vs. FPS (Camera Roll is disabled)
			bool camera_fps{ false };
			//  (These only effects the main camera, all added cameras specify this as part of their component)
			//  (When another camera is made active, it does not take over as main camera; it just uses its internal data, not the ones in EditorState)
			//  (Main camera cannot be deleted; however any other camera can be designated as Main, not just the default Shadesmar cam)

			// Grid-snapping enable
			bool enable_grid_snapping{ false };
			// Display grid enable
			bool enable_grid_display{ true };
			// Grid-dimensions and spacing
			glm::vec2 grid_dimensions{ 10.0f, 10.0f };
			glm::vec2 grid_spacing{ 1.0f, 1.0f };
		};
		// There should only be one state for the editor that is maintained.
		EditorState EditorState;
	};

}
