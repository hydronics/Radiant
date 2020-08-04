#pragma once

#include "Radiant.h"

class Sandbox2d : public Radiant::Layer
{
public:
	Sandbox2d();
	virtual ~Sandbox2d();

	void OnAttach() override;
	void OnDetach() override;

	void OnImGuiRender() override;

	void OnUpdate(Radiant::Timestep timestep) override;
	void OnEvent(Radiant::Event& e) override;

private:
	Radiant::OrthoCameraController m_camera_controller;
	glm::vec4 m_color = { 0.1f, 0.2f, 0.7f, 1.0f };

	Radiant::Ref<Radiant::Texture2d> m_texture;
};
