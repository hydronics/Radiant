#pragma once

#include <entt.hpp>
#include <glm/glm.hpp>

#include "Radiant/Core/Timestep.h"

namespace Radiant {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name);

		void OnUpdate(Timestep ts);

	private:
		// Keep hold of the entt::registry, though this might be held by SceneManager or SceneGraph.
		// Having a single registry, rather than one per Scene, could provide a performance boost
		entt::registry m_registry;

		friend class Entity;
	};

}
