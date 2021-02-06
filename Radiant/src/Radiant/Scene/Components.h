#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "RDSceneCamera.h"
#include "RDScriptableEntity.h"

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
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(glm::vec3 translation) : Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 }) *
				glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 }) *
				glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteComponent
	{
		glm::vec4 Color{ 0.0f, 0.0f, 0.0f, 1.0f };

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(glm::vec4 col)
			: Color(col) {}
	};

	struct CameraComponent
	{
		RDSceneCamera Camera;
		// TODO: Remove this later.
		// These should be handled by a Scene or SceneGraph.
		bool Primary = true;
		// whether this camera cares for window resizes or not
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		RDScriptableEntity* Instance = nullptr;

		RDScriptableEntity*(*CreateInstanceFunction)();
		void (*DestroyInstanceFunction)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			CreateInstanceFunction = []() {
				return static_cast<RDScriptableEntity*>(new T());
			};

			DestroyInstanceFunction = [](NativeScriptComponent* comp) {
				delete (T*)comp->Instance;
				comp->Instance = nullptr;
			};
		}
	};

}
