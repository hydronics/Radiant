#pragma once

#include <entt.hpp>
#include "Scene.h"

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
			return m_scene->Registry.has<T>(m_entity_handle);
		}

		template <class T, class... Args>
		T& AddComponent(Args&&... args)
		{
			RD_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			T& component = m_scene->Registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
			m_scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template <class T>
		T& GetComponent()
		{
			RD_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			return m_scene->Registry.get<T>(m_entity_handle);
		}

		template <class T>
		void RemoveComponent()
		{
			RD_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
			m_scene->Registry.remove<T>(m_entity_handle);
		}

		operator bool() const { return m_entity_handle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_entity_handle; }
		operator entt::entity() const { return m_entity_handle; }
		bool operator==(const Entity& other) const { return m_entity_handle == other.m_entity_handle && m_scene == other.m_scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_entity_handle{ entt::null };
		Scene* m_scene = nullptr;
	};

}
