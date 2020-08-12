#pragma once

#include <glm/glm.hpp>
#include "SceneCamera.h"

namespace Radiant {

	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& t)
			: tag(t) {}
	};

	struct TransformComponent
	{
		glm::mat4 transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(glm::mat4 trans) : transform(trans) {}

		operator glm::mat4&() { return transform; }
		operator const glm::mat4&() const { return transform; }
	};

	struct SpriteComponent
	{
		glm::vec4 color;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(glm::vec4 col)
			: color(col) {}
	};

	struct CameraComponent
	{
		SceneCamera camera;
		// TODO: Remove this later.
		// These should be handled by a Scene or SceneGraph.
		bool primary = false;
		// whether this camera cares for window resizes or not
		bool fixed_aspect_ratio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

}
