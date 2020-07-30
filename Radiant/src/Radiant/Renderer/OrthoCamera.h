#pragma once

#include <glm/glm.hpp>

namespace Radiant {

	class OrthoCamera
	{
	public:
		OrthoCamera(float left = -1.0f, float right = 1.0f, float bottom = -1.0f, float top = 1.0f, float n = -1.0f, float f = 1.0f);

		void SetProjection(float left, float right, float bottom, float top, float n = -1.0f, float f = 1.0f);

		//Getters
		inline const glm::mat4 GetViewMatrix() const { return m_view; }
		inline const glm::mat4 GetProjectionMatrix() const { return m_projection; }
		inline const glm::mat4 GetViewProjectionMatrix() const { return m_view_projection; }
		inline const glm::vec3 GetPosition() const { return m_position; }
		inline const float GetRotation() const { return m_rotation; }

		//Setters
		void SetPosition(const glm::vec3& pos) { m_position = pos; RecalculateViewMatrix(); }
		void SetRotation(float rot) { m_rotation = rot; RecalculateViewMatrix(); }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::mat4 m_view_projection;
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		float m_rotation = 0.0f;
	};

}
