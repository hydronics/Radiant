#pragma once

#include <entt.hpp>
#include "RDScene.h"

namespace Radiant {

	class RDEntity
	{
	public:
		RDEntity() = default;
		RDEntity(entt::entity entity, RDScene* scene);
		RDEntity(const RDEntity& other) = default;

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
		operator uint32_t() const { return (uint32_t)m_entity_handle; }
		bool operator==(const RDEntity& other) { return m_entity_handle == other.m_entity_handle && m_scene == other.m_scene; }
		bool operator!=(const RDEntity& other) { return !operator==(other); }

	private:
		entt::entity m_entity_handle{ entt::null };
		RDScene* m_scene = nullptr;
	};

}
