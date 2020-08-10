#pragma once

#include "Radiant.h"

namespace Radiant {

	class ShadesmarEditorLayer : public Layer
	{
	public:
		ShadesmarEditorLayer();
		virtual ~ShadesmarEditorLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnImGuiRender() override;

		void OnUpdate(Timestep timestep) override;
		void OnEvent(Event& e) override;

	private:
		OrthoCameraController m_camera_controller;

		Ref<Texture2d> m_texture;
		Ref<Texture2d> m_rpg_sprite_sheet;
		Ref<SubTexture2d> m_barrel;
		Ref<FrameBuffer> m_color_frame_buffer;

		Entity m_square_entity;

		bool m_viewport_focused = false, m_viewport_hovered = false;
		glm::vec2 m_viewport_size = { 1920.0f, 1080.0f };
		glm::vec4 m_select_color = {0.3f, 0.3f, 0.3f, 1.0f};

		Ref<Scene> m_active_scene;
	};

}
