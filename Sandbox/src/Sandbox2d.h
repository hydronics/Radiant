#pragma once

#include "Radiant.h"

class Sandbox2d : public Radiant::Layer
{
public:
	Sandbox2d();
	virtual ~Sandbox2d();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Radiant::Timestep timestep) override;
	void OnImGuiRender() override;
	void OnEvent(Radiant::Event& e) override;

private:
	Radiant::Ref<Radiant::Shader> m_texture_shader;
	Radiant::Ref<Radiant::VertexArray> m_vertex_array;

	Radiant::Ref<Radiant::Shader> m_flat_color_shader;
	Radiant::Ref<Radiant::VertexArray> m_square_va;

	Radiant::OrthoCameraController m_camera_controller;
	glm::vec4 m_color = { 0.1f, 0.2f, 0.7f, 1.0f };

	Radiant::Ref<Radiant::Texture2d> m_texture;
};