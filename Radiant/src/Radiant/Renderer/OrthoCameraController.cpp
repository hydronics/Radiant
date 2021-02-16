#include "rdpch.h"

#include "OrthoCameraController.h"

namespace Radiant {

	OrthoCameraController::OrthoCameraController(float width, float height, bool rotatable)
		 : m_aspect_ratio(width / height)
		, m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level)
		, m_rotatable(rotatable)
	{
		RD_PROFILE_FUNCTION();
	}

	OrthoCameraController::OrthoCameraController(float aspect_ratio, bool rotatable)
		: m_aspect_ratio(aspect_ratio)
		, m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level)
		, m_rotatable(rotatable)
	{
		RD_PROFILE_FUNCTION();
	}

	void OrthoCameraController::OnUpdate(Timestep timestep)
	{
		RD_PROFILE_FUNCTION();

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

		m_camera.SetPosition(m_camera_position);
		m_camera_translation_speed = m_zoom_level;
	}

	void OrthoCameraController::OnEvent(Event& e)
	{
		RD_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RD_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RD_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
	}

	void OrthoCameraController::ResizeCameraBounds(float w, float h)
	{
		m_aspect_ratio = w / h;
		m_camera.SetProjection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		RD_PROFILE_FUNCTION();

		// zoom is scaled by e.getyoffset()
		// clamp new zoom to between 0 -> 1, to avoid flipping screen when we zoom with negative values.
		m_zoom_level -= e.GetYOffset() * m_camera_zoom_speed;
		m_zoom_level = std::max(m_zoom_level, m_camera_minimum_zoom);
		m_camera.SetProjection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);

		return false;
	}

	bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		RD_PROFILE_FUNCTION();

		ResizeCameraBounds((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}

}
