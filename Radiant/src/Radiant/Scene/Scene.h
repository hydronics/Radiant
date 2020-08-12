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

		// General update during the game loop
		void OnUpdate(Timestep ts);

		// Scene keeps track of the viewport for the main camera to resize properly.
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		// Keep hold of the entt::registry, though this might be held by SceneManager or SceneGraph.
		// Having a single registry, rather than one per Scene, could provide a performance boost
		entt::registry m_registry;
		uint32_t m_viewport_width = 0, m_viewport_height = 0;

		friend class Entity;
	};

}