#pragma once

#include <entt.hpp>
#include <glm/glm.hpp>

#include "Radiant/Core/Timestep.h"

namespace Radiant {

	class RDEntity;

	class RDScene
	{
	public:
		RDScene();
		~RDScene();

		RDEntity CreateEntity(const std::string& name);
		void DestroyEntity(RDEntity entity);

		// General update during the game loop
		void OnUpdate(Timestep ts);

		// Scene keeps track of the viewport for the main camera to resize properly.
		void OnViewportResize(uint32_t width, uint32_t height);

		const std::string& GetName() const { return Name; }
		void SetName(const std::string& name) { Name = name; }

	private:
		template<typename T>
		void OnComponentAdded(RDEntity entity, T& comopnent);

	private:
		// Keep hold of the entt::registry, though this might be held by SceneManager or SceneGraph.
		// Having a single registry, rather than one per Scene, could provide a performance boost
		entt::registry Registry;
		uint32_t ViewportWidth = 0, ViewportHeight = 0;

		// TODO: Implement a GUID approach for all the Asset types, including Scene here
		std::string Name{ "default_scene" };

		friend class RDEntity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
