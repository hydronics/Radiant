#include "rdpch.h"

#include "OrthoCameraController.h"

namespace Radiant {

	OrthoCameraController::OrthoCameraController(float width, float height, bool rotatable)
		 : m_aspect_ratio(width / height)
		, m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio* m_zoom_level, -m_zoom_level, m_zoom_level)
	{

	}

	OrthoCameraController::OrthoCameraController(float aspect_ratio, bool rotatable)
		: m_aspect_ratio(aspect_ratio)
		, m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio* m_zoom_level, -m_zoom_level, m_zoom_level)
	{
	}

	void OrthoCameraController::OnUpdate(Timestep timestep)
	{
		float ts = timestep;

		// Update camera based on any input for this frame
		if (Input::IsKeyPressed(RD_KEY_D))
		{
			m_camera_position.x += m_camera_translation_speed * ts;
		}
		else if (Input::IsKeyPressed(RD_KEY_A))
		{
			m_camera_position.x -= m_camera_translation_speed * ts;
		}
		if (Input::IsKeyPressed(RD_KEY_W))
		{
			m_camera_position.y += m_camera_translation_speed * ts;
		}
		else if (Input::IsKeyPressed(RD_KEY_S))
		{
			m_camera_position.y -= m_camera_translation_speed * ts;
		}
		m_camera.SetPosition(m_camera_position);

		if (m_rotatable)
		{
			if (Input::IsKeyPressed(RD_KEY_Q))
			{
				m_camera_rotation += m_camera_rotation_speed * ts;
			}
			else if (Input::IsKeyPressed(RD_KEY_E))
			{
				m_camera_rotation -= m_camera_rotation_speed * ts;
			}
			m_camera.SetRotation(m_camera_rotation);
		}
		m_camera_translation_speed = m_zoom_level;
	}

	void OrthoCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrollEvent>(RD_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RD_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrollEvent& e)
	{
		// zoom is scaled by e.getyoffset()
		// clamp new zoom to between 0 -> 1, to avoid flipping screen when we zoom with negative values.
		m_zoom_level -= e.GetYOffset() * m_camera_zoom_speed;
		m_zoom_level = std::max(m_zoom_level, m_camera_minimum_zoom);

		m_camera.SetProjection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);

		return false;
	}

	bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_aspect_ratio = (float)e.GetWidth() / (float)e.GetHeight();

		m_camera.SetProjection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);

		return false;
	}

}
