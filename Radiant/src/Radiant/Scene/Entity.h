#pragma once

#include <entt.hpp>
#include "Scene.h"
#include "Components.h"

namespace Radiant {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);
		Entity(const Entity& other) = default;

		template <class T>
		bool HasComponent()
		{
			return m_scene->m_registry.has<T>(m_entity_handle);
		}

		template <class T, class... Args>
		T& AddComponent(Args&&... args)
		{
			RD_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			return m_scene->m_registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
		}

		template <class T>
		const T& GetComponent() const
		{
			RD_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			return m_scene->m_registry.get<T>(m_entity_handle);
		}

		template <class T>
		T& GetComponent()
		{
			RD_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			return m_scene->m_registry.get<T>(m_entity_handle);
		}

		template <class T>
		T& RemoveComponent()
		{
			RD_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			return m_scene->m_registry.remove<T>(m_entity_handle);
		}

		operator const entt::entity() const { return m_entity_handle; }
		operator bool() const { return m_entity_handle != entt::null; }

	private:
		entt::entity m_entity_handle{ entt::null };
		Scene* m_scene = nullptr;
	};
}
