#pragma once

#include "Radiant/Renderer/Camera.h"

namespace Radiant {

	class SceneCamera : public Camera 
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetViewportDimensions(uint32_t width, uint32_t height);

		// Orthographic getter/setter
		void SetOrthographic(float size, float nearClip, float farClip);
		float GetOrthographicSize() const { return OrthographicSize; }
		float GetOrthographicNear() const { return OrthographicNear; }
		float GetOrthographicFar() const { return OrthographicFar; }
		void SetOrthographicSize(float size) { OrthographicSize = size; RecalculateProjection(); }
		void SetOrthographicNear(float nearClip) { OrthographicNear = nearClip; RecalculateProjection(); }
		void SetOrthographicFar(float farClip) { OrthographicFar = farClip; RecalculateProjection(); }

		// Perspective getter/setters
		float GetPerspectiveVerticalFov() const { return PerspectiveVerticalFov; }
		float GetPerspectiveNear() const { return PerspectiveNear; }
		float GetPerspectiveFar() const { return PerspectiveFar; }
		void SetPerspectiveVerticalFov(float verticalFov) { PerspectiveVerticalFov = verticalFov; RecalculateProjection(); }
		void SetPerspectiveNear(float nearClip) { PerspectiveNear = nearClip; RecalculateProjection(); }
		void SetPerspectiveFar(float farClip) { OrthographicFar = farClip; RecalculateProjection(); }
		void SetPerspective(float verticalFov, float nearClip, float farClip);

		ProjectionType GetProjectionType() const { return ProjType; }
		void SetProjectionType(ProjectionType projectionType) { ProjType = projectionType; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		float OrthographicSize = 10.0f;
		float OrthographicNear = -1.0f,  OrthographicFar = 1.0f;

		float PerspectiveVerticalFov = glm::radians(45.0f);
		float PerspectiveNear = 0.01f, PerspectiveFar = 1000.0f;

		float AspectRatio = 1.0f;

		ProjectionType ProjType{ ProjectionType::Orthographic };
	};

}
