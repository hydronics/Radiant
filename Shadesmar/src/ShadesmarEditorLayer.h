#pragma once

#include "Radiant.h"

class ShadesmarEditorLayer : public Radiant::Layer
{
public:
	ShadesmarEditorLayer();
	virtual ~ShadesmarEditorLayer();

	void OnAttach() override;
	void OnDetach() override;

	void OnImGuiRender() override;

	void OnUpdate(Radiant::Timestep timestep) override;
	void OnEvent(Radiant::Event& e) override;

private:
	Radiant::OrthoCameraController m_camera_controller;

	Radiant::Ref<Radiant::Texture2d> m_texture;
	Radiant::Ref<Radiant::Texture2d> m_rpg_sprite_sheet;
	Radiant::Ref<Radiant::SubTexture2d> m_barrel;
	Radiant::Ref<Radiant::FrameBuffer> m_color_frame_buffer;

	glm::vec2 m_viewport_size = { 1920.0f, 1080.0f };
};
