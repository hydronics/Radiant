#pragma once

#include "Radiant/Core/Timestep.h"
#include "Radiant/Core/Input.h"
#include "Radiant/Core/KeyCodes.h"
#include "Radiant/Events/Event.h"
#include "Radiant/Events/MouseEvents.h"
#include "Radiant/Events/ApplicationEvents.h"
#include "Radiant/Renderer/OrthoCamera.h"

namespace Radiant {

	class OrthoCameraController
	{
	public:
		OrthoCameraController(float width, float height, bool rotatable = false);
		OrthoCameraController(float aspect_ratio, bool rotatable = false);

		OrthoCamera& GetCamera() { return m_camera; }
		const OrthoCamera& GetCamera() const { return m_camera; }

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);

	private:
		bool OnMouseScrolled(MouseScrollEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		// Data needed to support camera movement.
		float m_aspect_ratio = (1280.0f / 720.0f);
		float m_zoom_level = 1.0f;
		OrthoCamera m_camera;

		glm::vec3 m_camera_position = { 0.0f, 0.0f, 0.0f };
		float m_camera_translation_speed = 5.0f;

		bool m_rotatable = false;
		float m_camera_rotation = 0.0f;
		float m_camera_rotation_speed = 90.0f;

		float m_camera_zoom_speed = 0.25f;
		float m_camera_minimum_zoom = 0.25f;
	};

}