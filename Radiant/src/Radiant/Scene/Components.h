#pragma once

#include <glm/glm.hpp>

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

}
