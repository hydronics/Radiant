#pragma once

#include <glm/glm.hpp>
#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Radiant {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& t)
			: Tag(t) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(glm::mat4 trans) : Transform(trans) {}

		operator glm::mat4&() { return Transform; }
		operator const glm::mat4&() const { return Transform; }
	};

	struct SpriteComponent
	{
		glm::vec4 Color;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(glm::vec4 col)
			: Color(col) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		// TODO: Remove this later.
		// These should be handled by a Scene or SceneGraph.
		bool Primary = false;
		// whether this camera cares for window resizes or not
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*CreateInstanceFunction)();
		void (*DestroyInstanceFunction)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			CreateInstanceFunction = []() {
				return static_cast<ScriptableEntity*>(new T());
			};

			DestroyInstanceFunction = [](NativeScriptComponent* comp) {
				delete (T*)comp->Instance;
				comp->Instance = nullptr;
			};
		}
	};

}
